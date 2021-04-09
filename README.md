[![在这里插入图片描述](https://gitee.com/weharmony/docs/raw/master/pic/other/io.png)](https://weharmony.gitee.io)

[百万汉字注解 >> 精读内核源码,中文注解分析, 深挖地基工程,大脑永久记忆,四大码仓每日同步更新<](https://gitee.com/weharmony/kernel_liteos_a_note)[ gitee ](https://gitee.com/weharmony/kernel_liteos_a_note)[| github ](https://github.com/kuangyufei/kernel_liteos_a_note)[| csdn ](https://codechina.csdn.net/kuangyufei/kernel_liteos_a_note)[| coding ](https://weharmony.coding.net/public/harmony/kernel_liteos_a_note/git/files)[>]()

[百篇博客分析 >> 故事说内核,问答式导读,生活式比喻,表格化说明,图形化展示,主流站点定期更新中<](https://my.oschina.net/weharmony)[ oschina ](https://my.oschina.net/weharmony)[| csdn ](https://blog.csdn.net/kuangyufei)[| harmony ](https://weharmony.gitee.io/)[>]()

---

## **百万汉字注解**

**[kernel\_liteos\_a_note](https://gitee.com/weharmony/kernel_liteos_a_note)** 是在鸿蒙官方开源项目 **[kernel\_liteos\_a](https://gitee.com/openharmony/kernel_liteos_a)** 基础上给源码加上中文注解的版本

### **为何要精读内核源码?**

* 在每位码农的学职生涯,都应精读一遍内核源码.以浇筑好计算机知识大厦的地基，地基纵深的坚固程度，很大程度能决定了未来大厦能盖多高。为何一定要精读细品呢?
  
* 因为内核代码本身并不太多，都是浓缩的精华，精读是让各个知识点高频出现，不孤立成点状记忆,让各点相连成面,刻意练习,闪爆大脑，如此短时间内容易结成一张高浓度，高密度的底层网，不断训练大脑肌肉记忆,将这些地基信息从临时记忆区转移到永久记忆区。跟骑单车一样，一旦学会，即便多年不骑，照样跨上就走，游刃有余。

### **热爱是所有的理由和答案**

* 因大学时阅读 `linux 2.6` 内核痛并快乐的经历,一直有个心愿,如何让更多对内核感兴趣的朋友减少阅读时间,加速对计算机系统级的理解,而不至于过早的放弃.但因过程种种,多年一直没有行动,基本要放弃这件事了. 恰逢 **2020/9/10** 鸿蒙正式开源,重新激活了多年的心愿,就有那么点一发不可收拾了. 
  
* 到 **2021/3/10** 刚好半年, 对内核源码的注解已完成了 **70%** ,对内核源码的博客分析已完成了**40篇**, 每天都很充实,很兴奋,连做梦内核代码都在往脑海里鱼贯而入.如此疯狂地做一件事还是当年谈恋爱的时候, 只因热爱, 热爱是所有的理由和答案. :P

### **(〃･ิ‿･ิ)ゞ鸿蒙内核开发者**

* 感谢开放原子开源基金会,致敬鸿蒙内核开发者提供了如此优秀的源码,一了多年的夙愿,津津乐道于此.精读内核源码当然是件很困难的事,时间上要以月甚至年为单位,但正因为很难才值得去做! 干困难事,必有所得. 专注聚焦,必有所获. 

* 从内核一行行的代码中能深深感受到开发者各中艰辛与坚持,及鸿蒙生态对未来的价值,这些是张嘴就来的网络喷子们永远不能体会到的.可以毫不夸张的说鸿蒙内核源码可作为大学 **C语言**,**数据结构**,**操作系统**,**汇编语言**,**计算机组成原理** 五门课程的教学项目.如此宝库,不深入研究实在是暴殄天物,于心不忍,注者坚信鸿蒙大势所趋,未来可期,是其坚定的追随者和传播者.

### **加注方式是怎样的？**

*  因鸿蒙内核6W+代码量,本身只有较少的注释, 中文注解以不对原有代码侵入为前提,源码中所有英文部分都是原有注释,所有中文部分都是中文版的注释,尽量不去增加代码的行数,不破坏文件的结构,试图把知识讲透彻,注释多类似以下的方式:

    在重要模块的.c文件开始位置先对模块功能做整体的介绍,例如异常接管模块注解如图所示:

    ![在这里插入图片描述](https://gitee.com/weharmony/docs/raw/master/pic/other/ycjg.png)

    注解过程中查阅了很多的资料和书籍,在具体代码处都附上了参考链接.

* 而函数级注解会详细到重点行,甚至每一行, 例如申请互斥锁的主体函数,不可谓不重要,而官方注释仅有一行,如图所示

    ![在这里插入图片描述](https://gitee.com/weharmony/docs/raw/master/pic/other/sop.png)

* 另外画了一些字符图方便理解,直接嵌入到头文件中,比如虚拟内存的全景图,因没有这些图是很难理解虚拟内存是如何管理的.

    ![在这里插入图片描述](https://gitee.com/weharmony/docs/raw/master/pic/other/vm.png)

### **理解内核的三个层级**

注者认为理解内核可分三个层级:

* **普通概念映射级:** 这一级不涉及专业知识,用大众所熟知的公共认知就能听明白是个什么概念,也就是说用一个普通人都懂的概念去诠释或者映射一个他们从没听过的概念.让陌生的知识点与大脑中烂熟于心的知识点建立多重链接,加深记忆.说别人能听得懂的话这很重要!!! 一个没学过计算机知识的卖菜大妈就不可能知道内核的基本运作了吗? 不一定!,在系列篇中试图用 **[鸿蒙内核源码分析(总目录)之故事篇](https://my.oschina.net/weharmony)** 去引导这一层级的认知,希望能卷入更多的人来关注基础软件,尤其是那些资本大鳄,加大对基础软件的投入.

* **专业概念抽象级:** 对抽象的专业逻辑概念具体化认知, 比如虚拟内存,老百姓是听不懂的,学过计算机的人都懂,具体怎么实现的很多人又都不懂了,但这并不妨碍成为一个优秀的上层应用程序员,因为虚拟内存已经被抽象出来,目的是要屏蔽上层对它的现实认知.试图用 **[鸿蒙内核源码分析(总目录)百篇博客](https://my.oschina.net/weharmony)** 去拆解那些已经被抽象出来的专业概念, 希望能卷入更多对内核感兴趣的应用软件人才流入基础软件生态, 应用软件咱们是无敌宇宙,但基础软件却很薄弱.

* **具体微观代码级:** 这一级是具体到每一行代码的实现,到了用代码指令级的地步,这段代码是什么意思?为什么要这么设计? **[鸿蒙内核源码注解分析](https://gitee.com/weharmony/kernel_liteos_a_note)** 试图从细微处去解释代码实现层,英文真的是天生适合设计成编程语言的人类语言,计算机的01码映射到人类世界的26个字母,诞生了太多的伟大奇迹.但我们的母语注定了很大部分人存在着自然语言层级的理解映射,希望对鸿蒙内核源码注解分析能让更多爱好者快速的理解内核,共同进步.
## **百篇博客分析**

* 给 **[鸿蒙内核源码加注释](https://gitee.com/weharmony/kernel_liteos_a_note)** 过程中,整理出以下文章.内容多以 轻松口语化的故事,生活场景打比方,表格,图像 将尽可能多的内核知识点置入某种场景,具有画面感,形成多重联接路径,达到轻松记忆,甚至永久记忆的目的.
* 鸿蒙内核源码注解分析系列不是百度教条式的在说清楚一堆诘屈聱牙的概念,那没什么意思.更希望是让内核变得栩栩如生,倍感亲切.确实有难度,不自量力,但已经出发,回头已是不可能的了.:P

### **鸿蒙源码百篇博客 往期回顾**

* [v46.xx (特殊进程篇) | 龙生龙,凤生凤,老鼠生儿会打洞  ](https://my.oschina.net/weharmony/blog/5014444) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/115556505) [ | harmony  ](https://weharmony.gitee.io/46_特殊进程篇篇.html)[>]()**

* [v45.xx (fork篇) | fork是如何做到调用一次,返回两次的 ?  ](https://my.oschina.net/weharmony/blog/5010301) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/115467961) [ | harmony  ](https://weharmony.gitee.io/45_fork篇.html)[>]()**

* [v44.xx (中断管理篇) | 硬中断的实现<>观察者模式 ](https://my.oschina.net/weharmony/blog/4995800) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/115130055) [ | harmony  ](https://weharmony.gitee.io/44_中断管理篇.html)[>]()**

* [v43.xx (中断概念篇) | 外人眼中权势滔天的当红海公公 ](https://my.oschina.net/weharmony/blog/4992750) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/115014442) [ | harmony  ](https://weharmony.gitee.io/43_中断概念篇.html)[>]()**

* [v42.xx (中断切换篇) | 中断切换到底在切换什么?](https://my.oschina.net/weharmony/blog/4990948) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114988891) [ | harmony  ](https://weharmony.gitee.io/42_中断切换篇.html)[>]()**

* [v41.xx (任务切换篇) | 汇编逐行注解分析任务上下文 ](https://my.oschina.net/weharmony/blog/4988628) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114890180) [ | harmony  ](https://weharmony.gitee.io/41_任务切换篇.html)[>]()**

* [v40.xx (汇编汇总篇) |  所有的汇编代码都在这里 ](https://my.oschina.net/weharmony/blog/4977924) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114597179) [ | harmony  ](https://weharmony.gitee.io/40_汇编汇总篇.html)[>]()**

* [v39.xx (异常接管篇) | 社会很单纯,复杂的是人 ](https://my.oschina.net/weharmony/blog/4973016) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114438285) [ | harmony  ](https://weharmony.gitee.io/39_异常接管篇.html)[>]()**

* [v38.xx (寄存器篇) | ARM所有寄存器一网打尽,不再神秘 ](https://my.oschina.net/weharmony/blog/4969487) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114326994) [ | harmony  ](https://weharmony.gitee.io/38_寄存器篇.html)[>]()**

* [v37.xx (系统调用篇) | 全盘解剖系统调用实现过程 ](https://my.oschina.net/weharmony/blog/4967613) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114285166) [ | harmony  ](https://weharmony.gitee.io/37_系统调用篇.html)[>]()**

* [v36.xx (工作模式篇) | CPU是韦小宝,有哪七个老婆? ](https://my.oschina.net/weharmony/blog/4965052) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/114168567) [ | harmony  ](https://weharmony.gitee.io/36_工作模式篇.html)[>]()**

* [v35.xx (时间管理篇) | Tick是操作系统的基本时间单位 ](https://my.oschina.net/weharmony/blog/4956163) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113867785) [ | harmony  ](https://weharmony.gitee.io/35_时间管理篇.html)[>]()**

* [v34.xx (原子操作篇) | 是谁在为原子操作保驾护航? ](https://my.oschina.net/weharmony/blog/4955290) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113850603) [ | harmony  ](https://weharmony.gitee.io/34_原子操作篇.html)[>]()**

* [v33.xx (消息队列篇) | 进程间如何异步解耦传递大数据 ? ](https://my.oschina.net/weharmony/blog/4952961) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113815355) [ | harmony  ](https://weharmony.gitee.io/33_消息队列篇.html)[>]()**

* [v32.xx (CPU篇) | 内核是如何描述CPU的? ](https://my.oschina.net/weharmony/blog/4952034) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113782749) [ | harmony  ](https://weharmony.gitee.io/32_CPU篇.html)[>]()**  

* [v31.xx (定时器篇) | 内核最高优先级任务是谁? ](https://my.oschina.net/weharmony/blog/4951625) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113774260) [ | harmony  ](https://weharmony.gitee.io/31_定时器机制篇.html)[>]()**

* [v30.xx (事件控制篇) | 任务间多对多的同步方案 ](https://my.oschina.net/weharmony/blog/4950956) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113759481) [ | harmony  ](https://weharmony.gitee.io/30_事件控制篇.html)[>]()**

* [v29.xx (信号量篇) | 信号量解决任务同步问题 ](https://my.oschina.net/weharmony/blog/4949720) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113744267) [ | harmony  ](https://weharmony.gitee.io/29_信号量篇.html)[>]()**
  
* [v28.xx (进程通讯篇) | 进程间通讯有哪九大方式? ](https://my.oschina.net/weharmony/blog/4947398) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113700751) [ | harmony  ](https://weharmony.gitee.io/28_进程通讯篇.html)[>]()**

* [v27.xx (互斥锁篇) | 互斥锁比自旋锁可丰满许多 ](https://my.oschina.net/weharmony/blog/4945465) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113660357) [ | harmony  ](https://weharmony.gitee.io/27_互斥锁篇.html)[>]()**

* [v26.xx (自旋锁篇) | 真的好想为自旋锁立贞节牌坊! ](https://my.oschina.net/weharmony/blog/4944129) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113616250) [ | harmony  ](https://weharmony.gitee.io/26_自旋锁篇.html)[>]()**

* [v25.xx (并发并行篇) | 怎么记住并发并行的区别? ](https://my.oschina.net/u/3751245/blog/4940329) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113516222) [ | harmony  ](https://weharmony.gitee.io/25_并发并行篇.html)[>]()**

* [v24.xx (进程概念篇) | 进程在管理哪些资源? ](https://my.oschina.net/u/3751245/blog/4937521) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113395872) [ | harmony  ](https://weharmony.gitee.io/24_进程概念篇.html)[>]()**

* [v23.xx (汇编传参篇) | 汇编如何传递复杂的参数? ](https://my.oschina.net/u/3751245/blog/4927892) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/113265990) [ | harmony  ](https://weharmony.gitee.io/23_汇编传参篇.html)[>]()**

* [v22.xx (汇编基础篇) | CPU在哪里打卡上班? ](https://my.oschina.net/u/3751245/blog/4920361) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/112986628) [ | harmony  ](https://weharmony.gitee.io/22_汇编基础篇.html)[>]()**

* [v21.xx (线程概念篇) | 是谁在不断的折腾CPU? ](https://my.oschina.net/u/3751245/blog/4915543) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/112870193) [ | harmony  ](https://weharmony.gitee.io/21_线程概念篇.html)[>]()**

* [v20.xx (用栈方式篇) | 栈是构建底层运行的基础 ](https://my.oschina.net/u/3751245/blog/4893388) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/112534331) [ | harmony  ](https://weharmony.gitee.io/20_用栈方式篇.html)[>]()**

* [v19.xx (位图管理篇) | 为何进程和线程优先级都是32个? ](https://my.oschina.net/u/3751245/blog/4888467) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/112394982) [ | harmony  ](https://weharmony.gitee.io/19_位图管理篇.html)[>]()**

* [v18.xx (源码结构篇) | 内核500问你能答对多少? ](https://my.oschina.net/u/3751245/blog/4869137) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/111938348) [ | harmony  ](https://weharmony.gitee.io/18_源码结构篇.html)[>]()**

* [v17.xx (物理内存篇) | 这样记伙伴算法永远不会忘 ](https://my.oschina.net/u/3751245/blog/4842408) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/111765600) [ | harmony  ](https://weharmony.gitee.io/17_物理内存篇.html)[>]()**

* [v16.xx (内存规则篇) | 内存管理到底在管什么? ](https://my.oschina.net/u/3751245/blog/4698384) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/109437223) [ | harmony  ](https://weharmony.gitee.io/16_内存规则篇.html)[>]()**

* [v15.xx (内存映射篇) | 什么是内存最重要的实现基础 ? ](https://my.oschina.net/u/3751245/blog/4694841) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/109032636) [ | harmony  ](https://weharmony.gitee.io/15_内存映射篇.html)[>]()**

* [v14.xx (内存汇编篇) | 什么是虚拟内存的实现基础? ](https://my.oschina.net/u/3751245/blog/4692156) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108994081) [ | harmony  ](https://weharmony.gitee.io/14_内存汇编篇.html)[>]()**

* [v13.xx (源码注释篇) | 热爱是所有的理由和答案 ](https://my.oschina.net/u/3751245/blog/4686747) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/109251754) [ | harmony  ](https://weharmony.gitee.io/13_源码注释篇.html)[>]()**

* [v12.xx (内存管理篇) | 虚拟内存全景图是怎样的? ](https://my.oschina.net/u/3751245/blog/4652284) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108821442) [ | harmony  ](https://weharmony.gitee.io/12_内存管理篇.html)[>]()**

* [v11.xx (内存分配篇) | 内存有哪些分配方式? ](https://my.oschina.net/u/3751245/blog/4646802) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108989906) [ | harmony  ](https://weharmony.gitee.io/11_内存分配篇.html)[>]()**

* [v10.xx (内存主奴篇) | 紫禁城的主子和奴才如何相处? ](https://my.oschina.net/u/3751245/blog/4646802) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108723672) [ | harmony  ](https://weharmony.gitee.io/10_内存主奴篇.html)[>]()**
  
* [v09.xx (调度故事篇) | 用故事说内核调度 ](https://my.oschina.net/u/3751245/blog/4634668) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108745174) [ | harmony  ](https://weharmony.gitee.io/09_调度故事篇.html)[>]()**

* [v08.xx (总目录) | 百万汉字注解 百篇博客分析 ](https://my.oschina.net/weharmony) **[<]()[  csdn](https://blog.csdn.net/kuangyufei) [ | harmony  ](https://weharmony.gitee.io/08_总目录.html)[>]()**
  
* [v07.xx (调度机制篇) | 任务是如何被调度执行的? ](https://my.oschina.net/u/3751245/blog/4623040) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108705968) [ | harmony  ](https://weharmony.gitee.io/07_调度机制篇.html)[>]()**

* [v06.xx (调度队列篇) | 就绪队列对调度的作用 ](https://my.oschina.net/u/3751245/blog/4606916) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108626671) [ | harmony  ](https://weharmony.gitee.io/06_调度队列篇.html)[>]()**

* [v05.xx (任务管理篇) | 谁在让CPU忙忙碌碌? ](https://my.oschina.net/u/3751245/blog/4603919) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108661248) [ | harmony  ](https://weharmony.gitee.io/05_任务管理篇.html)[>]()**
  
* [v04.xx (任务调度篇) | 任务是内核调度的单元 ](https://my.oschina.net/weharmony/blog/4595539) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108621428) [ | harmony  ](https://weharmony.gitee.io/04_任务调度篇.html)[>]()**

* [v03.xx (时钟任务篇) | 触发调度最大的动力来自哪里? ](https://my.oschina.net/u/3751245/blog/4574493) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108603468) [ | harmony  ](https://weharmony.gitee.io/03_时钟任务篇.html)[>]()**

* [v02.xx (进程管理篇) | 进程是内核资源管理单元 ](https://my.oschina.net/u/3751245/blog/4574429) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108595941) [ | harmony  ](https://weharmony.gitee.io/02_进程管理篇.html)[>]()**

* [v01.xx (双向链表篇) | 谁是内核最重要结构体? ](https://my.oschina.net/u/3751245/blog/4572304) **[<]()[  csdn](https://blog.csdn.net/kuangyufei/article/details/108585659) [ | harmony  ](https://weharmony.gitee.io/01_双向链表篇.html)[>]()**


### **主流站点**

感谢 `oschina`，`csdn`，`华为开发者论坛`, `51cto`, `掘金`,`电子发烧友`，以及其他小伙伴对系列文章的转载和推荐，无以为报，唯有不断的深挖内核地基，输出更多内容，错漏之处请多见谅，会持续完善源码注解和文档内容，精雕细琢，尽全力打磨精品内容。

**文章输出站点:**

* **[oschina ](https://my.oschina.net/weharmony)**
* **[csdn ](https://blog.csdn.net/kuangyufei)**
* **[51cto](https://harmonyos.51cto.com/column/34)**
* **[掘金](https://harmonyos.51cto.com/column/34)**
* **[华为开发者论坛](https://developer.huawei.com/consumer/cn/forum/)**
* **[头条号](https://gitee.com/weharmony/docs/raw/master/pic/other/tt.png)**
* **[公众号](https://gitee.com/weharmony/docs/raw/master/pic/other/so1so.png)**
* **[weharmony.gitee.io](https://weharmony.gitee.io)**
* **[weharmony.github.io](https://weharmony.github.io)**

## **Fork Me**

* 注解几乎占用了所有的空闲时间,每天都会更新,每天都有新感悟,一行行源码在不断的刷新和拓展对内核知识的认知边界. 对已经关注和fork的同学请及时同步最新的注解内容. 内核知识点体量实在太过巨大,过程会反复修正完善,力求言简意赅,词达本意.肯定会有诸多错漏之处,请多包涵. :)  

### **有哪些特殊的记号**

* 搜索 **`@note_pic`** 可查看绘制的全部字符图

* 搜索 **`@note_why`** 是尚未看明白的地方，有看明白的，请Pull Request完善

* 搜索 **`@note_thinking`** 是一些的思考和建议

* 搜索 **`@note_#if0`** 是由第三方项目提供不在内核源码中定义的极为重要结构体，为方便理解而添加的。

* 搜索 **`@note_good`** 是给源码点赞的地方

### **新增zzzz目录**

* 中文加注版比官方版无新增文件,只多了一个zzz的目录,里面放了一些文件,它与内核代码无关,大家可以忽略它,取名zzz是为了排在最后,减少对原有代码目录级的侵入,zzz的想法源于微信中名称为AAA的那帮朋友,你的微信里应该也有他们熟悉的身影吧 :|P

### 参与贡献

* [访问注解仓库地址](https://gitee.com/weharmony/kernel_liteos_a_note)

* [Fork 本仓库](https://gitee.com/weharmony/kernel_liteos_a_note) >> 新建 Feat_xxx 分支 >> 提交代码注解 >> [新建 Pull Request](https://gitee.com/weharmony/kernel_liteos_a_note/pull/new/weharmony:master...weharmony:master)

* [新建 Issue](https://gitee.com/weharmony/kernel_liteos_a_note/issues)

### 喜欢请「点赞+关注+收藏」

* [关注「鸿蒙内核源码分析」公众号，百万汉字注解 + 百篇博客分析 => 深挖鸿蒙内核源码](https://gitee.com/weharmony/docs/raw/master/pic/other/so1so.png)

* 各大站点搜 **「鸿蒙内核源码分析」** .欢迎转载,请注明出处. 