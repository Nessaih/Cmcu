#### `LDR/STR`指令
**LDR:把数据从内存加载到寄存器。**

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

**STR:把数据从寄存器保存到内存。**
> 例:
> ```Assembly
> str r1, [r0]        ; *r0     = r1
> str r1, [r0, #4]    ; *(r0+4) = r1
> str r1, [r0, #4]!   ; *(r0+4) = r1     ; r0 = r0+4
> str r1, [r0], #4    ; *r0     = r1     ; r0 = r0+4
> ```


