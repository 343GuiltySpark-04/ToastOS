
CFLAGS = -Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -Ibootboot -Isrc

SRCDIR   = src
RESDIR   = .
OBJDIR   = obj
BINDIR   = bin
TMPDIR	 = tmp

SOURCES  := $(wildcard $(SRCDIR)/*.c)
RESOURCES  := $(wildcard $(RESDIR)/*.psf)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
RESOURCEOBJECTS := $(RESOURCES:$(RESDIR)/%.psf=$(OBJDIR)/%.o)

all: makedirs kernel iso

makedirs:
	rm -Rf bin
	rm -Rf tmp
	mkdir -p bin
	mkdir -p obj

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	x86_64-elf-gcc $(CFLAGS) -mno-red-zone -c $< -o $@ 

$(RESOURCEOBJECTS): $(OBJDIR)/%.o : $(RESDIR)/%.psf
	x86_64-elf-ld -r -b binary -o $@ $< 

kernel: makedirs $(OBJECTS) $(RESOURCEOBJECTS)
	x86_64-elf-ld -nostdlib -nostartfiles -T $(SRCDIR)/link.ld $(OBJECTS) $(RESOURCEOBJECTS) -o $(BINDIR)/mykernel.x86_64.elf
	x86_64-elf-strip -s -K mmio -K fb -K bootboot -K environment $(BINDIR)/mykernel.x86_64.elf
	x86_64-elf-readelf -hls $(BINDIR)/mykernel.x86_64.elf >$(BINDIR)/mykernel.x86_64.txt

bootdir:
	mkdir -p $(TMPDIR)/sys
	cp $(BINDIR)/mykernel.x86_64.elf $(TMPDIR)/sys/core
	tar -czf INITRD tmp/*

	rm -Rf boot

	rm -Rf $(BINDIR)/*.img
	rm -Rf $(BINDIR)/*.iso
	rm -Rf $(BINDIR)/*.cdr
	rm -Rf $(BINDIR)/*.bin
	rm -Rf $(BINDIR)/*.vdi

	mkdir -p boot/EFI/BOOT
	mkdir -p boot/BOOTBOOT

	cp bootboot/bootboot.efi boot/EFI/BOOT/BOOTX64.EFI
	cp bootboot/bootboot.bin boot/BOOTBOOT/LOADER
	mv INITRD boot/BOOTBOOT/INITRD
	
iso-osx:
	sh makeisoosx.sh
	
iso-linux:
	sh makeisolinux.sh

clean:
	rm $(OBJDIR)/*.o bin/*.elf bin/*.txt $(BINDIR)/*.img
	rm -Rf tmp

.PHONY: all clean bootdir iso-osx iso-linux makedirs
