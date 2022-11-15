
KDIR:=/lib/modules/$(shell uname -r)/build/

obj-m := char_device_driver.o

all: modules

.DEFAULT:
	$(MAKE) -C $(KDIR) M=$$PWD $@

