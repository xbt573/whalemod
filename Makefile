KERNEL = $(shell uname -r)
obj-m := whale.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean
