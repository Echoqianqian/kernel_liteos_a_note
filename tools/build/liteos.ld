
// GNU 链接脚本语法 https://sourceware.org/binutils/docs/ld/LD-Index.html
ENTRY(reset_vector) /*指定入口地址*/
INCLUDE board.ld // > ram 指的是要大于 ram这个地址, ram在board.ld中定义
/* 这是脚本中最重要的命令了，所有的LD脚本都会有这个命令，用来指定如何将输入文件映射到输出文件等等 */
SECTIONS
{
     _start = .;
    .set_sysinit_set : {
        __start_set_sysinit_set = ABSOLUTE(.);
        KEEP (*(.set_sysinit_set))//在连接命令行内使用了选项–gc-sections后，连接器可能将某些它认为没用的section过滤掉，此时就有必要强制连接器保留一些特定的 section，可用KEEP()关键字达此目的。
        __stop_set_sysinit_set = ABSOLUTE(.);
    } > ram

    .got ALIGN(0x4) : { *(.got.plt) *(.got) } > ram

    .gcc_except_table ALIGN (0x8) : { . = .; } > ram  .gcc_except_table : { KEEP(*(.gcc_except_table*)) }
    .exception_ranges ALIGN (0x8) : ONLY_IF_RW { *(.exception_ranges .exception_ranges*) } > ram

    .ARM.extab ALIGN(0x4) : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > ram

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    .ARM.exidx ALIGN(0x8) : { __exidx_start = .; *(.ARM.exidx* .gnu.linkonce.armexidx.*) ;__exidx_end = .;} > ram

    /* text/read-only data */
    .text ALIGN(0x1000) : {
        __text_start = .;
        *(.text* .sram.text.glue_7* .gnu.linkonce.t.*)
    } > ram

    .rel.text : { *(.rel.text) *(.rel.text.*) *(.rel.gnu.linkonce.t*) } > ram
    .rela.text : { *(.rela.text) *(.rela.text.*) *(.rela.gnu.linkonce.t*) } > ram
    .rel.data : { *(.rel.data) *(.rel.data.*) *(.rel.gnu.linkonce.d*) } > ram
    .rela.data : { *(.rela.data) *(.rela.data.*) *(.rela.gnu.linkonce.d*) } > ram
    .rel.rodata : { *(.rel.rodata) *(.rel.rodata.*) *(.rel.gnu.linkonce.r*) } > ram
    .rela.rodata : { *(.rela.rodata) *(.rela.rodata.*) *(.rela.gnu.linkonce.r*) } > ram
    .rel.got : { *(.rel.got) } > ram
    .rela.got : { *(.rela.got) } > ram
    .rel.ctors : { *(.rel.ctors) } > ram
    .rela.ctors : { *(.rela.ctors) } > ram
    .rel.dtors : { *(.rel.dtors) } > ram
    .rela.dtors : { *(.rela.dtors) } > ram
    .rel.init : { *(.rel.init) } > ram
    .rela.init : { *(.rela.init) } > ram
    .rel.fini : { *(.rel.fini) } > ram
    .rela.fini : { *(.rela.fini) } > ram
    .rel.bss : { *(.rel.bss) } > ram
    .rela.bss : { *(.rela.bss) } > ram
    .rel.plt : { *(.rel.plt) } > ram
    .rela.plt : { *(.rela.plt) } > ram
    .rel.dyn : { *(.rel.dyn) } > ram

    .dummy_post_text : {
        __text_end = .;
    } > ram

    .rodata ALIGN(0x1000) : {
        __rodata_start = .;
        __kernel_init_level_0 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.0.*)));
        __kernel_init_level_1 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.1.*)));
        __kernel_init_level_2 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.2.*)));
        __kernel_init_level_3 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.3.*)));
        __kernel_init_level_4 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.4.*)));
        __kernel_init_level_5 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.5.*)));
        __kernel_init_level_6 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.6.*)));
        __kernel_init_level_7 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.7.*)));
        __kernel_init_level_8 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.8.*)));
        __kernel_init_level_9 = ABSOLUTE(.);
        KEEP(*( SORT (.rodata.init.kernel.9.*)));
        __kernel_init_level_10 = ABSOLUTE(.);
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        __exc_table_start = .;
        KEEP(*(__exc_table))
        __exc_table_end = .;
    } > ram

    /*
     * extra linker scripts tend to insert sections just after .rodata,
     * so we want to make sure this symbol comes after anything inserted above,
     * but not aligned to the next section necessarily.
     */
    .dummy_post_rodata : {
        _hdf_drivers_start = .;
        KEEP(*(.hdf.driver))
        _hdf_drivers_end = .;
        __rodata_end = .;
    } > ram

    .data ALIGN(0x1000) : {
        /* writable data  */
        __ram_data_start = .;
        __vdso_data_start = LOADADDR(.data);
        KEEP(*(.data.vdso.datapage))
        . = ALIGN(0x1000);
        KEEP(*(.data.vdso.text))
        . = ALIGN(0x1000);
        __vdso_text_end = .;
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(0x4);
        KEEP(*( SORT (.liteos.table.*)));
    } > ram

    .ctors : ALIGN(0x4) {
        __ctor_list__ = .;
        KEEP (*(.ctors .init_array))
        __ctor_end__ = .;
    } > ram
    .dtors : ALIGN(0x4) {
        __dtor_list__ = .;
        KEEP (*(.dtors .fini_array))
        __dtor_end__ = .;
    } > ram
    /*
     * extra linker scripts tend to insert sections just after .data,
     * so we want to make sure this symbol comes after anything inserted above,
     * but not aligned to the next section necessarily.
     */
    .dummy_post_data : {
        __ram_data_end = .;
    } > ram

    .user_init USER_INIT_VM_START : ALIGN(0x1000) {//开始地址设为 USER_INIT_VM_START
        . = ALIGN(0x4);
        __user_init_load_addr = LOADADDR(.user_init);//应用程序的加载地址
        __user_init_entry = .;//应用程序的入口地址
        KEEP(libuserinit.O (.user.entry))
        KEEP(libuserinit.O (.user.text))
        KEEP(libuserinit.O (.user.rodata))
        . = ALIGN(0X4);
        __user_init_data = .;//设置数据段开始位置 __user_init_data
        KEEP(libuserinit.O (.user.data))
        . = ALIGN(0X4);
        __user_init_bss = .;//将当前位置给__user_init_bss
        KEEP(libuserinit.O (.user.bss))
        . = ALIGN(0x1000);
        __user_init_end = .;/	
    } > user_ram AT > ram

    __user_init_size = __user_init_end - __user_init_entry;

    /* unintialized data (in same segment as writable data)*/
    .bss : {
        . = ALIGN(0x800); //当前位置按 0x800对齐
        __int_stack_start = .; //将当前位置给__int_stack_start
        *(.int_stack);
        . = ALIGN(0x4);
        KEEP(*(.bss.prebss.*))
        . = ALIGN(0x8);
        __bss_start = .;	//将当前位置给__bss_start,将所有的目标*(.bss .bss.*) .. 链接到 .bss中
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(0x8);
        __bss_end = .;
    } > ram

    . = ALIGN(0x1000);
    _end = .;
    /* mmu temp page table(sys aviliable mem is start with __bss_end) */
    . = ALIGN(0x4000);
    __mmu_ttlb_begin = .;

    /* Strip unnecessary stuff */
    /DISCARD/ 0 : { *(.comment .note) } > ram
}
