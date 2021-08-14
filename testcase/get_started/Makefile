CROSS_COMPILE:= riscv64-linux-
ARCH:= riscv
CC:= $(CROSS_COMPILE)gcc
LD:= $(CROSS_COMPILE)ld
obj-m += demo.o

KERNELDIR = /home/shichang/openeuler-kernel/kernel-m 
PWD := $(shell pwd)

modules:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) modules
	$(CC) test.c -o app
clean:
	rm -f *.o
	rm -f *.symvers
	rm -f *.order
	rm -f *.ko
	rm -f *.mod.c
