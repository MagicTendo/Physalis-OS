build_all:
	sudo make build_iso
	make build_img

build_img:
	cp ./bin/kernel.bin ./builds/physalis-os.img
	truncate -s 1440k ./builds/physalis-os.img

build_iso:
	cp ./bin/kernel.bin ./iso/boot/

	grub-mkrescue --xorriso=/usr/bin/xorriso --product-name="Physalis OS" --product-version="v0.1.0" -o ./builds/physalis-os.iso ./iso

clean:
	rm -f ./bin/kernel.bin
	rm -f ./bin/kernel.o
	rm -f ./bin/loader.o

compile:
	nasm -f elf32 ./src/boot/loader.asm -o ./bin/loader.o
	$(TARGET)-gcc ./src/libs/*.c -o ./bin/kernel.o -std=gnu99 -ffreestanding -O2 -r -nostdlib -nostartfiles -Wall -Wextra -Werror
	$(TARGET)-gcc -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -nostdlib ./bin/loader.o ./bin/kernel.o -lgcc

debug:
	qemu-system-i386 -accel tcg,thread=single -cpu core2duo -m 512 -no-reboot -drive format=raw,media=cdrom,file=./builds/physalis-os.iso -serial stdio -smp 1 -usb -vga std

emulate:
	qemu-system-i386 -cdrom ./builds/physalis-os.iso