# Code Segments
---

Code segments are used to organize how your compiled binary is sectioned up. The sections exist both as a legacy feature of cpu segmentation but also to create more efficient binary files(bss can be omitted from binary). 

| Name | Description |
|------|-------------|
| text | Segment that holds all of the executable code. |
| rodata | Holds any readonly data that is pre-initialized. |
| data | Holds any static or global variables that are pre-initialized|
| bss | Section starts out as all zeros but is reserved for initialization at runtime. Not included in binary. |

Example Use:
```asm

SECTION .text               ; The following code(asm function) will be placed
                            ; in the code/text segment
increment_my_variable:
    inc [my_variable]       ; Increment value pointed by my_variable
    ret

SEGMENT .data               ; The following label(variable) will be placed 
                            ; in the data segment/section as a global variable
                            ; The term SEGMENT or SECTION are interchangeable.
my_variable: dw 0

```