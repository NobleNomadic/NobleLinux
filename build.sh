#!/bin/bash
set -e

# Compile code
echo "Compiling binaries..."
gcc -static -o rootfs/bin/init src/init/init.c
gcc -static -o rootfs/bin/sh src/sh/sh.c

# Make sure they're executable
chmod +x rootfs/bin/init rootfs/bin/sh

# Verify binaries
echo "Verifying binaries are statically linked..."
ldd rootfs/bin/init 2>/dev/null && echo "WARNING: init is dynamically linked!" || echo "init: static ✓"
ldd rootfs/bin/sh 2>/dev/null && echo "WARNING: sh is dynamically linked!" || echo "sh: static ✓"

# Create essential directories if they don't exist
mkdir -p rootfs/dev rootfs/proc rootfs/sys rootfs/tmp

# Create uncompressed cpio file only
echo "Creating uncompressed initramfs..."
cd rootfs
find . | cpio -H newc -o > ../initramfs.cpio
cd ..

echo "Initramfs created: $(ls -lh initramfs.cpio)"

# Verify the cpio file can be read
echo "Testing cpio file integrity..."
cpio -tv < initramfs.cpio | head -10

# Run in qemu
echo "Starting QEMU..."
qemu-system-x86_64 \
  -kernel linux/arch/x86/boot/bzImage \
  -initrd initramfs.cpio \
