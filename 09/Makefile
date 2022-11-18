
KDIR:=/lib/modules/$(shell uname -r)/build/

obj-m := mountlist.o

all: modules

.DEFAULT:
	$(MAKE) -C $(KDIR) M=$$PWD $@

