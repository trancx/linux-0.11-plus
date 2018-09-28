#
# if you want the ram-disk device, define this to be the
# size in blocks.
#
RAMDISK =  #-DRAMDISK=512

AS86	=as86 -0 -a
LD86	=ld86 -0

AS	=as
LD	=ld
#-Map System.map -s -x
LDFLAGS	= -Map System.map -s -x -m elf_i386 -Ttext 0 #--verbose
CC	=gcc -m32 -march=i386 $(RAMDISK)
CFLAGS	= -Wall -O1 -fomit-frame-pointer #-D __DEBUG__

CPP	=cpp -nostdinc -Iinclude

#
# ROOT_DEV specifies the default root-device when making the image.
# This can be either FLOPPY, /dev/xxxx or empty, in which case the
# default of /dev/hd6 is used by 'build'.
#
ROOT_DEV= #FLOPPY 

ARCHIVES=kernel/kernel.o mm/mm.o fs/fs.o
DRIVERS =kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a
MATH	=kernel/math/math.a
LIBS	=lib/lib.a

.c.s:
	$(CC) $(CFLAGS) \
	-nostdinc -Iinclude -S -o $*.s $<
.s.o:
	$(AS) -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) \
	-nostdinc -Iinclude -c -o $*.o $<

all:	Image

Image: boot/bootsect boot/setup tools/system tools/build
	objcopy -O binary -R .note -R .comment tools/system tools/kernel
	tools/build boot/bootsect boot/setup tools/kernel $(ROOT_DEV) > Image
	rm tools/kernel -f
	sync

disk: Image
	dd bs=8192 if=Image of=/dev/fd0

tools/build: tools/build.c
	$(CC) $(CFLAGS) \
	-o tools/build tools/build.c

boot/head.o: boot/head.s
	$(CC) $(CFLAGS) -I./include -traditional -c boot/head.s
	mv head.o boot/

tools/system:	boot/head.o init/main.o \
		$(ARCHIVES) $(DRIVERS) $(MATH) $(LIBS)
	$(LD) $(LDFLAGS) boot/head.o init/main.o \
	$(ARCHIVES) \
	$(DRIVERS) \
	$(MATH) \
	$(LIBS) \
	-o tools/system 
	# nm tools/system| grep -v '\(compiled\)\|\(\.o$$\)\|\( [aU] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)'| sort > System.map 

kernel/math/math.a: FORCE 
	(cd kernel/math; make)

kernel/blk_drv/blk_drv.a: FORCE
	(cd kernel/blk_drv; make)

kernel/chr_drv/chr_drv.a: FORCE
	(cd kernel/chr_drv; make)

kernel/kernel.o: FORCE
	(cd kernel; make)

mm/mm.o: FORCE
	(cd mm; make)

fs/fs.o: FORCE
	(cd fs; make)

lib/lib.a: FORCE
	(cd lib; make)

FORCE: 
# 当前目录下没有FORCE这个文件，所以FORCE肯定被更新， 上述的FORCE都会被认为更新，进而都进子目录执行make
# If a rule has no prerequisites or recipe, and the target of the rule is a nonexistent file, 
# then make imagines this target to have been updated whenever its rule is run.	    --manual

boot/setup: boot/setup.s
	$(AS86) -o boot/setup.o boot/setup.s
	$(LD86) -s -o boot/setup boot/setup.o

boot/bootsect:	boot/bootsect.s
	$(AS86) -o boot/bootsect.o boot/bootsect.s
	$(LD86) -s -o boot/bootsect boot/bootsect.o

tmp.s:	boot/bootsect.s tools/system
	(echo -n "SYSSIZE = (";ls -l tools/system | grep system \
		| cut -c25-31 | tr '\012' ' '; echo "+ 15 ) / 16") > tmp.s
	cat boot/bootsect.s >> tmp.s


clean:
	rm -f Image System.map tmp_make core boot/bootsect boot/setup
	rm -f init/*.o tools/system tools/build boot/*.o
	(cd mm;make clean)
	(cd fs;make clean)
	(cd kernel;make clean)
	(cd lib;make clean)

backup: clean
	(cd .. ; tar cf - linux | compress16 - > backup.Z)
	sync

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in init/*.c;do echo -n "init/";$(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	(cd fs; make dep)
	(cd kernel; make dep)
	(cd mm; make dep)

### Dependencies:
init/main.o: init/main.c include/unistd.h include/sys/stat.h \
  include/sys/types.h include/sys/times.h include/sys/utsname.h \
  include/utime.h include/time.h include/linux/tty.h include/termios.h \
  include/linux/sched.h include/linux/head.h include/linux/fs.h \
  include/linux/mm.h include/signal.h include/asm/system.h \
  include/asm/io.h include/stddef.h include/stdarg.h include/fcntl.h
	$(CC) $(CFLAGS) -nostdinc -Iinclude init/main.c -c -o init/main.o

