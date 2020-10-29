## 0x1 ASLR(Address Space Layout Randomization)  

借助AELR技术，PE文件每次加载到内存的起始地址都会随机化并且每次运行时相应进程的栈以及堆的起始地址都会变化。简单地说就是EXE文件运行时加载到到进程内存的实际地址不同。因为增加了Windows的安全性，也增加了逆向分析的难度，因此删除ASLR功能可以为PE文件的逆向工作减轻负担。  

## 0x2 删除ASLR功能

### 0x2.1 利用VS创建文件
VS编译文件默认应用ASLR技术，可以在工程Properties-Linker-Advanced-Randomized Base Address中打开或关闭。
![Generate](http://23.91.100.118:8080/images/2019/11/01/generate_file.png)

下图分别是调试开启ASLR和禁用ASLR功能的EXE文件

![Debug ASLR](http://23.91.100.118:8080/images/2019/11/01/aslr.png)  
![Debug ASLR_no](http://23.91.100.118:8080/images/2019/11/01/aslr_no.png)
### 0x2.2 节区信息
采用ASLR技术会比非采用ASLR技术的PE文件节区中多出.reloc节区，如图  
![](http://23.91.100.118:8080/images/2019/11/01/reloc_info.png)
开启了ASLR功能的PE文件，当被加载到内存的时候，该节区被用作重定位的参考，但它不是EXE文件运行的必须部分，因此可将其从EXE文件中删除，但由于DLL文件总是需要重定位，所以在DLL文件中不可将其删除。  
**最重要的是部分是IMAGE_FILE_HEADER\Characteristics与IMAGE_OPTIONAL_HEADER\DLL Characteristics这2个字段**  
![](http://23.91.100.118:8080/images/2019/11/01/header_info.png)
### 0x2.3 删除ASLR功能
使用HxD工具修改ASLR.exe文件，来删除ASLR功能，从上图中可看到IMAGE_OPTIONAL_HEADER\DLL Characteristics中设有IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE(40)标志，删除它即可删除ASLR功能。使用HxD将DLL属性值由8140改为8100(位于偏移146处的DWORD值)  
![](http://23.91.100.118:8080/images/2019/11/01/patch_pre.png)
![](http://23.91.100.118:8080/images/2019/11/01/2019-11-02_013311.png)

保存后在调试器中运行，如下图  
![](http://23.91.100.118:8080/images/2019/11/01/patched.png)
可以看到，已经成功删除ASLR功能。