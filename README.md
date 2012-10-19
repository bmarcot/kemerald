kemerald
========

This is a basic kernel for the x86 architecture. Started few months after I left university during year 2008, I did not have much time to go on with it... I put it on GitHub for backup purpose, yet I am planning to overhaul it in the near future.

Although the code is far from clean, the kernel itself has some great and advanced features such as:

- Spatial partioning, ring0 / ring3, flat memory model to bypass the segmentation, paging enabled with smart use PAE feature to constantly have the kernel address-space mapped into the PDBR. Paging with the `mirroring` feature.
- Memory manager, malloc().
- Multithreading with some IPC, preemptive round-robin scheduler.
- Local-APIC, instead of the old PIC. IPI, MSI support.
- IO-APIC for nice IRQs, with keyboard support!
- Various time sources (TCR, LAPIC timers, ...)
- Supports the `sysenter` and `sysexit` instruction for modern syscalls, instead of the legacy `int 0x80`.
- Smart exception/interrupt routing.
- A lot of assembly wrap-up in inlined functions.
- A few libc functions (printf(), sprintf(), vsprintf()...).

todo
====

- Remove bad file attributes (for files created on Windows).
- Translate all comments from french to english.
- Apply K&R coding style, follow Linux Coding Style rules. Big big work to be done to clean code...
- Add basic data structures (rb-tree, Linux style doubly-linked list).
- Revamp the thread manager (pthread compliant) and the memory manager.