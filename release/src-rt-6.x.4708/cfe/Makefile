export PATH := $(PATH):$(shell pwd)/../toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin
export LD_LIBRARY_PATH := $(shell pwd)/../toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib

all: ea6900 r7000 ws880

ea6900 EA6900:
	make -C build/broadcom/bcm947xx clean
	make -C build/broadcom/bcm947xx EA6900=1

r7000 R7000:
	make -C build/broadcom/bcm947xx clean
	make -C build/broadcom/bcm947xx R7000=1

ws880 WS880:
	make -C build/broadcom/bcm947xx clean
	make -C build/broadcom/bcm947xx WS880=1

clean:
	make -C build/broadcom/bcm947xx clean
