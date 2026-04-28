- extracted the archive from i686-elf-5.2.0-Linux-x86_64.tar.xz into WSL home directory at /home/abhi/opt/cross/i686-elf-5.2.0-Linux-x86_64. The compiler binaries are in /home/abhi/opt/cross/i686-elf-5.2.0-Linux-x86_64/bin, which contains i686-elf-gcc, i686-elf-ld, and the other expected cross-toolchain executables.


``` bash
i686-elf-as boot.s -o boot.o

i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-gcc -T linker.ld -o babluOS -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

if grub-file --is-x86-multiboot babluOS; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

mkdir -p isodir/boot/grub
cp babluOS isodir/boot/babluOS
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o babluOS.iso isodir

```