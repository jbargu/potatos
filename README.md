# PotatOS

A simple bash like operating system.

![Cow says moo!](http://s30.postimg.org/6ml8205zl/cowsay.png)

## Installation

### GCC Cross-Compiler and binutils
First we need to install GCC Cross-Compiler. Everything is nicely described on [OSDev](http://wiki.osdev.org/GCC_Cross-Compiler). But since I'm a nice guy, I can give you a bit of a summary. You need to download the latest GCC from [here](https://gcc.gnu.org/mirrors.html), the latest `binutils` from [here](http://ftp.gnu.org/gnu/binutils/). Don't be followed by versioning: 2.9 was way way before 2.26. Then you need to install some dependencies, for Ubuntu the command would be:

```
sudo apt-get install libgmp3-dev libmpfr-dev libmpc-dev gcc-multilib bison libisl-dev
```

Note that these aren't all dependicies but the ones you most likely don't have just lying around on your computer. You'll find out when running `configure` script if anything's missing. When everything is set, just follow the tutorial on OSDev above under **The Build**. You can almost just copy paste everything.

### NASM

Our favourite Intel syntax assembler, am I right?

```
sudo apt-get install nasm
```

### QEMU

We need something for virtualization, QEMU is my weapon of choice.

```
sudo apt-get install qemu
```

### Burning ISO (optional)

I know you want this OS on your desktop machine, who wouldn't ... You might want to remove `-g` flag from `CCFLAGS` to make it smaller.
We'll install GRUB on it, which will later boot our kernel. You need `grub-mkrescue` to do so (if you're working on Linux, you already have it). On Ubuntu we also need:

```
sudo apt-get install grub-pc-bin xorriso
```

The first one are GRUB binaries which will be copied to the ISO, the latter is a program for creating ISO files.

### GDB or any kind of debugger

If you're on Linux, you're fine. If you want to upgrade your experience, use [Voltron](https://github.com/snare/voltron).

Phew, I think that's it.

## Running

Everything revolves around `Makefile`. It has the following rules:

* `run`: compiles (_potatos.bin_) and runs kernel in QEMU.
* `cdrom`: creates ISO image (_potatos.iso_).
* `run-cd`: create ISO images and runs it in QEMU.
* `clean`: removes any files produced by the above commands.

## Usage

Currently there are exactly 4 commands:

* `uname`: prints version of OS,
* `cowsay`: prints cowsay message (no support for additional parameter).
* `rainbow`: _Feel the rainbow!_
* `clock`: prints our current time.

The amount of things you can do just with the above is unthinkable.

## Debugging

There's another make rule for debugging:

```
make debug
```

which pauses QEMU. To break at `kernel_main`, there's a script `.gdbinit` included, which you can run with:

```
gdb -x .gdbinit
```

Then go wild.
