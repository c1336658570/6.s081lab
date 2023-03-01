// Physical memory layout

// qemu -machine virt is set up like this,
// based on qemu's hw/riscv/virt.c:
//
// 00001000 -- boot ROM, provided by qemu
// 02000000 -- CLINT
// 0C000000 -- PLIC
// 10000000 -- uart0 
// 10001000 -- virtio disk 
// 80000000 -- boot ROM jumps here in machine mode
//             -kernel loads the kernel here
// unused RAM after 80000000.

// the kernel uses physical memory thus:
// 80000000 -- entry.S, then kernel text and data
// end -- start of kernel page allocation area
// PHYSTOP -- end RAM used by the kernel

// qemu puts UART registers here in physical memory.
// 物理内存布局

// qemu -machine virt 是这样设置的，
// 基于 qemu 的 hw/riscv/virt.c:
//
// 00001000 -- boot ROM，由 qemu 提供
// 02000000 -- CLINT
// 0C000000 -- PLIC
// 10000000 -- UART0
// 10001000 -- virtio disk 
// 80000000 -- boot ROM 在机器模式下跳转到这里
// -kernel 在这里加载内核
// 80000000 之后未使用的 RAM。

// 内核因此使用物理内存：
// 80000000 -- entry.S，然后是内核文本和数据
// end -- 内核页面分配区域的开始
// PHYSTOP -- 内核使用的结束 RAM

// qemu 将 UART 寄存器放在物理内存中。
#define UART0 0x10000000L
#define UART0_IRQ 10

// virtio mmio interface
#define VIRTIO0 0x10001000
#define VIRTIO0_IRQ 1

#ifdef LAB_NET
#define E1000_IRQ 33
#endif

// core local interruptor (CLINT), which contains the timer.
// 内核本地中断器 (CLINT)，其中包含计时器。
#define CLINT 0x2000000L
#define CLINT_MTIMECMP(hartid) (CLINT + 0x4000 + 8*(hartid))
#define CLINT_MTIME (CLINT + 0xBFF8) // cycles since boot.

// qemu puts platform-level interrupt controller (PLIC) here.
// qemu 将平台级中断控制器（PLIC）放在这里。
#define PLIC 0x0c000000L
#define PLIC_PRIORITY (PLIC + 0x0)
#define PLIC_PENDING (PLIC + 0x1000)
#define PLIC_MENABLE(hart) (PLIC + 0x2000 + (hart)*0x100)
#define PLIC_SENABLE(hart) (PLIC + 0x2080 + (hart)*0x100)
#define PLIC_MPRIORITY(hart) (PLIC + 0x200000 + (hart)*0x2000)
#define PLIC_SPRIORITY(hart) (PLIC + 0x201000 + (hart)*0x2000)
#define PLIC_MCLAIM(hart) (PLIC + 0x200004 + (hart)*0x2000)
#define PLIC_SCLAIM(hart) (PLIC + 0x201004 + (hart)*0x2000)

// the kernel expects there to be RAM
// for use by the kernel and user pages
// from physical address 0x80000000 to PHYSTOP.
// 内核期望有 RAM
// 供内核和用户页面使用
// 从物理地址 0x80000000 到 PHYSTOP。
#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)

// map the trampoline page to the highest address,
// in both user and kernel space.
// 将蹦床页面映射到最高地址，
// 在用户和内核空间中。
#define TRAMPOLINE (MAXVA - PGSIZE)

// map kernel stacks beneath the trampoline,
// each surrounded by invalid guard pages.
// 在蹦床下映射内核堆栈，
// 每个都被无效的保护页包围。
#define KSTACK(p) (TRAMPOLINE - (p)*2*PGSIZE - 3*PGSIZE)

// User memory layout.
// Address zero first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap
//   ...
//   USYSCALL (shared with kernel)
//   TRAPFRAME (p->trapframe, used by the trampoline)
//   TRAMPOLINE (the same page as in the kernel)
// 用户内存布局。
// 首先地址为零：
//   文本
//原始数据和bss
// 固定大小的栈
// 可扩展堆
// ...
// USYSCALL（与内核共享）
// TRAPFRAME (p->trapframe, 被蹦床使用)
// TRAMPOLINE（与内核相同的页面）
#define TRAPFRAME (TRAMPOLINE - PGSIZE)
#ifdef LAB_PGTBL
#define USYSCALL (TRAPFRAME - PGSIZE)

struct usyscall {
  int pid;  // Process ID
};
#endif
