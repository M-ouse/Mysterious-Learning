## 这是什么？

记录日常学习

备注：1. 前3个月的学习还在整理中以周/月为记录单位，从19/10/2020开始以天为记录单位。  
    2. 满课指的是不得不完成的学校指标，例如8：00上课，22：00下课，课程中间签到、签退；实验课成果现场展示等不能依靠万能的舍友完成的任务。

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

</details>

<details>
<summary>2020.11.01 Windows 进程守护 & 内核实验</summary>

+ [x] [内核实验](https://www.ired.team/)
  + [x] Manipulating ActiveProcessLinks to Hide Processes in Userland
  + [x] Dll injection  
+ [x] [通过过滤请求的方法守护进程](https://bbs.pediy.com/thread-168023.htm)
  
</details>

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
