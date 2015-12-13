EXTRA_CFLAGS += -mfpu=neon
#EXTRA_CFLAGS += -D__KERNEL__ -DKERNEL -DCONFIG_KEXEC -DCONFIG_KEXEC_JUMP -march=armv7-a -mtune=cortex-a9
EXTRA_CFLAGS += -D__KERNEL__ -DKERNEL -DCONFIG_KEXEC -march=armv7-a -mtune=cortex-a9

# Make this match the optimisation values of the kernel you're
# loading this into. Should work without changes, but it seems to
# crash on the Nexus One and Droid Pro. If you're compiling on an 
# Evo 4g, set this value to 1 and change the EXTRA_CFLAGS value to
# something appropriate to your phone
# EXTRA_CFLAGS += -O0

ARCH		= arm
KERNEL = /mnt/mnt/kernel-build
CONFIG = ford_cyanogenmod_defconfig
CROSS_COMPILE = /mnt/mnt/cyanogen/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-

CPPFLAGS	=  -I$(KERNEL)/

obj-m += kexec_load.o
kexec_load-objs := kexec.o machine_kexec.o sys.o relocate_kernel.o \
	rodata.o

all: module push

module:
	ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KERNEL)/ M=$(PWD) modules

clean:
	ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KERNEL)/ M=$(PWD) clean
	rm -f *.order

prepare:
	ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KERNEL)/ $(CONFIG)
	ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KERNEL)/ modules_prepare

kernel_clean:
	ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KERNEL)/ mrproper
	ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make -C $(KERNEL)/ clean

push:
	adb push kexec_load.ko /mnt/sdcard/
