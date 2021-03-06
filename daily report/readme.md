## 这是什么？

记录日常学习

备注：1. 前3个月的学习还在整理中以周/月为记录单位，从19/10/2020开始以天为记录单位。  
    2. 满课指的是不得不完成的学校指标，例如8：00上课，22：00下课，课程中间签到、签退；实验课成果现场展示等不能依靠万能的舍友完成的任务。

<details>
<summary>2020</summary>

<details>
<summary>2020.7-9 总结</summary>

+ [x] 梦的开始，由于在家无聊想草Windows内核，于是学习HEVD系列，加了tj大佬的好友，于是有了下面的故事：
+ [x] 7月份加入了神秘组织，开始学习并抄写SGI STL，视频看的是侯捷的C+教程以及STL教程，参考书籍依然是侯捷的STL源码剖析
+ [x] STL没写完，直接切入写Linux内核的作业中，参考书籍是《操作系统真相还原》
+ [x] 群友大部分都切到了内核，统一学习CSAPP，我也花了差不多一周把南大视频刷完了
+ [x] 花了差不多2周去做CSAPP Lab，写到差不多malloc lab继续写内核，没有写ucore
+ [x] 这时候差不多开始期末考试复习了，所以一个月skr lab基本没进度，大悲
+ [x] 开学之后课程有点多，到10月左右的国赛时写了个linux debugger，国赛水了个国二，ylb垃圾，，哦其实还有个写ELF parser的作业，因为要打国赛而且又写过PE Parser，所以这个作业就咕咕咕了。
+ [x] 好了，这三个月其实也不止做了这么多事，其中还有恰不少烂钱买mbp，接私活，捡垃圾折腾物联网日学校什么的事情，到下面的日报也基本衔接得上了   

</details>

<details>
<summary>2020.10.19 满课</summary>
</details>

<details>
<summary>2020.10.20 满课&日跑步软件</summary>

+ [x] 花了30min做了一个自动跑步地app应付检查，以增加以后的学习时间
</details>

<details>
<summary>2020.10.21 满课&win32k.sys&漏洞wp</summary>

+ [x] 分析了win32k.sys在内核中加载的规律，学习了windbg一些调试技巧，例如：dd以及!dd的使用场景


+ [ ] MS16-098 (整数溢出)

  正在产出MS16-098的wp，帮助国内相关研究人员更好地理解此漏洞产生原理和利用方式

  + [生肉](https://sensepost.com/blog/2017/exploiting-ms16-098-rgnobj-integer-overflow-on-windows-8.1-x64-bit-by-abusing-gdi-objects/)

   

</details>

<details>
<summary>2020.10.22 满课&MS16-089wp</summary>

+ [ ] MS16-098 analysis
  

</details>

<details>
<summary>2020.10.23 实验课&MS16-089wp&GoogleCTF</summary>

+ [x] MS16-098 analysis [链接](https://github.com/M-ouse/Mysterious-Learning/tree/master/CVE%E5%88%86%E6%9E%90/MS16-098)
+ [x] 写了个实验课的project，虽然挺水的也算是coding了吧
+ [ ] Google CTF 2020 MathSH
  

</details>

<details>
<summary>2020.10.24 Windows Internal&GoogleCTF</summary>

+ [x] Windows Internal `Chapter5 : Memory Management Introduction to the memory manager->Virtual address space layouts`
+ [ ] 补觉
+ [ ] Google CTF 2020 MathSH
  

</details>

<details>
<summary>2020.10.25 Windows Internal&GoogleCTF</summary>

+ [x] Windows Internal `Chapter5 : Stacks Chapter4 : Threads`
+ [ ] 健身
+ [ ] Google CTF 2020 MathSH
  

</details>

<details>
<summary>2020.10.26 准备面试 & review 泛型、kernel</summary>

+ [x] 听说玄武收windows方向的寒假实习生，准备了下简历，并review了下以前写过的项目
    + [x] C++ STL：C++ 泛型编程、traits的使用
    + [x] Linux Kernel：review了下自己做了什么，从加电到保护模式

</details>

<details>
<summary>2020.10.27 准备面试 & review C++ OOP、Windows内核、Linux调试器、调过的漏洞</summary>

+ [x] 花了半天扫了一遍C++的一些经典OOP问题以及不同版本C++的特性，例如：智能指针、lambda表达式
+ [x] 过了一遍老外的内核面试题目，虽然没什么用也算补了一点之前没有注意的知识点：Trap Frame
+ [x] 复习了内核里进程通信的一些手段：信号、管道、共享内存；同步手段：信号量、锁等；用户态陷入内核；内核对象；内核中比较重要的结构
+ [x] review了下调过的MS16-098的背景、利用
+ [x] 总结了STL里因为浅拷贝引起的Crash
  

</details>

<details>
<summary>2020.10.28 玄武实验室一面</summary>

+ [x] 早上总结了下目前以来遇到的漏洞模式，虽然最终都会归到最基本的UAF、Double Free等等，明白程序的原本功能和利用路径还是比较重要的
+ [x] 复习了Windows Internal的笔记
+ [x] 下午一面，面试的过程比较友好，尽可能地把自己的思路和会的知识说了出去，约二面
+ [x] 晚上把面试问到不会的问题搜了一遍
  

</details>

<details>
<summary>2020.10.29 玄武实验室二面</summary>

+ [ ] 和大佬聊天了解到了还有因为变量未初始化引起的漏洞，学习中
+ [x] 下午体测、人快没了
+ [x] 体测后才注意到第二次的面试官发了短信，于是放弃晚饭立刻面试，可能是刚体测完脑子不好使或者是太紧张，思路有些堵住，晚上在床上突然想出思路 。约三面
+ [x] 晚上依旧把面试不会的问题记录并学习
  

</details>

<details>
<summary>2020.10.30 摸鱼</summary>



</details>

<details>
<summary>2020.10.31 Windows SSDT Hook&驱动进程隐藏</summary>

+ [x] 写了个驱动实现进程隐藏，基本是unlink，但是在回收进程的时候炸了，直接蓝屏
+ [ ] Windows 10 SSDT Hook 
  

</details>

<details>
<summary>2020.11.01 Windows 进程守护 & 内核实验</summary>

+ [x] [内核实验](https://www.ired.team/)
  + [x] Manipulating ActiveProcessLinks to Hide Processes in Userland
  + [x] Dll injection  
+ [x] [通过过滤请求的方法守护进程](https://bbs.pediy.com/thread-168023.htm)
  

</details>

<details>
<summary>2020.11.02 Windows RDP协议漏洞</summary>

+ [ ] https://bbs.pediy.com/thread-256766.htm 
  

</details>

<details>
<summary>2020.11.03 Windows RDP协议漏洞</summary>

+ [x] https://bbs.pediy.com/thread-256766.htm 
  

</details>

<details>
<summary>2020.11.04 满课</summary>

</details>

<details>
<summary>2020.11.05 HR面 && 复习文化课</summary>

</details>

<details>
<summary>2020.11.06 Windows Internal Process</summary>

+ [x] 笔记Creating Process -> Flow of Create Process
  

</details>

<details>
<summary>2020.11.07 面试新队员</summary>

+ [x] 摸鱼
  

</details>

<details>
<summary>2020.11.08 CodeQL</summary>

+ [x] 学习CodeQL的使用
  + [x] [基本语法及应用](https://bestwing.me/codeql.html)
  + [x] 在mac下搭建需要注意的点：下载好binary之后放置到合适目录，并在/bin/path下设置好变量，推荐使用vscode workspace。创建编译型语言的数据库的时候需要指定command，我使用的是make，因此也需要makeile基础 
  

</details>

<details>
<summary>2020.11.09 - 2020.11.15 复习文化课</summary>

+ [x] 复习文化课
  

</details>

<details>
<summary>2020.11.16 CVE-2014-1767 双重释放漏洞POC分析</summary>

+ [ ] CVE-2014-1767分析：漏洞由于afd.sys的两个函数在处理socket数据包的过程中，异常处理机制中一个结构体指针在free之后没有被置零，两次异常处理调用到的为同一个函数，对这个指针做出了free操作造成double free。该double free可以转为UAF实现任意写打入shellcode，窃取token实现提权。
  

</details>

<details>
<summary>2020.11.17 CVE-2014-1767 双重释放漏洞exp分析 && 内核SEH机制</summary>

+ [ ] 双重释放漏洞exp分析
  + [x] 参考书籍：《漏洞战争》
  + [x] 学习了内核中SEH的机制。
  

</details>

<details>
<summary>2020.11.18 WCTF2020 day0 && 360游览</summary>

+ [x] wctf2020前一天，做了一下以前的题目，利用windows defender实现侧信道攻击：通过web服务器向windows靶机写入病毒特征码，中间利用flag文件字符构造，从而逐字节爆破出flag。
+ [x] 去比赛现场看了一眼，顺便游览了下vulcan和alpah实验室，出人意料的简朴。和sakura教练以及thunder大哥恰了顿晚饭。
  

</details>

<details>
<summary>2020.11.19 WCTF2020 day1</summary>

+ [x] 爆0的一天，一整天都在手撕rust逆向minesweeper，累死。和学长一起逆的，学长很强，逆向出了”秘籍“按键可以让题目打log，但题目远程被限制了info调试信息权限，打不开debug；我想应该需要日rdp的一些服务。
  

</details>

<details>
<summary>2020.11.20 WCTF2020 day2</summary>

+ [x] 队伍做出来了3道题，一个1血和一个3血，很强，最后新锐赛rank4。接着说minesweeper那道题，这个题被我们非预期拿到了1血：通过连续按动shift按键唤出本地&远程的粘滞键设置然后调用shell，直接拿到flag抢到全场一血。（学长就比我快5s尝试这个方法，呜呜，不然就可以抢到这个一血了）。除了扫雷其他题是真的看不懂，化身茶歇终结者。比赛结束前2min30s我们队伍又出了一道题，血压飙升，紧张刺激。
  

</details>

<details>
<summary>2020.11.21 WCTF2020 day3</summary>

+ [x] 学习了扫雷那道题的官方做法：测信道攻击，因为通过秘籍进入debug模式之后，cursor在block上移动的时候会detect周围雷的数量，利用反应时间不同来辨别出方块下面雷的情况。
  

</details>

<details>
<summary>2020.11.22 -  2020.12.6 考试周</summary>

+ [x] 考试周前夕+考试周
  

</details>

<details>
<summary>2020.12.7 总结逆向技巧</summary>

+ [x] （未公开）看了国外一篇博客关于逆向的30个tricks，感觉上和自己摸索出来的差不多，其中利用RS Hacker的方法定位GUI程序关键函数很棒。
+ [x] 淦实习任务 
  

</details>

<details>
<summary>2020.12.8 Windows Dll hacking</summary>

+ [x] (未公开)学习了一种日dll的方法，灵感来自打pwn时做流量转发的马，如果做一个dll proxy夹在程序和真正的dll中，就可以持久化执行我们想部署的命令，甚至可以rootkit
  - 参考资料：
    - [C#写的工具](https://github.com/Flangvik/SharpDllProxy)
    - [dll导出函数的一些问题](https://stackoverflow.com/questions/2804893/c-dll-export-decorated-mangled-names) tips：如果要用C++开发又要“很干净”地导出你的函数，用C的形式导出吧，extern "C"搓个宏
    - [讲理论的blog 1](https://kevinalmansa.github.io/application%20security/DLL-Proxying/)
    - [讲理论的blog 2](https://itm4n.github.io/dll-proxying/)
+ [x] 淦实习任务
  

</details>

<details>
<summary>2020.12.9 满课</summary>

</details>

<details>
<summary>2020.12.10 培训 & x86 win7 defender逆向</summary>

+ [x]接了一期钱少的很的培训，，麻了
+ [x]defender不好日，从最原始的开始入手，翻到了[微软的文档](https://docs.microsoft.com/en-us/windows/win32/lwef/windows-defender-functions),在学校上课真耽误学习，，，
  

</details>

<details>
<summary>2020.12.11 培训 & 满课</summary>

+ [x] 满课跑路
+ [x] 满课
  

</details>

<details>
<summary>2020.12.12 淦实习任务</summary>

+ [x] [调试一个大神把windows defender移植到linux上的项目](https://github.com/taviso/loadlibrary)
+ [x] 看了一些关于api hook的资料 
  

</details>

<details>
<summary>2020.12.13 Windows Defender ByPass & 阅读 Github上AlphaSecLab上关于windows安全的文章 & 淦实习任务</summary>

+ [x] 实习相关已整理到实习repo
+ [x] https://github.com/alphaSeclab/windows-security（未整理完）
  

</details>

<details>
<summary>2020.12.14 阅读《Windows黑客编程》</summary>

+ [x]《windows黑客编程》，看着挺基础的，给淦实习任务打打基础。
  + [x] 基础技术、注入技术、启动技术
  

</details>

<details>
<summary>2020.12.22 合集</summary>

淦，，，又断更了，，回忆下都做了什么

+ [x] 继续学习驱动的编写，之前学的太弟弟了，继续熟悉了下IRP以及文件过滤
+ [x] 抽空写了个CTFChallengeMonitor项目，检测自己的题目被激活的状态
+ [x] 漏洞没时间看。。。
+ [x] 想学一下C++高并发网络编程，看到个github项目`muduo`，考虑下吧
+ [x] 深入理解了Windows窗口程序的机制
+ [x] 学了点Go，搓出来一个爆破网络账号的脚本，不得不说网络编程比C++好写 
+ [x] 课太多了，到此为止吧
  
<<<<<<< HEAD
</details>


</details>

<details>
<summary>2021.1 - 2021.2 玄武实验室实习</summary>

+ [x] 独立在做一个项目，导师留的进度基本上算是完成了，从中发现的问题：
  + [x] 和导师沟通的方法有待提升，提高效率以及解决问题问题才是应该做的事情，自闭不可取
  + [x] 遇到棘手的问题想绕过或缺乏解决欲望，也是我一直想学pwn但进度不佳的原因
  + [x] 缺乏开发经验以及高级C++特性的了解，上手大型工程的速度较慢
  + [x] 逆向依然有很长的路要走
  + [x] 写文档的语言也需要学习，简单明了是目前仍然做不到的
  + [x] 永远不要用当前身的眼界限制自己的潜能，实习不仅是经验过程的积累，组员的评价可以更好地帮助自我认知
+ [x] 在职期间额外写了个1day的poc，分析思路和一段时间过后网上大佬的思路基本相同，并学习了利用技巧
  
</details>

<details>
<summary>2021.2.28 winpwn</summary>

+ [x] 搭建起windows pwn环境
+ [ ] [HITB GSEC]BABYSTACK
  + [x] 原理
    + [x] 攻击面 
    + [x] GS保护
    + [x] SEH底层实现（进入vcruntime） 
  + [ ] exp
  + [ ] wp 
+ [x] Windows内存安全缓解机制了解
  
</details>

<details>
<summary>2021.3.1 - 3.7 Qt课设</summary>

+ [x] 一直在做Qt cpp课设，基本没什么心学的点，解耦的操作都被老师每天一变的要求摧毁了，除了增加点微不足道的开发经验没什么可记录的
+ [x] 真担心学校的课耽误我学习（就这么多吧）
+ [x] 下一步该干什么，学Pwn？继续搞洞？办XCTF分站？卷CTF？（太菜了，只想打原神  
  
</details>


<details>
<summary>2021.3.8 MacOS沙箱</summary>

+ [x] 入门MacOS的app沙箱启动过程
+ [x] 学xpc协议的细节 

</details>

<details>
<summary>2021.3.9 MacOS沙箱</summary>

+ [x] 学习了沙箱tarce以及历史沙箱的启动逻辑
+ [x] 看到一道macos沙箱逃逸的ctf题，打算学会之后用在战队公开赛上 
+ [ ] Linux虚表攻击

</details>

<details>
<summary>2021.3.10 MacOS沙箱，XPC，IPC</summary>

+ [x] Mach的进程通信手段以及阅读对应示例源码
+ [x] macos沙箱逃逸题源码解读
+ [x] 逃逸题目相关的p0文章阅读（题目来源CVE）：
  + [ ] https://bugs.chromium.org/p/project-zero/issues/detail?id=959
  + [ ] https://bugs.chromium.org/p/project-zero/issues/detail?id=1417

</details>

<details>
<summary>2021.3.11 IoT</summary>

+ [x] 开始学日NAS
+ [ ] 今天装台式机消耗大量体力，摸个鱼

</details>

<details>
<summary>2021.3.12 出题</summary>

+ [x] 用Windows的minifilter出了道题，可惜mac的沙箱pwn不能出，先留一手

</details>

</details>

<details>
<summary>2021.3.13 IoT</summary>

+ [x] 日NAS本当上手，熟悉一下cgi的流程以及工作行为

</details>

<details>
<summary>2021.3.14 出题&怀疑人生</summary>

+ [x] 校队要招新，设计了几个水题给学弟学妹们玩，时间有点紧，质量也不算特别高，之前在学的macos sandbox逃逸找个机会出掉
+ [x] 心态有点爆炸，完全不知道以后该干什么，下午学着学着突然想到：这种没有产出的学习究竟到什么时候可以结束啊；几个知名的实验室暑假hc直接被应届生挤爆，又不是很想去小公司打杂搬砖。当然最好还是暑假可以回玄武实习，总之有没有机会这个学期依旧要all in学习，，，烦躁，迷茫

</details>

<details>
<summary>2021.3.15 准备考试</summary>

+ [x] 准备考试。

</details>

<details>
<summary>2021.3.16 考试</summary>

+ [x] 考试。

</details>

<details>
<summary>2021.3.17 macOS，Objective-c</summary>

+ [x] 学完了oc的语法，看xnu源码基本没有问题
+ [x] 分析了国内外多篇mac security文章，对整个行业发展timeline有自己的见解 

</details>

<details>
<summary>2021.3.18 更新notion上的macOS笔记</summary>

+ [x] 更新笔记
+ [x] 其他的什么也没干， 

</details>

<details>
<summary>2021.3.19-21 CVE-2016-1757</summary>

+ [x] 学习了CVE-2016-1757的成因，前置知识，exp调试，漏洞总结到笔记
  + [ ] 扩展研究待整理。
+ [x] 去武大看樱花，好！ 

</details>

<details>
<summary>2021.3.22-23 CVE-2016-1757</summary>

+ [x] CVE-2016-1757第一轮学习结束
  + [x] 漏洞原因
  + [x] side effect
  + [x] 背后的系统机制
  + [x] 补丁分析
+ [x] 研究了整型提升/溢出相关的问题     

</details>

<details>
<summary>2021.3.24 Pwn</summary>

+ [x] 刷pwn   

</details>

<details>
<summary>2021.3.25 Pwn & 翻译</summary>

+ [x] 刷pwn   
+ [x] 翻译书籍 

</details>

<details>
<summary>2021.3.26-28 Pwn & fuzz macOS</summary>

</details>

<details>
<summary>2021.3.29 fuzz macOS</summary>

</details>

<details>
<summary>2021.3.30 - 4.8 </summary>

+ [x] 调整
+ [x] 做了校赛的pwn
+ [x] macOS停滞
+ [x] Windows内核架构加深理解，复习cpp
+ [x] 投了华为的奇点实验室，准备面试
+ [x] 准备培训     

</details>

<details>
<summary>2021.4.9-11 </summary>

+ [x] angr
+ [x] 准备培训  

</details>

<details>
<summary>2021 4-6月 </summary>

+ [x] 因为接二连三的事情忙得晕头转向，又因为后来开始熟悉用notion去记笔记因此停更了本日志
+ [x] 总的来说，这个学期算是比较认真学习了学校的内容，数学能力提升了！但也因为紧张的课程安排，以及比较差的作息导致没有什么时间去做安全，相当于摸了大几周或者几个月的鱼，检讨
+ [x] 开发能力算是有所增强，包括在期末阶段写完了好久之前遗留的csapp proxy lab，用较多的C++ feature完成一些比较复杂的课程设计，阅读了不少开源工具的源码
+ [x] 此外拿到了华为奇点实验室的实习offer，全依赖于上个学期的学习成果，，，（这学期过的真的不行啊，btw，从玄武离职仍然是我做的最愚蠢的决定，在学校除了消磨时间和学不到东西外，也没什么了，如果和2020年下半年学习速度相比的话，2021上半年无疑是落后了很多，检讨，只有自己变强才可以做心仪的工作以及有更多的选择
+ [x] 选择仍然做Windows Kernel，面试的时候和大佬沟通后理解：什么有意思，有价值去研究什么。（已经没有什么好怕的了bushi），因此再次认真阅读内核相关的资料，有了许多新的发现和理解
+ [x] 培训收入可观
+ [ ] 没有脱单，失败 
+ [ ] 想到再更新，下次详细更新应该是准备入职前的高强度学习了，，先摆平期末。

</details>