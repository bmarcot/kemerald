PROJECT=	Kemerald
CC=		gcc
AS=		as
LD=		ld
INCL=		includes
CFLAGS=		-Wall -ffreestanding -nostdinc -mno-stack-arg-probe -I $(INCL) -Winline
LDMAP=		kernel.map
LDSCRIPT= 	link.ld
LDFLAGS=	-T $(LDSCRIPT) --print-map > $(LDMAP)
EXEC=		kernel.img
ASRC=		$(wildcard src/*.s)
CSRC=		$(wildcard src/*.c)
AOBJ=		$(ASRC:.s=.o)
COBJ=		$(CSRC:.c=.o)
OBJ=		$(AOBJ) $(COBJ)
BPRODS=		boot.bin boot.o boot.out
KPRODS=		kernel.o kernel.bin

all: $(EXEC)

kernel.img: kernel.bin
	cat $^ /dev/zero | dd of=$@ bs=4096 count=2880
	cp kernel.img ../qemu-0.9.1-windows/
	cp kernel.img ../Qemu-0.12.2/

kernel.bin: kernel.o
	objcopy -R .note -R .comment -S -O binary $^ $@

kernel.o: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.s
	$(AS) -o $@ $<

clean:
	@rm -rf src/*.o	src/*~ includes/*~ autogen/*~ *~ $(BPRODS) $(KPRODS)

distclean: clean
	@rm -f $(EXEC) $(LDMAP)

dist: distclean
	@tar czf ../_$(PROJECT).tgz ../$(PROJECT)
	@mv ../_$(PROJECT).tgz ../$(PROJECT)_`date "+%d%b%Y-%kh%M"`.tgz

vdi:
	@/cygdrive/c/Program\ Files/Sun/VirtualBox/VBoxManage.exe			\
	convertfromraw /cygdrive/d/USERS/Bmarcot/Dossier_personnel/Kemerald/kernel.bin	\
	/cygdrive/d/USERS/Bmarcot/Dossier_personnel/Kemerald/vm/kemerald.vdi		\
	--format VDI --variant Fixed

