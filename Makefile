.PHONY: clean

 obj-m += one.o
 
 KERNEL_VERSION := $(shell uname -r)
 MODULE_INSTALL_LOCATION := /lib/modules/$(KERNEL_VERSION)/
 all:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules

99-one.rules:
	ls 99-one.rules

install: 99-one.rules
	cp 99-one.rules /etc/udev/rules.d/
	mkdir -p $(MODULE_INSTALL_LOCATION)/misc
	cp one.ko $(MODULE_INSTALL_LOCATION)/misc
	echo "misc/one" > $(MODULE_INSTALL_LOCATION)/modules.dep
	depmod -a
	modprobe one
clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean
