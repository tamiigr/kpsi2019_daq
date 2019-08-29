# Makefile for babildrv.c

TARGET = babildrv

#CC=i386-linux26-gcc
obj-m = $(TARGET).o
KERNEL_VERSION = $(shell uname -r)
#KERNEL_VERSION = 2.6.34.1
PWD = $(shell pwd)


default: 
	rm -f babildrv.ko
	make -C /lib/modules/$(KERNEL_VERSION)/build SUBDIRS=$(PWD) modules


clean:
	rm -f $(TARGET).ko $(TARGET).o $(TARGET).mod.*
