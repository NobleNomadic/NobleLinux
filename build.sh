#!/bin/bash
set -e

# --- Paths ---
ROOT=$(pwd)
ROOTFS="$ROOT/rootfs"
INIT_SRC="$ROOT/init/src/init.c"
INIT_BIN="$ROOTFS/bin/init"
INITRAMFS="$ROOT/init/build/initramfs.cpio"
KERNEL="$ROOT/linux/arch/x86/boot/bzImage"

# --- Prep rootfs ---
echo "[*] Preparing rootfs..."
mkdir -p "$ROOTFS"/{bin,etc,home,dev,proc,sys}

# --- Build init ---
echo "[*] Compiling init..."
gcc -static -Os -o "$INIT_BIN" "$INIT_SRC"

# --- Build initramfs ---
echo "[*] Creating initramfs..."
cd "$ROOTFS"
find . -print0 | cpio --null -ov --format=newc > "$INITRAMFS"
cd "$ROOT"

# --- Run kernel with qemu ---
echo "[*] Booting kernel..."
qemu-system-x86_64 \
    -kernel "$KERNEL" \
    -initrd "$INITRAMFS"

