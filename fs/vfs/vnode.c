/*
 * Copyright (c) 2021-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "los_mux.h"
#include "fs/vfs_util.h"
#include "fs/vnode.h"
#include "fs/dirent_fs.h"
#include "fs_other.h"

LIST_HEAD g_vnodeFreeList;              /* free vnodes list */	//空闲节点链表
LIST_HEAD g_vnodeVirtualList;           /* dev vnodes list */	//虚拟设备节点链表,暂无实际的文件系统
LIST_HEAD g_vnodeCurrList;              /* inuse vnodes list */	//非虚拟设备的节点链表,有实际的文件系统(如:FAT)
static int g_freeVnodeSize = 0;         /* system free vnodes size */	//剩余节点数量
static int g_totalVnodeSize = 0;        /* total vnode size */	//总节点数量

static LosMux g_vnodeMux;	//操作链表互斥量			
static struct Vnode *g_rootVnode = NULL;//根节点
static struct VnodeOps g_devfsOps;//节点操作

#define ENTRY_TO_VNODE(ptr)  LOS_DL_LIST_ENTRY(ptr, struct Vnode, actFreeEntry) //通过局部(actFreeEntry)找到整体(Vnode)
#define VNODE_LRU_COUNT      10		//最多回收数量
#define DEV_VNODE_MODE       0755	//0755可不是深圳的意思哈,而是节点的权限,对照以下注释理解
/*
#ifndef S_IRUSR
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXU 0700
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXG 0070
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001
#define S_IRWXO 0007
#endif
*/
int VnodesInit(void)
{
    int retval = LOS_MuxInit(&g_vnodeMux, NULL);//初始化操作vnode链表的互斥量
    if (retval != LOS_OK) {
        PRINT_ERR("Create mutex for vnode fail, status: %d", retval);
        return retval;
    }

    LOS_ListInit(&g_vnodeFreeList);		//初始化空闲的节点链表
    LOS_ListInit(&g_vnodeVirtualList);	//初始化虚拟设备节点链表
    LOS_ListInit(&g_vnodeCurrList);		//初始化当前(已在使用)的节点链表
    retval = VnodeAlloc(NULL, &g_rootVnode);//分配根节点
    if (retval != LOS_OK) {
        PRINT_ERR("VnodeInit failed error %d\n", retval);
        return retval;
    }
    g_rootVnode->mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;// 40777 (chmod 777)
    g_rootVnode->type = VNODE_TYPE_DIR;//节点类型为目录

    return LOS_OK;
}
//获取空闲节点链表,分配的节点从空闲链表里出
static struct Vnode *GetFromFreeList(void)
{
    if (g_freeVnodeSize <= 0) {
        return NULL;
    }
    struct Vnode *vnode = NULL;

    if (LOS_ListEmpty(&g_vnodeFreeList)) {
        PRINT_ERR("get vnode from free list failed, list empty but g_freeVnodeSize = %d!\n", g_freeVnodeSize);
        g_freeVnodeSize = 0;
        return NULL;
    }

    vnode = ENTRY_TO_VNODE(LOS_DL_LIST_FIRST(&g_vnodeFreeList));
    LOS_ListDelete(&vnode->actFreeEntry);//从空闲链表上摘出去.
    g_freeVnodeSize--;
    return vnode;
}
//节点批量回收, LRU是Least Recently Used的缩写，即最近最少使用，
struct Vnode *VnodeReclaimLru(void)
{
    struct Vnode *item = NULL;
    struct Vnode *nextItem = NULL;
    int releaseCount = 0;
	//遍历链表
    LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, nextItem, &g_vnodeCurrList, struct Vnode, actFreeEntry) {
        if ((item->useCount > 0) || //还有链接数
            (item->flag & VNODE_FLAG_MOUNT_ORIGIN) || //原来是个mount节点
            (item->flag & VNODE_FLAG_MOUNT_NEW)) {	//是个新mount节点
            continue;
        }

        if (VnodeFree(item) == LOS_OK) {//正式回收,其实就是将自己从链表上摘出去,再挂到空闲链表上.
            releaseCount++;
        }
        if (releaseCount >= VNODE_LRU_COUNT) {//一次最多回收量
            break;
        }
    }

    if (releaseCount == 0) {//回收失败
        PRINT_ERR("VnodeAlloc failed, vnode size hit max but can't reclaim anymore!\n");
        return NULL;
    }

    item = GetFromFreeList();//获取一个空闲节点
    if (item == NULL) {
        PRINT_ERR("VnodeAlloc failed, reclaim and get from free list failed!\n");
    }
    return item;
}
//申请分配一个 vnode 节点
int VnodeAlloc(struct VnodeOps *vop, struct Vnode **newVnode)
{
    struct Vnode* vnode = NULL;

    VnodeHold();
    vnode = GetFromFreeList();
    if ((vnode == NULL) && g_totalVnodeSize < LOSCFG_MAX_VNODE_SIZE) {
        vnode = (struct Vnode*)zalloc(sizeof(struct Vnode));
        g_totalVnodeSize++;
    }

    if (vnode == NULL) {//没有分配到节点
        vnode = VnodeReclaimLru();//执行回收算法,释放节点
    }

    if (vnode == NULL) {//回收也没有可用节点
        *newVnode = NULL;
        VnodeDrop();
        return -ENOMEM;//分配失败,返回
    }

    vnode->type = VNODE_TYPE_UNKNOWN;	//节点默认类型,未知
    LOS_ListInit((&(vnode->parentPathCaches)));//后续可能要当爸爸,做好准备
    LOS_ListInit((&(vnode->childPathCaches)));//后续可能要当儿子,做好准备
    LOS_ListInit((&(vnode->hashEntry)));	  //用它挂到全局哈希表上
    LOS_ListInit((&(vnode->actFreeEntry)));		//用它挂到全局正使用节点链表上

    if (vop == NULL) {//
        LOS_ListAdd(&g_vnodeVirtualList, &(vnode->actFreeEntry));//挂到虚拟设备
        vnode->vop = &g_devfsOps;
    } else {//如果已有指定的文件系统(FAT),直接绑定
        LOS_ListTailInsert(&g_vnodeCurrList, &(vnode->actFreeEntry));
        vnode->vop = vop;
    }
    VnodeDrop();

    *newVnode = vnode;

    return LOS_OK;
}
//是否 vnode 节点
int VnodeFree(struct Vnode *vnode)
{
    if (vnode == NULL) {
        return LOS_OK;
    }

    VnodeHold();//拿互斥锁
    if (vnode->useCount > 0) {//还有引用数,或者叫链接数.这里的链接数指的是还有硬链接没删除的情况
        VnodeDrop();
        return -EBUSY;//返回设备或资源忙着呢.
    }

    VnodePathCacheFree(vnode);//节点和父亲,孩子告别
    LOS_ListDelete(&(vnode->hashEntry));//将自己从当前哈希链表上摘出来,此时vnode通过hashEntry挂在 g_vnodeHashEntrys
    LOS_ListDelete(&vnode->actFreeEntry);//将自己从当前链表摘出来,此时vnode通过actFreeEntry挂在 g_vnodeCurrList

    if (vnode->vop->Reclaim) {
        vnode->vop->Reclaim(vnode);
    }

    memset_s(vnode, sizeof(struct Vnode), 0, sizeof(struct Vnode));
    LOS_ListAdd(&g_vnodeFreeList, &vnode->actFreeEntry);//通过 actFreeEntry链表节点挂到 空闲链表上. 

    g_freeVnodeSize++;
    VnodeDrop();//释放互斥锁

    return LOS_OK;
}

int VnodeFreeAll(const struct Mount *mount)
{
    struct Vnode *vnode = NULL;
    struct Vnode *nextVnode = NULL;
    int ret;

    LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(vnode, nextVnode, &g_vnodeCurrList, struct Vnode, actFreeEntry) {
        if ((vnode->originMount == mount) && !(vnode->flag & VNODE_FLAG_MOUNT_NEW)) {
            ret = VnodeFree(vnode);
            if (ret != LOS_OK) {
                return ret;
            }
        }
    }

    return LOS_OK;
}

BOOL VnodeInUseIter(const struct Mount *mount)
{
    struct Vnode *vnode = NULL;

    LOS_DL_LIST_FOR_EACH_ENTRY(vnode, &g_vnodeCurrList, struct Vnode, actFreeEntry) {
        if (vnode->originMount == mount) {
            if ((vnode->useCount > 0) || (vnode->flag & VNODE_FLAG_MOUNT_ORIGIN)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

int VnodeHold()
{
    int ret = LOS_MuxLock(&g_vnodeMux, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("VnodeHold lock failed !\n");
    }
    return ret;
}

int VnodeDrop()
{
    int ret = LOS_MuxUnlock(&g_vnodeMux);
    if (ret != LOS_OK) {
        PRINT_ERR("VnodeDrop unlock failed !\n");
    }
    return ret;
}

static char *NextName(char *pos, uint8_t *len)
{
    char *name = NULL;
    if (*pos == '\0') {
        return NULL;
    }
    while (*pos != 0 && *pos == '/') {
        pos++;
    }
    if (*pos == '\0') {
        return NULL;
    }
    name = (char *)pos;
    while (*pos != '\0' && *pos != '/') {
        pos++;
    }
    *len = pos - name;
    return name;
}
//处理前的准备
static int PreProcess(const char *originPath, struct Vnode **startVnode, char **path)
{
    int ret;
    char *absolutePath = NULL;
	//通过相对路径找到绝对路径
    ret = vfs_normalize_path(NULL, originPath, &absolutePath);
    if (ret == LOS_OK) {//成功
        *startVnode = g_rootVnode;//根节点为开始节点
        *path = absolutePath;//返回绝对路径
    }

    return ret;
}

static struct Vnode *ConvertVnodeIfMounted(struct Vnode *vnode)
{
    if ((vnode == NULL) || !(vnode->flag & VNODE_FLAG_MOUNT_ORIGIN)) {
        return vnode;
    }
    return vnode->newMount->vnodeCovered;
}

static void RefreshLRU(struct Vnode *vnode)
{
    if (vnode == NULL || (vnode->type != VNODE_TYPE_REG && vnode->type != VNODE_TYPE_DIR) ||
        vnode->vop == &g_devfsOps || vnode->vop == NULL) {
        return;
    }
    LOS_ListDelete(&(vnode->actFreeEntry));
    LOS_ListTailInsert(&g_vnodeCurrList, &(vnode->actFreeEntry));
}

static int ProcessVirtualVnode(struct Vnode *parent, uint32_t flags, struct Vnode **vnode)
{
    int ret = -ENOENT;
    if (flags & V_CREATE) {
        // only create /dev/ vnode
        ret = VnodeAlloc(NULL, vnode);
    }
    if (ret == LOS_OK) {
        (*vnode)->parent = parent;
    }
    return ret;
}
//一级一级向下找
static int Step(char **currentDir, struct Vnode **currentVnode, uint32_t flags)
{
    int ret;
    uint8_t len = 0;
    struct Vnode *nextVnode = NULL;
    char *nextDir = NULL;

    if ((*currentVnode)->type != VNODE_TYPE_DIR) {
        return -ENOTDIR;
    }
    nextDir = NextName(*currentDir, &len);
    if (nextDir == NULL) {
        *currentDir = NULL;
        return LOS_OK;
    }

    ret = PathCacheLookup(*currentVnode, nextDir, len, &nextVnode);
    if (ret == LOS_OK) {
        goto STEP_FINISH;
    }

    (*currentVnode)->useCount++;
    if (flags & V_DUMMY) {
        ret = ProcessVirtualVnode(*currentVnode, flags, &nextVnode);
    } else {
        if ((*currentVnode)->vop != NULL && (*currentVnode)->vop->Lookup != NULL) {
            ret = (*currentVnode)->vop->Lookup(*currentVnode, nextDir, len, &nextVnode);
        } else {
            ret = -ENOSYS;
        }
    }
    (*currentVnode)->useCount--;

    if (ret == LOS_OK) {
        (void)PathCacheAlloc((*currentVnode), nextVnode, nextDir, len);
    }

STEP_FINISH:
    nextVnode = ConvertVnodeIfMounted(nextVnode);
    RefreshLRU(nextVnode);

    *currentDir = nextDir + len;
    if (ret == LOS_OK) {
        *currentVnode = nextVnode;
    }

    return ret;
}
//通过路径 查找索引节点.路径和节点是 N:1的关系, 硬链接 
int VnodeLookup(const char *path, struct Vnode **result, uint32_t flags)
{
    struct Vnode *startVnode = NULL;
    char *normalizedPath = NULL;


    int ret = PreProcess(path, &startVnode, &normalizedPath);//找到根节点和绝对路径
    if (ret != LOS_OK) {
        PRINT_ERR("[VFS]lookup failed, invalid path=%s err = %d\n", path, ret);
        goto OUT_FREE_PATH;
    }

    if (normalizedPath[0] == '/' && normalizedPath[1] == '\0') {//如果是根节点 
        *result = g_rootVnode;//啥也不说了,还找啥呀,直接返回根节点
        free(normalizedPath);
        return LOS_OK;
    }

    char *currentDir = normalizedPath;
    struct Vnode *currentVnode = startVnode;

    while (currentDir && *currentDir != '\0') {//循环一直找到结尾
        ret = Step(&currentDir, &currentVnode, flags);//一级一级向下找
        if (*currentDir == '\0') {//找到最后了,返回目标节点或父虚拟节点
            // return target or parent vnode as result
            *result = currentVnode;//找到了
        } else if (VfsVnodePermissionCheck(currentVnode, EXEC_OP)) {
            ret = -EACCES;
            goto OUT_FREE_PATH;
        }

        if (ret != LOS_OK) {
            // no such file, lookup failed
            goto OUT_FREE_PATH;
        }
    }

OUT_FREE_PATH:
    if (normalizedPath) {
        free(normalizedPath);
    }

    return ret;
}

static void ChangeRootInternal(struct Vnode *rootOld, char *dirname)
{
    int ret;
    struct Mount *mnt = NULL;
    char *name = NULL;
    struct Vnode *node = NULL;
    struct Vnode *nodeInFs = NULL;
    struct PathCache *item = NULL;
    struct PathCache *nextItem = NULL;

    LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, nextItem, &rootOld->childPathCaches, struct PathCache, childEntry) {
        name = item->name;
        node = item->childVnode;

        if (strcmp(name, dirname)) {
            continue;
        }
        PathCacheFree(item);

        ret = VnodeLookup(dirname, &nodeInFs, 0);
        if (ret) {
            PRINTK("%s-%d %s NOT exist in rootfs\n", __FUNCTION__, __LINE__, dirname);
            break;
        }

        mnt = node->newMount;
        mnt->vnodeBeCovered = nodeInFs;

        nodeInFs->newMount = mnt;
        nodeInFs->flag |= VNODE_FLAG_MOUNT_ORIGIN;

        break;
    }
}

void ChangeRoot(struct Vnode *rootNew)
{
    struct Vnode *rootOld = g_rootVnode;
    g_rootVnode = rootNew;
    ChangeRootInternal(rootOld, "proc");
    ChangeRootInternal(rootOld, "dev");
}

static int VnodeReaddir(struct Vnode *vp, struct fs_dirent_s *dir)
{
    int result;
    int cnt = 0;
    off_t i = 0;
    off_t idx;
    unsigned int dstNameSize;

    struct PathCache *item = NULL;
    struct PathCache *nextItem = NULL;

    if (dir == NULL) {
        return -EINVAL;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, nextItem, &vp->childPathCaches, struct PathCache, childEntry) {
        if (i < dir->fd_position) {
            i++;
            continue;
        }

        idx = i - dir->fd_position;

        dstNameSize = sizeof(dir->fd_dir[idx].d_name);
        result = strncpy_s(dir->fd_dir[idx].d_name, dstNameSize, item->name, item->nameLen);
        if (result != EOK) {
            return -ENAMETOOLONG;
        }
        dir->fd_dir[idx].d_off = i;
        dir->fd_dir[idx].d_reclen = (uint16_t)sizeof(struct dirent);

        i++;
        if (++cnt >= dir->read_cnt) {
            break;
        }
    }

    dir->fd_position = i;

    return cnt;
}

int VnodeOpendir(struct Vnode *vnode, struct fs_dirent_s *dir)
{
    (void)vnode;
    (void)dir;
    return LOS_OK;
}

int VnodeClosedir(struct Vnode *vnode, struct fs_dirent_s *dir)
{
    (void)vnode;
    (void)dir;
    return LOS_OK;
}
//创建节点
int VnodeCreate(struct Vnode *parent, const char *name, int mode, struct Vnode **vnode)
{
    int ret;
    struct Vnode *newVnode = NULL;

    ret = VnodeAlloc(NULL, &newVnode);//分配一个节点
    if (ret != 0) {
        return -ENOMEM;
    }

    newVnode->type = VNODE_TYPE_CHR;//字符设备
    newVnode->vop = parent->vop;//继承父节点 vop
    newVnode->fop = parent->fop;//继承父节点 fop
    newVnode->data = NULL;		//默认值
    newVnode->parent = parent;	//指定父节点
    newVnode->originMount = parent->originMount; //挂载点
    newVnode->uid = parent->uid;
    newVnode->gid = parent->gid;
    newVnode->mode = mode;

    *vnode = newVnode;
    return 0;
}
//设备初始化,设备结点：/dev目录下，对应一个设备，如/dev/mmcblk0
int VnodeDevInit()
{
    struct Vnode *devNode = NULL;
    struct Mount *devMount = NULL;

    int retval = VnodeLookup("/dev", &devNode, V_CREATE | V_CACHE | V_DUMMY);//通过名称找到索引节点
    if (retval != LOS_OK) {
        PRINT_ERR("VnodeDevInit failed error %d\n", retval);
        return retval;
    }
    devNode->mode = DEV_VNODE_MODE | S_IFDIR; // 40755(chmod 755)
    devNode->type = VNODE_TYPE_DIR;	//设备根节点目录

    devMount = MountAlloc(devNode, NULL);//分配一个mount节点
    if (devMount == NULL) {
        PRINT_ERR("VnodeDevInit failed mount point alloc failed.\n");
        return -ENOMEM;
    }
    devMount->vnodeCovered = devNode;
    devMount->vnodeBeCovered->flag |= VNODE_FLAG_MOUNT_ORIGIN;
    return LOS_OK;
}

int VnodeGetattr(struct Vnode *vnode, struct stat *buf)
{
    (void)memset_s(buf, sizeof(struct stat), 0, sizeof(struct stat));
    buf->st_mode = vnode->mode;
    buf->st_uid = vnode->uid;
    buf->st_gid = vnode->gid;

    return LOS_OK;
}

struct Vnode *VnodeGetRoot()
{
    return g_rootVnode;
}

static int VnodeChattr(struct Vnode *vnode, struct IATTR *attr)
{
    mode_t tmpMode;
    if (vnode == NULL || attr == NULL) {
        return -EINVAL;
    }
    if (attr->attr_chg_valid & CHG_MODE) {
        tmpMode = attr->attr_chg_mode;
        tmpMode &= ~S_IFMT;
        vnode->mode &= S_IFMT;
        vnode->mode = tmpMode | vnode->mode;
    }
    if (attr->attr_chg_valid & CHG_UID) {
        vnode->uid = attr->attr_chg_uid;
    }
    if (attr->attr_chg_valid & CHG_GID) {
        vnode->gid = attr->attr_chg_gid;
    }
    return LOS_OK;
}

int VnodeDevLookup(struct Vnode *parentVnode, const char *path, int len, struct Vnode **vnode)
{
    (void)parentVnode;
    (void)path;
    (void)len;
    (void)vnode;
    /* dev node must in pathCache. */
    return -ENOENT;
}
//虚拟设备 默认节点操作
static struct VnodeOps g_devfsOps = {
    .Lookup = VnodeDevLookup,
    .Getattr = VnodeGetattr,
    .Readdir = VnodeReaddir,
    .Opendir = VnodeOpendir,
    .Closedir = VnodeClosedir,
    .Create = VnodeCreate,
    .Chattr = VnodeChattr,
};

void VnodeMemoryDump(void)
{
    struct Vnode *item = NULL;
    struct Vnode *nextItem = NULL;
    int vnodeCount = 0;

    LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, nextItem, &g_vnodeCurrList, struct Vnode, actFreeEntry) {
        if ((item->useCount > 0) ||
            (item->flag & VNODE_FLAG_MOUNT_ORIGIN) ||
            (item->flag & VNODE_FLAG_MOUNT_NEW)) {
            continue;
        }

        vnodeCount++;
    }

    PRINTK("Vnode number = %d\n", vnodeCount);
    PRINTK("Vnode memory size = %d(B)\n", vnodeCount * sizeof(struct Vnode));
}

int VnodeDestory(struct Vnode *vnode)
{
    if (vnode == NULL || vnode->vop != &g_devfsOps) {
        /* destory only support dev vnode */
        return -EINVAL;
    }

    VnodeHold();
    if (vnode->useCount > 0) {
        VnodeDrop();
        return -EBUSY;
    }

    VnodePathCacheFree(vnode);
    LOS_ListDelete(&(vnode->hashEntry));
    LOS_ListDelete(&vnode->actFreeEntry);

    free(vnode->data);
    free(vnode);
    g_totalVnodeSize--;
    VnodeDrop();

    return LOS_OK;
}
