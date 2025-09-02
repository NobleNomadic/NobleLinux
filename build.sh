#!/bin/bash
set -e

echo "[*] Noble Linux Build Script"

# --- Paths ---
ROOT=$(pwd)
ROOTFS="$ROOT/rootfs"
ROOTFSSRC="$ROOT/rootfssrc"

INIT_SRC="$ROOTFSSRC/init/src/init.c"
INIT_BIN="$ROOTFS/bin/init"

SHELL_SRC="$ROOTFSSRC/shell/sh.c"
SHELL_BIN="$ROOTFS/bin/sh"

LS_SRC="$ROOTFSSRC/ls/ls.c"
LS_BIN="$ROOTFS/bin/ls"

CLEAR_SRC="$ROOTFSSRC/clear/clear.c"
CLEAR_BIN="$ROOTFS/bin/clear"

INITRAMFS="$ROOTFSSRC/init/build/initramfs.cpio"
KERNEL="$ROOT/linux/arch/x86/boot/bzImage"

# --- Ensure required directories exist ---
echo "[*] Preparing rootfs structure..."
mkdir -p "$ROOTFS/bin" "$ROOTFS/dev" "$ROOTFS/etc" "$ROOTFS/home" "$ROOTFS/proc" "$ROOTFS/sys" "$ROOTFS/tmp"
mkdir -p "$ROOTFS/lib/x86_64-linux-gnu" "$ROOTFS/lib64"
chmod 1777 "$ROOTFS/tmp"

# --- Compile init ---
echo "[*] Compiling init..."
gcc -Os -o "$INIT_BIN" "$INIT_SRC"
strip "$INIT_BIN"

# === USER PROGRAMS ===
# --- Compile shell ---
if [ -f "$SHELL_SRC" ]; then
  echo "[*] Compiling shell..."
  gcc -Os -o "$SHELL_BIN" "$SHELL_SRC"
  strip "$SHELL_BIN"
fi

# --- Compile ls ---
if [ -f "$LS_SRC" ]; then
  echo "[*] Compiling ls..."
  gcc -Os -o "$LS_BIN" "$LS_SRC"
  strip "$LS_BIN"
fi

# --- Compile clear ---
if [ -f "$CLEAR_SRC" ]; then
  echo "[*] Compiling clear..."
  gcc -Os -o "$CLEAR_BIN" "$CLEAR_SRC"
  strip "$CLEAR_BIN"
fi

# --- Copy shared libraries ---
echo "[*] Copying shared libraries..."
cp /lib/x86_64-linux-gnu/libc.so.6 "$ROOTFS/lib/x86_64-linux-gnu/"
cp /lib64/ld-linux-x86-64.so.2 "$ROOTFS/lib64/"

# --- Create initramfs ---
echo "[*] Creating initramfs..."
mkdir -p "$(dirname "$INITRAMFS")"
cd "$ROOTFS"
find . -print0 | cpio --null -ov --format=newc > "$INITRAMFS"
cd "$ROOT"

# --- Boot with QEMU ---
echo "[*] Booting kernel with QEMU..."
qemu-system-x86_64 \
  -kernel "$KERNEL" \
  -initrd "$INITRAMFS"

