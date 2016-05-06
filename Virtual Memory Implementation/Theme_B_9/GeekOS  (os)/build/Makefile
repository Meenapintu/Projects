UNAMEALL=$(shell uname -a)
MACHTYPE?=$(shell uname -m)
OSTYPE?=$(shell uname -s | tr A-Z a-z)

HAS_STD_VGA=$(shell qemu --help | grep std-vga)
ifneq "$(findstring std-vga,$(HAS_STD_VGA))" ""
	QEMU_VGA_OPT?=-std-vga
else
	QEMU_VGA_OPT?=-vga std
endif

QEMU_NO_KQEMU?=$(shell qemu --help | grep -e -no-kqemu | awk '{print $$1}') -k en-us
RANDOM_PORT=1234

HAS_GDB_OPT=$(shell qemu --help | grep -e -gdb)
ifneq "$(findstring -gdb,$(HAS_GDB_OPT))" ""
  GDB_OPT = -gdb tcp::$(RANDOM_PORT)
else
  GDB_OPT = -s -p $(RANDOM_PORT)
endif



ifeq ($(findstring csic.umd.edu,$(UNAMEALL)),csic.umd.edu)
	BIOSLOCATION:=-L /usr/local/qemu-0.10.5/share/qemu
	QEMU=/usr/local/bin/qemu -no-kqemu
	QEMU_VGA_OPT=-vga std 
	include Makefile.linux
else
	OSTYPE ?= linux
	ifeq ($(MACHTYPE),x86_64)
		ifeq ($(wildcard Makefile.$(OSTYPE).$(MACHTYPE)),Makefile.linux.x86_64)
	   	    include Makefile.$(OSTYPE).$(MACHTYPE)
	        else
		    include Makefile.$(OSTYPE)
		endif
	else 
	  	include Makefile.$(OSTYPE)
	endif
endif
