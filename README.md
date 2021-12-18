子曰：“里仁为美。择不处仁，焉得知？”《论语》：里仁篇

[![](https://gitee.com/weharmonyos/resources/raw/master/common/io.png)](http://weharmonyos.com/)

百篇博客分析.本篇为: (源码注释篇) | 每天死磕一点点

**中文注解鸿蒙内核 | [kernel_liteos_a_note](https://gitee.com/weharmony/kernel_liteos_a_note)** 是在 `OpenHarmony` 的 [kernel_liteos_a](https://gitee.com/openharmony/kernel_liteos_a) 基础上给内核源码加上中文注解的版本.

### 为何要精读内核源码?
* 码农的学职生涯，都应精读一遍内核源码。以浇筑好计算机知识大厦的地基，地基纵深的坚固程度，很大程度能决定未来大厦能盖多高。那为何一定要精读细品呢?
* 因为内核代码本身并不太多，都是浓缩的精华，精读是让各个知识点高频出现，不孤立成点状记忆，没有足够连接点的知识点是很容易忘的，点点成线，线面成体，连接越多，记得越牢，如此短时间内容易结成一张高浓度，高密度的系统化知识网，训练大脑肌肉记忆，驻入大脑直觉区，想抹都抹不掉，终生携带，随时调取。跟骑单车一样，一旦学会，即便多年不骑，照样跨上就走，游刃有余。
### 热爱是所有的理由和答案
* 因大学时阅读 `linux 2.6` 内核痛并快乐的经历，一直有个心愿，如何让更多对内核感兴趣的朋友减少阅读时间，加速对计算机系统级的理解，而不至于过早的放弃。但因过程种种，多年一直没有行动，基本要放弃这件事了。恰逢 `2020/9/10` 鸿蒙正式开源，重新激活了多年的心愿，就有那么点如黄河之水一发不可收拾了。 
* 目前对内核源码的注解已完成了 `80%` ，对内核源码的博客分析已完成了`70+篇`， 已持续一年有余, 占用了所有的空闲时间, 但每天都很充实，很兴奋，连做梦内核代码都在鱼贯而入。如此疯狂地做一件事还是当年谈恋爱的时候， 只因热爱， 热爱是所有的理由和答案。 :P 
### (〃･ิ‿･ิ)ゞ鸿蒙内核开发者
* 感谢开放原子开源基金会，致敬鸿蒙内核开发者提供了如此优秀的源码，一了多年的夙愿，津津乐道于此。精读内核源码加注并整理成档是件很有挑战的事，时间上是以年为单位，但正因为很难才值得去做! 干困难事，方有所得;专注聚焦，必有所获。 
* 从内核一行行的代码中能深深感受到开发者各中艰辛与坚持，及鸿蒙生态对未来的价值，这些是张嘴就来的网络喷子们永远不能体会到的。可以毫不夸张的说鸿蒙内核源码可作为大学: C语言，数据结构，操作系统，汇编语言，计算机系统结构，计算机组成原理 六门课程的教学项目。如此宝库，不深入研究实在是暴殄天物，于心不忍，注者坚信鸿蒙大势所趋，未来可期，其必定成功，也必然成功，誓做其坚定的追随者和传播者。
  
### 理解内核的三个层级

* 普通概念映射级: 这一级不涉及专业知识，用大众所熟知的公共认知就能听明白是个什么概念，也就是说用一个普通人都懂的概念去诠释或者映射一个他们从没听过的概念。让陌生的知识点与大脑中烂熟于心的知识点建立多重链接，加深记忆。说别人能听得懂的话这很重要!!! 一个没学过计算机知识的卖菜大妈就不可能知道内核的基本运作了吗? 不一定!在系列篇中试图用故事篇,去引导这一层级的认知，希望能卷入更多的人来关注基础软件，人多了场子热起来了创新就来了。
* 专业概念抽象级: 对抽象的专业逻辑概念具体化认知， 比如虚拟内存，老百姓是听不懂的，学过计算机的人都懂，具体怎么实现的很多人又都不懂了，但这并不妨碍成为一个优秀的上层应用开发者，因为虚拟内存已经被抽象出来，目的是要屏蔽上层对它具体实现的认知。试图用百篇博客系列篇去拆解那些已经被抽象出来的专业概念， 希望能卷入更多对内核感兴趣的应用软件人才流入基础软硬件生态， 应用软件咱们是无敌宇宙，但基础软件却很薄弱。
* 具体微观代码级: 这一级是具体到每一行代码的实现，到了用代码指令级的地步，这段代码是什么意思?为什么要这么设计?有没有更好的方案? [鸿蒙内核源码注解分析](https://gitee.com/weharmony/kernel_liteos_a_note) 试图从细微处去解释代码实现层，英文真的是天生适合设计成编程语言的人类语言，计算机的01码映射到人类世界的26个字母，诞生了太多的伟大奇迹。但我们的母语注定了很大部分人存在着自然语言层级的理解映射，希望内核注解分析能让更多爱好者快速的理解内核，共同进步。

### 四个维度解剖内核
为了很好的全方位剖析内核,在`中文注释`,`官方文档`,`百篇博客`,`参考手册`四个方向做了努力,试图以`讲故事`,`画图表`,`写文档`,`拆源码`立体的方式表述清楚内核.很喜欢易中天老师的一句话:研究方式不等于表述方式。底层技术并不枯燥,它可以就是我们生活中的场景。
#### 一 : 中文注释 | 源码呈现

*  因鸿蒙内核6W+代码量，本身只有较少的注释， 中文注解以不对原有代码侵入为前提，源码中所有英文部分都是原有注释，所有中文部分都是中文版的注释，同时为方便同步官方版本的更新，尽量不去增加代码的行数，不破坏文件的结构，注释多类似以下的方式:

    在重要模块的.c/.h文件开始位置先对模块功能做整体的介绍，例如异常接管模块注解如图所示:

    ![](https://gitee.com/weharmonyos/resources/raw/master/13/ycjg.png)

    注解过程中查阅了很多的资料和书籍，在具体代码处都附上了参考链接。

* 而函数级注解会详细到重点行，甚至每一行， 例如申请互斥锁的主体函数，不可谓不重要，而官方注释仅有一行，如图所示

    ![](https://gitee.com/weharmonyos/resources/raw/master/13/sop.png)

* 另外画了一些字符图方便理解，直接嵌入到头文件中，比如虚拟内存的全景图，因没有这些图是很难理解虚拟内存是如何管理的。

    ![](https://gitee.com/weharmonyos/resources/raw/master/13/vm.png)

* 注解创建了一些特殊记号,可直接搜索查看
  - [x] 搜索 `@note_pic` 可查看绘制的全部字符图
  - [x] 搜索 `@note_why` 是尚未看明白的地方，有看明白的，请[新建 Pull Request](https://gitee.com/weharmony/kernel_liteos_a_note/pull/new/weharmony:master...weharmony:master)完善
  - [x] 搜索 `@note_thinking` 是一些的思考和建议
  - [x] 搜索 `@note_#if0` 是由第三方项目提供不在内核源码中定义的极为重要结构体，为方便理解而添加的。
  - [x] 搜索 `@note_link` 是网址链接,方便理解模块信息,来源于官方文档,百篇博客,外部链接
  - [x] 搜索 `@note_good` 是给源码点赞的地方
  
#### 二 : 官方文档 | 静态站点呈现

研究鸿蒙需不断的翻阅资料,吸取别人的精华,其中官方文档必不可少, 为更好的呈现 **OpenHarmony开发者文档** , 特意做了静态站点 [ >> 鸿蒙研究站 | 官方文档](http://weharmonyos.com/openharmony) 来方便搜索,阅读官方资料.

![](https://gitee.com/weharmonyos/resources/raw/master/52/2.png)

左侧导航栏,右边索引区

![](https://gitee.com/weharmonyos/resources/raw/master/52/4.png)


 [鸿蒙研究站](http://weharmonyos.com) 定位于做一个专注而靠谱的技术站, 没有广告,干净简洁,永久免费, 对鸿蒙研究会持续在上面输出。同时感谢资助鸿蒙研究和网站建设的小伙伴,很温暖。 [ >> 送温暖记录](http://weharmonyos.com/donate.html)


#### 三 : 百篇博客 | 文档呈现

* 给鸿蒙内核源码加注释过程中，整理出以下文章。内容立足源码，常以生活场景打比方尽可能多的将内核知识点置入某种场景，具有画面感，容易理解记忆。说别人能听得懂的话很重要! 百篇博客绝不是百度教条式的在说一堆诘屈聱牙的概念，那没什么意思。更希望让内核变得栩栩如生，倍感亲切.确实有难度，自不量力，但已经出发，回头已是不可能的了。　😛
* 与代码有bug需不断debug一样，文章和注解内容会存在不少错漏之处，请多包涵，但会反复修正，持续更新，v**.xx 代表文章序号和修改的次数，精雕细琢，言简意赅，力求打造精品内容。


按时间顺序:

* [v01.12 鸿蒙内核源码分析(双向链表) | 谁是内核最重要结构体](http://weharmonyos.com/blog/01.html)
* [v02.06 鸿蒙内核源码分析(进程管理) | 谁在管理内核资源](http://weharmonyos.com/blog/02.html)
* [v03.06 鸿蒙内核源码分析(时钟任务) | 调度的源动力从哪来](http://weharmonyos.com/blog/03.html)
* [v04.03 鸿蒙内核源码分析(任务调度) | 内核调度的单元是谁](http://weharmonyos.com/blog/04.html)
* [v05.05 鸿蒙内核源码分析(任务管理) | 如何管理任务池](http://weharmonyos.com/blog/05.html)
* [v06.03 鸿蒙内核源码分析(调度队列) | 内核调度也需要排队](http://weharmonyos.com/blog/06.html)
* [v07.08 鸿蒙内核源码分析(调度机制) | 任务是如何被调度执行的](http://weharmonyos.com/blog/07.html)
* [v08.03 鸿蒙内核源码分析(总目录) | 百万汉字注解 百篇博客分析](http://weharmonyos.com/blog/08.html)
* [v09.04 鸿蒙内核源码分析(调度故事) | 用故事说内核调度](http://weharmonyos.com/blog/09.html)
* [v10.03 鸿蒙内核源码分析(内存主奴) | 皇上和奴才如何相处](http://weharmonyos.com/blog/10.html)
* [v11.03 鸿蒙内核源码分析(内存分配) | 内存有哪些分配方式](http://weharmonyos.com/blog/11.html)
* [v12.04 鸿蒙内核源码分析(内存管理) | 虚拟内存全景图是怎样的](http://weharmonyos.com/blog/12.html)
* [v13.05 鸿蒙内核源码分析(源码注释) | 每天死磕一点点](http://weharmonyos.com/blog/13.html)
* [v14.02 鸿蒙内核源码分析(内存汇编) | 谁是虚拟内存实现的基础](http://weharmonyos.com/blog/14.html)
* [v15.03 鸿蒙内核源码分析(内存映射) | 映射真是个好东西](http://weharmonyos.com/blog/15.html)
* [v16.02 鸿蒙内核源码分析(内存规则) | 内存管理到底在管什么](http://weharmonyos.com/blog/16.html)
* [v17.04 鸿蒙内核源码分析(物理内存) | 怎么管理物理内存](http://weharmonyos.com/blog/17.html)
* [v18.02 鸿蒙内核源码分析(源码结构) | 内核文件各自含义](http://weharmonyos.com/blog/18.html)
* [v19.04 鸿蒙内核源码分析(位图管理) | 特节俭的苦命孩子](http://weharmonyos.com/blog/19.html)
* [v20.03 鸿蒙内核源码分析(用栈方式) | 谁来提供程序运行场地](http://weharmonyos.com/blog/20.html)
* [v21.07 鸿蒙内核源码分析(线程概念) | 是谁在不断的折腾CPU](http://weharmonyos.com/blog/21.html)
* [v22.03 鸿蒙内核源码分析(汇编基础) | CPU上班也要打卡](http://weharmonyos.com/blog/22.html)
* [v23.04 鸿蒙内核源码分析(汇编传参) | 如何传递复杂的参数](http://weharmonyos.com/blog/23.html)
* [v24.03 鸿蒙内核源码分析(进程概念) | 如何更好的理解进程](http://weharmonyos.com/blog/24.html)
* [v25.05 鸿蒙内核源码分析(并发并行) | 听过无数遍的两个概念](http://weharmonyos.com/blog/25.html)
* [v26.08 鸿蒙内核源码分析(自旋锁) | 当立贞节牌坊的好同志](http://weharmonyos.com/blog/26.html)
* [v27.05 鸿蒙内核源码分析(互斥锁) | 同样是锁它确更丰满](http://weharmonyos.com/blog/27.html)
* [v28.04 鸿蒙内核源码分析(进程通讯) | 九种进程间通讯方式速揽](http://weharmonyos.com/blog/28.html)
* [v29.05 鸿蒙内核源码分析(信号量) | 谁在解决任务间的同步](http://weharmonyos.com/blog/29.html)
* [v30.07 鸿蒙内核源码分析(事件控制) | 多对多任务如何同步](http://weharmonyos.com/blog/30.html)
* [v31.02 鸿蒙内核源码分析(定时器) | 内核最高级任务竟是它](http://weharmonyos.com/blog/31.html)
* [v32.03 鸿蒙内核源码分析(CPU) | 整个内核是一个死循环](http://weharmonyos.com/blog/32.html)
* [v33.03 鸿蒙内核源码分析(消息队列) | 进程间如何异步传递大数据](http://weharmonyos.com/blog/33.html)
* [v34.04 鸿蒙内核源码分析(原子操作) | 谁在为完整性保驾护航](http://weharmonyos.com/blog/34.html)
* [v35.03 鸿蒙内核源码分析(时间管理) | 内核基本时间单位是谁](http://weharmonyos.com/blog/35.html)
* [v36.05 鸿蒙内核源码分析(工作模式) | 程序界的韦小宝是谁](http://weharmonyos.com/blog/36.html)
* [v37.06 鸿蒙内核源码分析(系统调用) | 开发者永远的口头禅](http://weharmonyos.com/blog/37.html)
* [v38.06 鸿蒙内核源码分析(寄存器) | 讲真 全宇宙只佩服它](http://weharmonyos.com/blog/38.html)
* [v39.06 鸿蒙内核源码分析(异常接管) | 社会很单纯 复杂的是人](http://weharmonyos.com/blog/39.html)
* [v40.03 鸿蒙内核源码分析(汇编汇总) | 汇编可爱如邻家女孩](http://weharmonyos.com/blog/40.html)
* [v41.03 鸿蒙内核源码分析(任务切换) | 看汇编如何切换任务](http://weharmonyos.com/blog/41.html)
* [v42.05 鸿蒙内核源码分析(中断切换) | 系统因中断活力四射](http://weharmonyos.com/blog/42.html)
* [v43.05 鸿蒙内核源码分析(中断概念) | 海公公的日常工作](http://weharmonyos.com/blog/43.html)
* [v44.04 鸿蒙内核源码分析(中断管理) | 没中断太可怕](http://weharmonyos.com/blog/44.html)
* [v45.05 鸿蒙内核源码分析(Fork) | 一次调用 两次返回](http://weharmonyos.com/blog/45.html)
* [v46.05 鸿蒙内核源码分析(特殊进程) | 老鼠生儿会打洞](http://weharmonyos.com/blog/46.html)
* [v47.02 鸿蒙内核源码分析(进程回收) | 临终托孤的短命娃](http://weharmonyos.com/blog/47.html)
* [v48.05 鸿蒙内核源码分析(信号生产) | 年过半百 活力十足](http://weharmonyos.com/blog/48.html)
* [v49.03 鸿蒙内核源码分析(信号消费) | 谁让CPU连续四次换栈运行](http://weharmonyos.com/blog/49.html)
* [v50.03 鸿蒙内核源码分析(编译环境) | 编译鸿蒙防掉坑指南](http://weharmonyos.com/blog/50.html)
* [v51.04 鸿蒙内核源码分析(ELF格式) | 应用程序入口并非main](http://weharmonyos.com/blog/51.html)
* [v52.05 鸿蒙内核源码分析(静态站点) | 码农都不爱写注释和文档](http://weharmonyos.com/blog/52.html)
* [v53.03 鸿蒙内核源码分析(ELF解析) | 敢忘了她姐俩你就不是银](http://weharmonyos.com/blog/53.html)
* [v54.04 鸿蒙内核源码分析(静态链接) | 一个小项目看中间过程](http://weharmonyos.com/blog/54.html)
* [v55.04 鸿蒙内核源码分析(重定位) | 与国际接轨的对外发言人](http://weharmonyos.com/blog/55.html)
* [v56.05 鸿蒙内核源码分析(进程映像) | 程序是如何被加载运行的](http://weharmonyos.com/blog/56.html)
* [v57.02 鸿蒙内核源码分析(编译过程) | 简单案例说透中间过程](http://weharmonyos.com/blog/57.html)
* [v58.03 鸿蒙内核源码分析(环境脚本) | 编译鸿蒙原来很简单](http://weharmonyos.com/blog/58.html)
* [v59.04 鸿蒙内核源码分析(构建工具) | 顺瓜摸藤调试构建过程](http://weharmonyos.com/blog/59.html)
* [v60.04 鸿蒙内核源码分析(gn应用) | 如何构建鸿蒙系统](http://weharmonyos.com/blog/60.html)
* [v61.03 鸿蒙内核源码分析(忍者ninja) | 忍者的特点就是一个字](http://weharmonyos.com/blog/61.html)
* [v62.02 鸿蒙内核源码分析(文件概念) | 为什么说一切皆是文件](http://weharmonyos.com/blog/62.html)
* [v63.04 鸿蒙内核源码分析(文件系统) | 用图书管理说文件系统](http://weharmonyos.com/blog/63.html)
* [v64.06 鸿蒙内核源码分析(索引节点) | 谁是文件系统最重要的概念](http://weharmonyos.com/blog/64.html)
* [v65.05 鸿蒙内核源码分析(挂载目录) | 为何文件系统需要挂载](http://weharmonyos.com/blog/65.html)
* [v66.07 鸿蒙内核源码分析(根文件系统) | 谁先挂到/谁就是根总](http://weharmonyos.com/blog/66.html)
* [v67.03 鸿蒙内核源码分析(字符设备) | 绝大多数设备都是这类](http://weharmonyos.com/blog/67.html)
* [v68.02 鸿蒙内核源码分析(VFS) | 文件系统是个大家庭](http://weharmonyos.com/blog/68.html)
* [v69.04 鸿蒙内核源码分析(文件句柄) | 你为什么叫句柄](http://weharmonyos.com/blog/69.html)
* [v70.05 鸿蒙内核源码分析(管道文件) | 如何降低数据流动成本](http://weharmonyos.com/blog/70.html)
* [v71.03 鸿蒙内核源码分析(Shell编辑) | 两个任务 三个阶段](http://weharmonyos.com/blog/71.html)
* [v72.01 鸿蒙内核源码分析(Shell解析) | 应用窥伺内核的窗口](http://weharmonyos.com/blog/72.html)
* [v73.01 鸿蒙内核源码分析(参考文档) | 阅读内核源码必备工具](http://weharmonyos.com/blog/73.html)
* [v74.01 鸿蒙内核源码分析(控制台) | 一个让很多人模糊的概念](http://weharmonyos.com/blog/74.html)


#### 四 : 参考手册 | Doxygen呈现

为了很好的输出注解内容,注解标准遵循 [doxygen](https://www.doxygen.nl) 格式标准。函数/头文件/模块之间的调用关系清晰可见,丝丝入扣.
下图为内核`main`的调用关系直观展现,如果没有这张图,光`main`一个函数就够喝一壶。 `main`本身是由汇编指令 `bl        main`调用
![](https://gitee.com/weharmonyos/resources/raw/master/73/1.png)

可前往 >> [鸿蒙研究站 | 参考手册 ](http://weharmonyos.com/doxygen/index.html) 体验

下图为内核所有结构体索引,点击可查看每个结构变量细节
![](https://gitee.com/weharmonyos/resources/raw/master/73/6.png)
可前往 >> [鸿蒙研究站 | 结构体索引 ](http://weharmonyos.com/doxygen/classes.html) 体验

### 四大码仓发布 | 源码同步官方
内核注解同时在 [gitee](https://gitee.com/weharmony/kernel_liteos_a_note) | [github](https://github.com/kuangyufei/kernel_liteos_a_note) | [coding](https://weharmony.coding.net/public/harmony/kernel_liteos_a_note/git/files) | [codechina](https://codechina.csdn.net/kuangyufei/kernel_liteos_a_note) 发布,并与官方源码按月保持同步，同步历史如下:
* `2021/12/11` -- 增加LMS模块,完善PM
* `2021/11/12` -- 加入epoll支持,对shell模块有较大调整,微调process,task,更正单词拼写错误
* `2021/10/21` -- 增加性能优化模块`perf`,优化了文件映射模块
* `2021/09/14` -- common,extended等几个目录结构和Makefile调整
* `2021/08/19` -- 各目录增加了`BUILD.gn`文件,文件系统部分文件调整 
* `2021/07/15` -- 改动不大,新增`blackbox`,`hidumper`,对一些宏规范化使用 
* `2021/06/27` -- 对文件系统/设备驱动改动较大,目录结构进行了重新整理
* `2021/06/08` -- 对编译构建,任务,信号模块有较大的改动
* `2021/05/28` -- 改动不大,主要针对一些错误单词拼写纠正
* `2021/05/13` -- 对系统调用，任务切换，信号处理，异常接管，文件管理，shell做了较大更新，代码结构更清晰
* `2021/04/21` -- 官方优化了很多之前吐槽的地方，点赞
* `2020/09/16` -- 中文注解版起点

### 注解子系统仓库
  
在给鸿蒙内核源码加注过程中发现仅仅注解内核仓库还不够,因为它关联了其他子系统,若对这些子系统不了解是很难完整的注解鸿蒙内核,所以也对这些关联仓库进行了部分注解,这些仓库包括:

* [编译构建子系统 | build_lite](https://gitee.com/weharmony/build_lite_note)  
* [协议栈 | lwip](https://gitee.com/weharmony/third_party_lwip) 
* [文件系统 | NuttX](https://gitee.com/weharmony/third_party_NuttX)
* [标准库 | musl](https://gitee.com/weharmony/third_party_musl) 

### 其他

* [微信 | QQ 群](http://weharmonyos.com/qun.html)
* [离线文档 | PDF | CHM](http://weharmonyos.com/history.html)
* [主流站点 | 全网覆盖](http://weharmonyos.com/extsite.html)
* [送温暖 | 润心田](http://weharmonyos.com/donate.html)
* [新建 Issue](https://gitee.com/weharmony/kernel_liteos_a_note/issues)
* [新建 Pull Request](https://gitee.com/weharmony/kernel_liteos_a_note/pull/new/weharmony:master...weharmony:master)

### 内核目录结构
```
/kernel/liteos_a
├── apps                   # 用户态的init和shell应用程序
├── arch                   # 体系架构的目录，如arm等
│   └── arm                # arm架构代码
├── bsd                    # freebsd相关的驱动和适配层模块代码引入，例如USB等
├── compat                 # 内核接口兼容性目录
│   └── posix              # posix相关接口
├── drivers                # 内核驱动
│   └── char               # 字符设备
│       ├── mem            # 访问物理IO设备驱动
│       ├── quickstart     # 系统快速启动接口目录
│       ├── random         # 随机数设备驱动
│       └── video          # framebuffer驱动框架
├── fs                     # 文件系统模块，主要来源于NuttX开源项目
│   ├── fat                # fat文件系统
│   ├── jffs2              # jffs2文件系统
│   ├── include            # 对外暴露头文件存放目录
│   ├── nfs                # nfs文件系统
│   ├── proc               # proc文件系统
│   ├── ramfs              # ramfs文件系统
│   └── vfs                # vfs层
├── kernel                 # 进程、内存、IPC等模块
│   ├── base               # 基础内核，包括调度、内存等模块
│   ├── common             # 内核通用组件
│   ├── extended           # 扩展内核，包括动态加载、vdso、liteipc等模块
│   ├── include            # 对外暴露头文件存放目录
│   └── user               # 加载init进程
├── lib                    # 内核的lib库
├── net                    # 网络模块，主要来源于lwip开源项目
├── platform               # 支持不同的芯片平台代码，如Hi3516DV300等
│   ├── hw                 # 时钟与中断相关逻辑代码
│   ├── include            # 对外暴露头文件存放目录
│   └── uart               # 串口相关逻辑代码
├── platform               # 支持不同的芯片平台代码，如Hi3516DV300等
├── security               # 安全特性相关的代码，包括进程权限管理和虚拟id映射管理
├── syscall                # 系统调用
├── tools                  # 构建工具及相关配置和代码
└── zzz                    # 中文加注版比官方版无新增文件，只多了一个zzz的目录，里面放了一些图片/文件/工具，
                           # 它与内核代码无关，大家可以忽略它，取名zzz是为了排在最后，减少对原有代码目录级的侵入，
                           # zzz 的想法源于微信中名称为AAA的那帮朋友，你的微信里应该也有他们熟悉的身影吧 :|P
```
### 关注不迷路.代码即人生

![](https://gitee.com/weharmonyos/resources/raw/master/common/so1so.png)

**QQ群 790015635 | 入群密码 666  | 存放重要文档资料**
