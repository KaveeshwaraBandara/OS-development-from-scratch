GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-exceptions -fno-rtti -fno-leading-underscore -fno-builtin
ASPARAMS = --32
LDPARAMS = -m elf_i386

objects = loader.o gdt.o port.o kernel.o

%.o: %.cpp
	g++ $(GPPPARAMS) -c $< -o $@

%.o: %.s
	as $(ASPARAMS) -o $@ -c $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/mykernel.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Kernel" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o mykernel.iso iso
	rm -rf iso

run: mykernel.iso
	-VBoxManage controlvm myos poweroff || true
	sleep 1
	VBoxManage startvm myos --type gui

clean:
	rm -f *.o mykernel.bin mykernel.iso
	rm -rf iso
