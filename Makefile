KERNEL 	:= $(shell uname -r)
PWD	:= $(CURDIR)
obj-m 	:= whale.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean
