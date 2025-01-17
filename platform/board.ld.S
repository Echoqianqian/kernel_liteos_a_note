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

#include "los_vm_zone.h"
#define TEXT_BASE  KERNEL_VADDR_BASE //代码区为内核起始地址

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

/***********************************************
本文件经过
python build.py ipcamera_hi3516dv300 
编译后生成 board.ld,内容如下:
#define TEXT_BASE  KERNEL_VADDR_BASE //代码区为内核起始地址

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    ram : ORIGIN = 0x40000000, LENGTH = 0x20000000
    sram : ORIGIN = 0x40000000, LENGTH = 0x1000
    user_ram : ORIGIN = 0x1000000, LENGTH = 0x100000
}
SECTIONS
{
    /DISCARD/ : { *(.comment .note) }

    .ram_vectors 0x40000000 : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    } > ram
    __ram_vectors_lma = LOADADDR(.ram_vectors);
}

USER_INIT_VM_START = 0x1000000;
***********************************************/
/* 
https://www.jianshu.com/p/42823b3b7c8e
MEMORY：内存布局,描述板上的存储器位置
语法为
NAME1 [(ATTR)] : ORIGIN = ORIGIN1, LENGTH = LEN2

NAME ：存储区域的名字。（自己可以随意命名）
ATTR ：定义该存储区域的属性。ATTR属性内可以出现以下7 个字符：
	R 只读section
	W 读/写section
	X 可执行section
	A 可分配的section
	I 初始化了的section
	L 同I
	! 不满足该字符之后的任何一个属性的section
ORIGIN ：关键字，区域的开始地址，可简写成org 或o
LENGTH ：关键字，区域的大小，可简写成len 或l
*/
MEMORY
{ //ram,sram为存储区域的名字,可以随意取
    ram : ORIGIN = KERNEL_VADDR_BASE, LENGTH = KERNEL_VADDR_SIZE
    sram : ORIGIN = 0x40000000, LENGTH = 0x1000
    user_ram : ORIGIN = 0x1000000, LENGTH = 0x100000
}
SECTIONS
{
    /DISCARD/ : { *(.comment .note) }

    .ram_vectors TEXT_BASE : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    } > ram
    __ram_vectors_lma = LOADADDR(.ram_vectors);
}

USER_INIT_VM_START = 0x1000000;
