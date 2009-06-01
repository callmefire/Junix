#########################  Junix Root Makefile: copyright by Callmefire 2009 #########################

# Global definations

############ Extern Files ############
-include include/config

############ Build info #############

TOPDIR=$(shell pwd)
OS = Junix
VERSION = 0.1
ARCH = ia32

ARCHDIR = arch
LIBDIR = lib
INITDIR = init
INCLUDE_DIR = include
INSTALL_DIR = /boot

CROSS_COMPILE =

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
NM = $(CROSS_COMPILE)nm
CP = cp
RM = rm
MAKE = make

ENTRY_SYMBOL = _start

CFLAGS = -nostdinc -nostdlib -fPIC -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -march=i686 -mtune=pentium4 -mtune=generic -I$(TOPDIR)/$(INCLUDE_DIR) -ffreestanding  -fno-strict-aliasing -fno-common -pipe -freg-struct-return -mpreferred-stack-boundary=2 -fno-omit-frame-pointer -fno-stack-protector

CCAFLAGS = $(CFLAGS)
LDFLAGS = -m elf_i386 --emit-relocs -u $(ENTRY_SYMBOL) -T $(LD_SCRIPT) --start-group
LDFLAGS_SUFFIX = --end-group

LD_SCRIPT = $(TOPDIR)/Junix.lds

export CC LD AS AR NM RM MAKE
export CFLAGS CCAFLAGS LDFLAGS LDFLAGS_SUFFIX

IMAGE = $(TOPDIR)/$(OS)
IMAGE_MAP = $(TOPDIR)/$(OS).map

TARGET = build_in.o
OBJECTS = $(ARCHDIR)/$(ARCH)/$(TARGET)
OBJECTS += $(INITDIR)/$(TARGET)
OBJECTS += $(LIBDIR)/$(TARGET)

SUBDIRS = $(ARCHDIR)/$(ARCH)
SUBDIRS += $(INITDIR)
SUBDIRS += $(LIBDIR)

export OBJECTS

############# Local definations ##################

TAGS = tags
TAGS += cscope.out

##################  Targets ##################

all: $(IMAGE) install

$(IMAGE): build $(OBJECTS)
	@$(LD) -o $@ $(LDFLAGS) $(OBJECTS) $(LDFLAGS_SUFFIX)
	@$(NM) $(IMAGE) | sort > $(IMAGE_MAP)	
	@echo "LD $@"

build:
	@$(foreach dir,$(SUBDIRS),$(MAKE) -s -C $(dir);)

.S.o:
	@$(CC) $(CCAFLAGS) -c $<
	@echo "CC $@"
.c.o:
	@$(CC) $(CFLAGS) -c $<
	@echo "CC $@"

install: $(IMAGE)
	@$(CP) -f $(IMAGE) $(INSTALL_DIR)
	@echo "INSTALL $< --> $(INSTALL_DIR)/"

tags:
	@ctags -R
	@echo "CTAGS ..."
	@cscope -Rkb
	@echo "CSCOPE ..."
	@echo "Done."

clean:
	@$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean;)
	@$(RM) -rf  $(IMAGE) $(IMAGE_MAP)
	@$(RM) -rf `find * -name '*~'`

cleanall: clean
	@$(RM) -f $(TAGS)
