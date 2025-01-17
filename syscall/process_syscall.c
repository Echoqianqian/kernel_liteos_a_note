/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
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

#include "los_process_pri.h"
#include "los_task_pri.h"
#include "los_sched_pri.h"
#include "los_hw_pri.h"
#include "los_sys_pri.h"
#include "los_futex_pri.h"
#include "los_mp.h"
#include "sys/wait.h"
#include "user_copy.h"
#include "time.h"
#ifdef LOSCFG_SECURITY_CAPABILITY
#include "capability_api.h"
#endif

//进程相关系统回调
//检查进程权限
static int OsPermissionToCheck(unsigned int pid, unsigned int who)
{
    int ret = LOS_GetProcessGroupID(pid);//获取进程组ID
    if (ret < 0) {
        return ret;
    } else if (ret == OS_KERNEL_PROCESS_GROUP) {//为内核进程组
        return -EPERM;
    } else if ((ret == OS_USER_PRIVILEGE_PROCESS_GROUP) && (pid != who)) {//为用户进程组,但两个参数进程不一致
        return -EPERM;
    } else if (pid == OsGetUserInitProcessID()) {//为用户进程的祖宗
        return -EPERM;
    }

    return 0;
}
///设置用户级任务调度信息
static int OsUserTaskSchedulerSet(unsigned int tid, unsigned short policy, unsigned short priority, bool policyFlag)
{
    int ret;
    unsigned int intSave;
    bool needSched = false;

    if (OS_TID_CHECK_INVALID(tid)) {
        return EINVAL;
    }

    if (priority > OS_TASK_PRIORITY_LOWEST) {
        return EINVAL;
    }

    if ((policy != LOS_SCHED_FIFO) && (policy != LOS_SCHED_RR)) {
        return EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    LosTaskCB *taskCB = OS_TCB_FROM_TID(tid);
    ret = OsUserTaskOperatePermissionsCheck(taskCB);
    if (ret != LOS_OK) {
        SCHEDULER_UNLOCK(intSave);
        return ret;
    }

    policy = (policyFlag == true) ? policy : taskCB->policy;
    needSched = OsSchedModifyTaskSchedParam(taskCB, policy, priority);
    SCHEDULER_UNLOCK(intSave);

    LOS_MpSchedule(OS_MP_CPU_ALL);
    if (needSched && OS_SCHEDULER_ACTIVE) {
        LOS_Schedule();
    }

    return LOS_OK;
}

void SysSchedYield(int type)
{
    (void)LOS_TaskYield();
    return;
}

int SysSchedGetScheduler(int id, int flag)
{
    LosTaskCB *taskCB = NULL;
    unsigned int intSave;
    int policy;
    int ret;

    if (flag < 0) {
        if (OS_TID_CHECK_INVALID(id)) {
            return -EINVAL;
        }

        SCHEDULER_LOCK(intSave);
        taskCB = OS_TCB_FROM_TID(id);
        ret = OsUserTaskOperatePermissionsCheck(taskCB);
        if (ret != LOS_OK) {
            SCHEDULER_UNLOCK(intSave);
            return -ret;
        }

        policy = taskCB->policy;
        SCHEDULER_UNLOCK(intSave);
        return policy;
    }

    return LOS_GetProcessScheduler(id);
}

int SysSchedSetScheduler(int id, int policy, int prio, int flag)
{
    int ret;

    if (flag < 0) {
        return -OsUserTaskSchedulerSet(id, policy, prio, true);
    }

    if (prio < OS_USER_PROCESS_PRIORITY_HIGHEST) {
        return -EINVAL;
    }

    if (id == 0) {
        id = (int)LOS_GetCurrProcessID();
    }

    ret = OsPermissionToCheck(id, LOS_GetCurrProcessID());
    if (ret < 0) {
        return ret;
    }

    return OsSetProcessScheduler(LOS_PRIO_PROCESS, id, prio, policy);
}

int SysSchedGetParam(int id, int flag)
{
    LosTaskCB *taskCB = NULL;
    int pri;
    unsigned int intSave;

    if (flag < 0) {
        if (OS_TID_CHECK_INVALID(id)) {
            return -EINVAL;
        }

        SCHEDULER_LOCK(intSave);
        taskCB = OS_TCB_FROM_TID(id);
        pri = OsUserTaskOperatePermissionsCheck(taskCB);
        if (pri != LOS_OK) {
            SCHEDULER_UNLOCK(intSave);
            return -pri;
        }

        pri = taskCB->priority;
        SCHEDULER_UNLOCK(intSave);
        return pri;
    }

    if (id == 0) {
        id = (int)LOS_GetCurrProcessID();
    }

    if (OS_PID_CHECK_INVALID(id)) {
        return -EINVAL;
    }

    return OsGetProcessPriority(LOS_PRIO_PROCESS, id);
}
///设置进程优先级
int SysSetProcessPriority(int which, int who, unsigned int prio)
{
    int ret;

    if (prio < OS_USER_PROCESS_PRIORITY_HIGHEST) {
        return -EINVAL;
    }

    if (who == 0) {
        who = (int)LOS_GetCurrProcessID();
    }

    ret = OsPermissionToCheck(who, LOS_GetCurrProcessID());
    if (ret < 0) {
        return ret;
    }

    return OsSetProcessScheduler(which, who, prio, LOS_GetProcessScheduler(who));
}

int SysSchedSetParam(int id, unsigned int prio, int flag)
{
    if (flag < 0) {
        return -OsUserTaskSchedulerSet(id, LOS_SCHED_RR, prio, false);
    }

    return SysSetProcessPriority(LOS_PRIO_PROCESS, id, prio);
}

int SysGetProcessPriority(int which, int who)
{
    if (who == 0) {
        who = (int)LOS_GetCurrProcessID();
    }

    return OsGetProcessPriority(which, who);
}

int SysSchedGetPriorityMin(int policy)
{
    if (policy != LOS_SCHED_RR) {
        return -EINVAL;
    }

    return OS_USER_PROCESS_PRIORITY_HIGHEST;
}

int SysSchedGetPriorityMax(int policy)
{
    if (policy != LOS_SCHED_RR) {
        return -EINVAL;
    }

    return OS_USER_PROCESS_PRIORITY_LOWEST;
}

int SysSchedRRGetInterval(int pid, struct timespec *tp)
{
    unsigned int intSave;
    int ret;
    time_t timeSlice = 0;
    struct timespec tv;
    LosTaskCB *taskCB = NULL;
    LosProcessCB *processCB = NULL;

    if (tp == NULL) {
        return -EINVAL;
    }

    if (OS_PID_CHECK_INVALID(pid)) {
        return -EINVAL;
    }

    if (pid == 0) {
        processCB = OsCurrProcessGet();
    } else {
        processCB = OS_PCB_FROM_PID(pid);
    }

    SCHEDULER_LOCK(intSave);
    /* if can not find process by pid return ESRCH */
    if (OsProcessIsInactive(processCB)) {
        SCHEDULER_UNLOCK(intSave);
        return -ESRCH;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(taskCB, &processCB->threadSiblingList, LosTaskCB, threadList) {
        if (!OsTaskIsInactive(taskCB) && (taskCB->policy == LOS_SCHED_RR)) {
            timeSlice += taskCB->initTimeSlice;
        }
    }

    SCHEDULER_UNLOCK(intSave);

    timeSlice = timeSlice * OS_NS_PER_CYCLE;
    tv.tv_sec = timeSlice / OS_SYS_NS_PER_SECOND;
    tv.tv_nsec = timeSlice % OS_SYS_NS_PER_SECOND;
    ret = LOS_ArchCopyToUser(tp, &tv, sizeof(struct timespec));
    if (ret != 0) {
        return -EFAULT;
    }

    return 0;
}

int SysWait(int pid, USER int *status, int options, void *rusage)
{
    (void)rusage;

    return LOS_Wait(pid, status, (unsigned int)options, NULL);
}

int SysWaitid(idtype_t type, int pid, USER siginfo_t *info, int options, void *rusage)
{
    (void)rusage;
    int ret;
    int truepid = 0;

    switch (type) {
        case P_ALL:
            /* Wait for any child; id is ignored. */
            truepid = -1;
            break;
        case P_PID:
            /* Wait for the child whose process ID matches id */
            if (pid <= 0) {
                return -EINVAL;
            }
            truepid = pid;
            break;
        case P_PGID:
            /* Wait for any child whose process group ID matches id */
            if (pid <= 1) {
                return -EINVAL;
            }
            truepid = -pid;
            break;
        default:
            return -EINVAL;
    }

    ret = LOS_Waitid(truepid, info, (unsigned int)options, NULL);
    if (ret > 0) {
        ret = 0;
    }
    return ret;
}

int SysFork(void)
{
    return OsClone(0, 0, 0);
}

int SysVfork(void)
{
    return OsClone(CLONE_VFORK, 0, 0);
}

unsigned int SysGetPPID(void)
{
    return OsCurrProcessGet()->parentProcessID;
}

unsigned int SysGetPID(void)
{
    return LOS_GetCurrProcessID();
}
/// 为指定进程设置进程组ID
int SysSetProcessGroupID(unsigned int pid, unsigned int gid)
{
    int ret;

    if (pid == 0) {//无指定进程ID时 @note_thinking 此处会不会有风险, 直接返回会不会好些 ?
        pid = LOS_GetCurrProcessID();//获取当前进程ID,给当前进程设置组ID
    }

    if (gid == 0) {
        gid = pid;
    } else if (gid <= OS_USER_PRIVILEGE_PROCESS_GROUP) {
        return -EPERM;
    }

    ret = OsPermissionToCheck(pid, gid);
    if (ret < 0) {
        return ret;
    }

    return OsSetProcessGroupID(pid, gid);
}
/// 获取指定进程的组ID,为0时返回当前进程ID
int SysGetProcessGroupID(unsigned int pid)
{
    if (pid == 0) {
        pid = LOS_GetCurrProcessID();
    }

    return LOS_GetProcessGroupID(pid);
}
/// 获取当前进程组ID
int SysGetCurrProcessGroupID(void)
{
    return LOS_GetCurrProcessGroupID();
}
/// 获取用户ID
int SysGetUserID(void)
{
    return LOS_GetUserID();
}

int SysGetEffUserID(void)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    UINT32 intSave;
    int euid;

    SCHEDULER_LOCK(intSave);
    euid = (int)OsCurrUserGet()->effUserID;
    SCHEDULER_UNLOCK(intSave);
    return euid;
#else
    return 0;
#endif
}

int SysGetEffGID(void)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    UINT32 intSave;
    int egid;

    SCHEDULER_LOCK(intSave);
    egid = (int)OsCurrUserGet()->effGid;
    SCHEDULER_UNLOCK(intSave);
    return egid;
#else
    return 0;
#endif
}

int SysGetRealEffSaveUserID(int *ruid, int *euid, int *suid)
{
    int ret;
    int realUserID, effUserID, saveUserID;
#ifdef LOSCFG_SECURITY_CAPABILITY
    unsigned int intSave;

    SCHEDULER_LOCK(intSave);
    realUserID = OsCurrUserGet()->userID;
    effUserID = OsCurrUserGet()->effUserID;
    saveUserID = OsCurrUserGet()->effUserID;
    SCHEDULER_UNLOCK(intSave);
#else
    realUserID = 0;
    effUserID = 0;
    saveUserID = 0;
#endif

    ret = LOS_ArchCopyToUser(ruid, &realUserID, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    ret = LOS_ArchCopyToUser(euid, &effUserID, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    ret = LOS_ArchCopyToUser(suid, &saveUserID, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    return 0;
}

int SysSetUserID(int uid)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    int ret = -EPERM;
    unsigned int intSave;

    if (uid < 0) {
        return -EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    User *user = OsCurrUserGet();
    if (IsCapPermit(CAP_SETUID)) {
        user->userID = uid;
        user->effUserID = uid;
        /* add process to a user */
    } else if (user->userID != uid) {
        goto EXIT;
    }

    ret = LOS_OK;
    /* add process to a user */
EXIT:
    SCHEDULER_UNLOCK(intSave);
    return ret;
#else
    if (uid != 0) {
        return -EPERM;
    }

    return 0;
#endif
}

#ifdef LOSCFG_SECURITY_CAPABILITY
static int SetRealEffSaveUserIDCheck(int ruid, int euid, int suid)
{
    if ((ruid < 0) && (ruid != -1)) {
        return -EINVAL;
    }

    if ((euid < 0) && (euid != -1)) {
        return -EINVAL;
    }

    if ((suid < 0) && (suid != -1)) {
        return -EINVAL;
    }

    return 0;
}
#endif

int SysSetRealEffSaveUserID(int ruid, int euid, int suid)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    int ret;

    if ((ruid == -1) && (euid == -1) && (suid == -1)) {
        return 0;
    }

    ret = SetRealEffSaveUserIDCheck(ruid, euid, suid);
    if (ret != 0) {
        return ret;
    }

    if (ruid >= 0) {
        if (((euid != -1) && (euid != ruid)) || ((suid != -1) && (suid != ruid))) {
            return -EPERM;
        }
        return SysSetUserID(ruid);
    } else if (euid >= 0) {
        if ((suid != -1) && (suid != euid)) {
            return -EPERM;
        }
        return SysSetUserID(euid);
    } else {
        return SysSetUserID(suid);
    }
#else
    if ((ruid != 0) || (euid != 0) || (suid != 0)) {
        return -EPERM;
    }
    return 0;
#endif
}

int SysSetRealEffUserID(int ruid, int euid)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    return SysSetRealEffSaveUserID(ruid, euid, -1);
#else
    if ((ruid != 0) || (euid != 0)) {
        return -EPERM;
    }
    return 0;
#endif
}

int SysSetGroupID(int gid)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    int ret = -EPERM;
    unsigned int intSave;
    unsigned int count;
    unsigned int oldGid;
    User *user = NULL;

    if (gid < 0) {
        return -EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    user = OsCurrUserGet();
    if (IsCapPermit(CAP_SETGID)) {
        oldGid = user->gid;
        user->gid = gid;
        user->effGid = gid;
        for (count = 0; count < user->groupNumber; count++) {
            if (user->groups[count] == oldGid) {
                user->groups[count] = gid;
                ret = LOS_OK;
                goto EXIT;
            }
        }
    } else if (user->gid != gid) {
        goto EXIT;
    }

    ret = LOS_OK;
    /* add process to a user */
EXIT:
    SCHEDULER_UNLOCK(intSave);
    return ret;

#else
    if (gid != 0) {
        return -EPERM;
    }

    return 0;
#endif
}

int SysGetRealEffSaveGroupID(int *rgid, int *egid, int *sgid)
{
    int ret;
    int realGroupID, effGroupID, saveGroupID;
#ifdef LOSCFG_SECURITY_CAPABILITY
    unsigned int intSave;

    SCHEDULER_LOCK(intSave);
    realGroupID = OsCurrUserGet()->gid;
    effGroupID = OsCurrUserGet()->effGid;
    saveGroupID = OsCurrUserGet()->effGid;
    SCHEDULER_UNLOCK(intSave);
#else
    realGroupID = 0;
    effGroupID = 0;
    saveGroupID = 0;
#endif

    ret = LOS_ArchCopyToUser(rgid, &realGroupID, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    ret = LOS_ArchCopyToUser(egid, &effGroupID, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    ret = LOS_ArchCopyToUser(sgid, &saveGroupID, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    return 0;
}

#ifdef LOSCFG_SECURITY_CAPABILITY
static int SetRealEffSaveGroupIDCheck(int rgid, int egid, int sgid)
{
    if ((rgid < 0) && (rgid != -1)) {
        return -EINVAL;
    }

    if ((egid < 0) && (egid != -1)) {
        return -EINVAL;
    }

    if ((sgid < 0) && (sgid != -1)) {
        return -EINVAL;
    }

    return 0;
}
#endif

int SysSetRealEffSaveGroupID(int rgid, int egid, int sgid)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    int ret;

    if ((rgid == -1) && (egid == -1) && (sgid == -1)) {
        return 0;
    }

    ret = SetRealEffSaveGroupIDCheck(rgid, egid, sgid);
    if (ret != 0) {
        return ret;
    }

    if (rgid >= 0) {
        if (((egid != -1) && (egid != rgid)) || ((sgid != -1) && (sgid != rgid))) {
            return -EPERM;
        }
        return SysSetGroupID(rgid);
    } else if (egid >= 0) {
        if ((sgid != -1) && (sgid != egid)) {
            return -EPERM;
        }
        return SysSetGroupID(egid);
    } else {
        return SysSetGroupID(sgid);
    }

#else
    if ((rgid != 0) || (egid != 0) || (sgid != 0)) {
        return -EPERM;
    }
    return 0;
#endif
}

int SysSetRealEffGroupID(int rgid, int egid)
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    return SysSetRealEffSaveGroupID(rgid, egid, -1);
#else
    if ((rgid != 0) || (egid != 0)) {
        return -EPERM;
    }
    return 0;
#endif
}

int SysGetGroupID(void)
{
    return LOS_GetGroupID();
}

#ifdef LOSCFG_SECURITY_CAPABILITY
static int SetGroups(int listSize, const int *safeList, int size)
{
    User *oldUser = NULL;
    unsigned int intSave;

    User *newUser = LOS_MemAlloc(m_aucSysMem1, sizeof(User) + listSize * sizeof(int));
    if (newUser == NULL) {
        return -ENOMEM;
    }

    SCHEDULER_LOCK(intSave);
    oldUser = OsCurrUserGet();
    (VOID)memcpy_s(newUser, sizeof(User), oldUser, sizeof(User));
    if (safeList != NULL) {
        (VOID)memcpy_s(newUser->groups, size * sizeof(int), safeList, size * sizeof(int));
    }
    if (listSize == size) {
        newUser->groups[listSize] = oldUser->gid;
    }

    newUser->groupNumber = listSize + 1;
    OsCurrProcessGet()->user = newUser;
    SCHEDULER_UNLOCK(intSave);

    (void)LOS_MemFree(m_aucSysMem1, oldUser);
    return 0;
}

static int GetGroups(int size, int list[])
{
    unsigned int intSave;
    int groupCount;
    int ret;
    int *safeList = NULL;
    unsigned int listSize;

    SCHEDULER_LOCK(intSave);
    groupCount = OsCurrUserGet()->groupNumber;
    SCHEDULER_UNLOCK(intSave);

    listSize = groupCount * sizeof(int);
    if (size == 0) {
        return groupCount;
    } else if (list == NULL) {
        return -EFAULT;
    } else if (size < groupCount) {
        return -EINVAL;
    }

    safeList = LOS_MemAlloc(m_aucSysMem1, listSize);
    if (safeList == NULL) {
        return -ENOMEM;
    }

    SCHEDULER_LOCK(intSave);
    (void)memcpy_s(safeList, listSize, &OsCurrProcessGet()->user->groups[0], listSize);
    SCHEDULER_UNLOCK(intSave);

    ret = LOS_ArchCopyToUser(list, safeList, listSize);
    if (ret != 0) {
        groupCount = -EFAULT;
    }

    (void)LOS_MemFree(m_aucSysMem1, safeList);
    return groupCount;
}
#endif

int SysGetGroups(int size, int list[])
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    return GetGroups(size, list);
#else
    int group = 0;
    int groupCount = 1;
    int ret;

    if (size == 0) {
        return groupCount;
    } else if (list == NULL) {
        return -EFAULT;
    } else if (size < groupCount) {
        return -EINVAL;
    }

    ret = LOS_ArchCopyToUser(list, &group, sizeof(int));
    if (ret != 0) {
        return -EFAULT;
    }

    return groupCount;
#endif
}

int SysSetGroups(int size, const int list[])
{
#ifdef LOSCFG_SECURITY_CAPABILITY
    int ret;
    int gid;
    int listSize = size;
    unsigned int count;
    int *safeList = NULL;
#endif

    if ((size != 0) && (list == NULL)) {
        return -EFAULT;
    }

    if ((size < 0) || (size > OS_GROUPS_NUMBER_MAX)) {
        return -EINVAL;
    }

#ifdef LOSCFG_SECURITY_CAPABILITY
    if (!IsCapPermit(CAP_SETGID)) {
        return -EPERM;
    }

    if (size != 0) {
        safeList = LOS_MemAlloc(m_aucSysMem1, size * sizeof(int));
        if (safeList == NULL) {
            return -ENOMEM;
        }

        ret = LOS_ArchCopyFromUser(safeList, list, size * sizeof(int));
        if (ret != 0) {
            ret = -EFAULT;
            goto EXIT;
        }
        gid = OsCurrUserGet()->gid;
        for (count = 0; count < size; count++) {
            if (safeList[count] == gid) {
                listSize = size - 1;
            } else if (safeList[count] < 0) {
                ret = -EINVAL;
                goto EXIT;
            }
        }
    }

    ret = SetGroups(listSize, safeList, size);
EXIT:
    if (safeList != NULL) {
        (void)LOS_MemFree(m_aucSysMem1, safeList);
    }

    return ret;
#else
    return 0;
#endif
}

unsigned int SysCreateUserThread(const TSK_ENTRY_FUNC func, const UserTaskParam *userParam, bool joinable)
{
    TSK_INIT_PARAM_S param = { 0 };
    int ret;

    ret = LOS_ArchCopyFromUser(&(param.userParam), userParam, sizeof(UserTaskParam));
    if (ret != 0) {
        return OS_INVALID_VALUE;
    }

    param.pfnTaskEntry = func;
    if (joinable == TRUE) {
        param.uwResved = LOS_TASK_ATTR_JOINABLE;
    } else {
        param.uwResved = LOS_TASK_STATUS_DETACHED;
    }

    return OsCreateUserTask(OS_INVALID_VALUE, &param);
}

int SysSetThreadArea(const char *area)
{
    unsigned int intSave;
    LosTaskCB *taskCB = NULL;
    LosProcessCB *processCB = NULL;
    unsigned int ret = LOS_OK;

    if (!LOS_IsUserAddress((unsigned long)(uintptr_t)area)) {
        return EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    taskCB = OsCurrTaskGet();
    processCB = OS_PCB_FROM_PID(taskCB->processID);
    if (processCB->processMode != OS_USER_MODE) {
        ret = EPERM;
        goto OUT;
    }

    taskCB->userArea = (unsigned long)(uintptr_t)area;
OUT:
    SCHEDULER_UNLOCK(intSave);
    return ret;
}

char *SysGetThreadArea(void)
{
    return (char *)(OsCurrTaskGet()->userArea);
}

int SysUserThreadSetDetach(unsigned int taskID)
{
    unsigned int intSave;
    int ret;
    LosTaskCB *taskCB = NULL;

    if (OS_TID_CHECK_INVALID(taskID)) {
        return EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    taskCB = OS_TCB_FROM_TID(taskID);
    ret = OsUserTaskOperatePermissionsCheck(taskCB);
    if (ret != LOS_OK) {
        goto EXIT;
    }

    ret = OsTaskSetDetachUnsafe(taskCB);

EXIT:
    SCHEDULER_UNLOCK(intSave);
    return ret;
}

int SysUserThreadDetach(unsigned int taskID)
{
    unsigned int intSave;
    LosTaskCB *taskCB = NULL;
    unsigned int ret;

    if (OS_TID_CHECK_INVALID(taskID)) {
        return EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    taskCB = OS_TCB_FROM_TID(taskID);
    ret = OsUserTaskOperatePermissionsCheck(taskCB);
    if (ret != LOS_OK) {
        SCHEDULER_UNLOCK(intSave);
        return ret;
    }

    ret = OsTaskDeleteUnsafe(taskCB, OS_PRO_EXIT_OK, intSave);
    if (ret != LOS_OK) {
        return ESRCH;
    }

    return LOS_OK;
}

int SysThreadJoin(unsigned int taskID)
{
    unsigned int intSave;
    unsigned int ret;
    LosTaskCB *taskCB = NULL;

    if (OS_TID_CHECK_INVALID(taskID)) {
        return EINVAL;
    }

    SCHEDULER_LOCK(intSave);
    taskCB = OS_TCB_FROM_TID(taskID);
    ret = OsUserTaskOperatePermissionsCheck(taskCB);
    if (ret != LOS_OK) {
        goto EXIT;
    }

    ret = OsTaskJoinPendUnsafe(OS_TCB_FROM_TID(taskID));

EXIT:
    SCHEDULER_UNLOCK(intSave);
    return ret;
}

void SysUserExitGroup(int status)
{
    OsTaskExitGroup((unsigned int)status);
}
/// 系统调用线程退出
void SysThreadExit(int status)
{
    OsTaskToExit(OsCurrTaskGet(), (unsigned int)status);
}

/*!
 * @brief SysFutex	操作用户态快速互斥锁
 * 系统调用
 * @param absTime	绝对时间
 * @param flags	操作标识
 * @param newUserAddr FUTEX_REQUEUE下调整后带回新的用户空间地址	
 * @param uAddr	用户空间地址 
 * @param val 	
 * @return	
 *
 * @see
 */
int SysFutex(const unsigned int *uAddr, unsigned int flags, int val,
             unsigned int absTime, const unsigned int *newUserAddr)
{
    if ((flags & FUTEX_MASK) == FUTEX_REQUEUE) {//调整标识
        return -OsFutexRequeue(uAddr, flags, val, absTime, newUserAddr);
    }

    if ((flags & FUTEX_MASK) == FUTEX_WAKE) {//唤醒标识
        return -OsFutexWake(uAddr, flags, val);
    }

    return -OsFutexWait(uAddr, flags, val, absTime);//等待标识
}

unsigned int SysGetTid(void)
{
    return OsCurrTaskGet()->taskID;
}

/* If flag >= 0, the process mode is used. If flag < 0, the thread mode is used. */
static int SchedAffinityParameterPreprocess(int id, int flag, unsigned int *taskID, unsigned int *processID)
{
    if (flag >= 0) {
        if (OS_PID_CHECK_INVALID(id)) {
            return -ESRCH;
        }
        *taskID = (id == 0) ? (OsCurrTaskGet()->taskID) : (OS_PCB_FROM_PID((UINT32)id)->threadGroupID);
        *processID = (id == 0) ? (OS_TCB_FROM_TID(*taskID)->processID) : id;
    } else {
        if (OS_TID_CHECK_INVALID(id)) {
            return -ESRCH;
        }
        *taskID = id;
        *processID = OS_INVALID_VALUE;
    }
    return LOS_OK;
}

/* If flag >= 0, the process mode is used. If flag < 0, the thread mode is used. */
int SysSchedGetAffinity(int id, unsigned int *cpuset, int flag)
{
    int ret;
    unsigned int processID;
    unsigned int taskID;
    unsigned int intSave;
    unsigned int cpuAffiMask;

    ret = SchedAffinityParameterPreprocess(id, flag, &taskID, &processID);
    if (ret != LOS_OK) {
        return ret;
    }

    SCHEDULER_LOCK(intSave);
    if (flag >= 0) {
        if (OsProcessIsInactive(OS_PCB_FROM_PID(processID))) {
            SCHEDULER_UNLOCK(intSave);
            return -ESRCH;
        }
    } else {
        ret = OsUserTaskOperatePermissionsCheck(OS_TCB_FROM_TID(taskID));
        if (ret != LOS_OK) {
            SCHEDULER_UNLOCK(intSave);
            if (ret == EINVAL) {
                return -ESRCH;
            }
            return -ret;
        }
    }

#ifdef LOSCFG_KERNEL_SMP
    cpuAffiMask = (unsigned int)OS_TCB_FROM_TID(taskID)->cpuAffiMask;
#else
    cpuAffiMask = 1;
#endif /* LOSCFG_KERNEL_SMP */

    SCHEDULER_UNLOCK(intSave);
    ret = LOS_ArchCopyToUser(cpuset, &cpuAffiMask, sizeof(unsigned int));
    if (ret != LOS_OK) {
        return -EFAULT;
    }

    return LOS_OK;
}

/* If flag >= 0, the process mode is used. If flag < 0, the thread mode is used. */
int SysSchedSetAffinity(int id, const unsigned short cpuset, int flag)
{
    int ret;
    unsigned int processID;
    unsigned int taskID;
    unsigned int intSave;
    unsigned short currCpuMask;
    bool needSched = FALSE;

    if (cpuset > LOSCFG_KERNEL_CPU_MASK) {
        return -EINVAL;
    }

    ret = SchedAffinityParameterPreprocess(id, flag, &taskID, &processID);
    if (ret != LOS_OK) {
        return ret;
    }

    if (flag >= 0) {
        ret = OsPermissionToCheck(processID, LOS_GetCurrProcessID());
        if (ret != LOS_OK) {
            return ret;
        }
        SCHEDULER_LOCK(intSave);
        if (OsProcessIsInactive(OS_PCB_FROM_PID(processID))) {
            SCHEDULER_UNLOCK(intSave);
            return -ESRCH;
        }
    } else {
        SCHEDULER_LOCK(intSave);
        ret = OsUserTaskOperatePermissionsCheck(OS_TCB_FROM_TID(taskID));
        if (ret != LOS_OK) {
            SCHEDULER_UNLOCK(intSave);
            if (ret == EINVAL) {
                return -ESRCH;
            }
            return -ret;
        }
    }

    needSched = OsTaskCpuAffiSetUnsafe(taskID, cpuset, &currCpuMask);
    SCHEDULER_UNLOCK(intSave);
    if (needSched && OS_SCHEDULER_ACTIVE) {
        LOS_MpSchedule(currCpuMask);
        LOS_Schedule();
    }

    return LOS_OK;
}

