
KDIR:=/lib/modules/$(shell uname -r)/build/

obj-m := useless_module.o

all: modules

.DEFAULT:
	$(MAKE) -C $(KDIR) M=$$PWD $@

