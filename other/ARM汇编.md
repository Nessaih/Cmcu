#### `LDR/STR`指令
**LDR指令**
  1. 将符号放入到寄存器，形式：`ldr r0,=addr`。（伪指令）
  2. 把数据从内存加载到寄存器，形式：`ldr r0,[r1]`
  
> 例:
> ```Assembly
> ldr r0, =addr       ; r0 = addr
> ldr r1, [r0]        ; r1 = *r0
> ldr r1, [r0, #4]    ; r1 = *(r0+4)
> ldr r1, [r0, #4]!   ; r1 = *(r0+4)    ; r0 = r0+4
> ldr r1, [r0], #4    ; r1 = *r0        ; r0 = r0+4
> ldr r1, [r0]        ; r1 = *r0
> ldr r1, [r0]        ; r1 = *r0
> ```

**STR指令**
1. 把数据从寄存器保存到内存。
> 例:
> ```Assembly
> str r1, [r0]        ; *r0     = r1
> str r1, [r0, #4]    ; *(r0+4) = r1
> str r1, [r0, #4]!   ; *(r0+4) = r1     ; r0 = r0+4
> str r1, [r0], #4    ; *r0     = r1     ; r0 = r0+4
> ```


