kemerald
========

This is a very basic kernel for the x86 architecture. Started few months after I left university during year 2008, I did not have much time to go on with it... I put it on GitHub for backup purpose.

Although the code is far from clean, the kernel itself has some great and advanced features such as:

- Multithreading
- Paging with the `mirroring` support (write to a physical address only knowing its virtual address, which is obviously in a different address space)
- APIC, instead of the old PIC
- IO-APIC with keyboard
- `sysenter` instruction support, instead of the traditionnal `int 0x80`