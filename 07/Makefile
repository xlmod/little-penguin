
KDIR:=/lib/modules/$(shell uname -r)/build/

obj-m := debug_fs.o

all: modules

.DEFAULT:
	$(MAKE) -C $(KDIR) M=$$PWD $@

