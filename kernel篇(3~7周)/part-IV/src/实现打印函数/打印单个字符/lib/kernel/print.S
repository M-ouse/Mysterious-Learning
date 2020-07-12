TI_GDT equ 0
RPL0 equ 0
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

[bits 32]
section .text
; ----------------- put_char -----------------
; 把栈中的一个字符写入光标所在处
; --------------------------------------------
global put_char ; 全局变量，外部可调用
put_char:
	pushad ; 备份环境
	; 保证gs中为正确的视频段选择子
	; 为保险起见，每次打印时都为gs赋值
	mov ax, SELECTOR_VIDEO ; 不能直接把立即数送入段寄存器
	mov gs, ax
	
; 获取当前光标位置，25个字符一行，一共80行，从0行开始
; 先获得高8位
	mov dx, 0x03d4  ; 索引寄存器
	mov al, 0x0e    ; 用于提供光标位置的高8位
	out dx, al
	mov dx, 0x03d5  ; 通过读写数据端口0x3d5来获得或设置光标位置
	in  al, dx      ; 得到了光标位置的高8位
	mov ah, al
	
	; 在获取低8位光标
	mov dx, 0x3d4
	mov al, 0x0f
	out dx, al
	mov dx, 0x3d5
	in  al, dx
	; 将16位完整的光标存入bx
	mov bx, ax
	; 下面这行是在栈中获取待打印的字符
	mov ecx, [esp + 36] ; pushad压入4x8=32字节
						; 加上主函数4字节返回地址
	cmp cl, 0xd			; 回车CR是0x0d，换行LF是0x0a
	jz .is_carriage_return
	cmp cl, 0xa
	jz .is_line_feed
	
	cmp cl, 0x8			; BS(backspace)的asc码是8
	jz .is_backspace
	jmp .put_other

.is_backspace:
	;;;;;;;;;;;;;;;;;; 对于backspace的一点说明 ;;;;;;;;;;;;;;;;;;
	; 当为 backspace 时，光标前移一位
	; 末尾添加空格或空字符0
	dec bx
	shl bx, 1				; 光标左移一位等于乘2
							; 表示光标对应显存中的偏移字节
	mov byte [gs:bx], 0x20	; 将待删除的字节补为0或空格皆可
	inc bx
	mov byte [gs:bx], 0x07
	shr bx, 1
	jmp .set_cursor
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.put_other:
	shl bx, 1      			; 光标位置用2字节表示，将光标值乘2         
						    ; 表示对应显存中的偏移字节
	mov [gs:bx], cl         ; ASCII字符本身
	inc bx
	mov byte [gs:bx], 0x07  ; 字符属性
	shr bx, 1				; 恢复老的光标值
	inc bx					; 下一个光标值
	cmp bx, 2000
	jl .set_cursor			; 若光标值小于2000，表示未写到显存的最后，则去设置新的光标值
							; 若超出屏幕字符数大小(2000)则换行处理
.is_line_feed:				; 是换行符LF(\n)
.is_carriage_return:			; 是回车符
; 如果是CR(\r)，只要把光标移到行首就行了
	xor dx, dx 				; dx是被除数的高16位，清0
	mov ax, bx				; ax是被除数的低16位
	mov si, 80              ; 效访Linux中\n表示下一行的行首
	div si					; 这里\n和\r都处理为下一行的行首
	sub bx, dx				; 光标值减去除80的余数便是取整
							; 以上4行处理\r的代码
.is_carriage_return_end:    ; 回车符CR处理结束
	add bx, 80
	cmp bx, 2000
.is_line_feed_end:			; 若是LF(\n)，将光标移+80便可
	jl .set_cursor
; 屏幕行范围是0~24，滚屏的原理是将屏幕的第1~24行搬运到第0~23行，再将第24行用空格填充
.roll_screen:				; 若超出屏幕大小，开始滚屏
	cld
	mov ecx, 960			; 2000-80=1920个字符要搬运，共1920*2=3820字节
							; 一次搬4字节，共3840/4=960次
	mov esi, 0xc00b80a0		; 第一行行首
	mov edi, 0xc00b8000		; 第0行行首
	rep movsd
	
; 将最后一行填充为空白
	mov ebx, 3840			; 最后一行首字符的第一个字节偏移=1920*2
	mov ecx, 80				; 一行是80字符(160字节)，每次清空1字符(2字节)，一行需要移动80次
	
.cls:
	mov word [gs:ebx], 0x0720 	; 0x0720是黑底白字的空格键
	add ebx, 2
	loop .cls
	mov bx, 1920			  	; 将光标值重置为1920，最后一行的首字符

.set_cursor:
; 将光标设为bx值
; 1.先设置高8位
	mov dx, 0x03d4			  	; 索引寄存器
	mov al, 0x0e				; 用于提供光标位置的高8位
	out dx, al
	mov dx, 0x03d5				; 通过读写数据端口0x3d5来获得或设置光标位置
	mov al, bh
	out dx, al
	
; 2.再设置低8位
	mov dx, 0x3d4
	mov al, 0x0f
	out dx, al
	mov dx, 0x03d5
	mov al, bl
	out dx, al
.put_char_done:
	popad
	ret
