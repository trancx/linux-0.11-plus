CC	=gcc -m32 -march=i386
#CFLAGS	=-Wall -O -fstrength-reduce -fomit-frame-pointer \
	-finline-functions -nostdinc -I../include
CFLAGS	=-Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -I../include
AS	=as
AR	=ar
LD	=ld
CPP	=gcc -E -nostdinc -I../include
ASFLAGS = -mtune=i386 --32
LDFLAGS	= -m elf_i386

.c.o:
	$(CC) $(CFLAGS) \
	-c -o $*.o $<
.s.o:
	$(AS) $(ASFLAGS) -o $*.o $<
.c.s:
	$(CC) $(CFLAGS) \
	-S -o $*.s $<

OBJS	= memory.o page.o

all: mm.o

mm.o: $(OBJS)
	$(LD) $(LDFLAGS) -r -o mm.o $(OBJS)

clean:
	rm -f core *.o *.a tmp_make
	for i in *.c;do rm -f `basename $$i .c`.s;done

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

### Dependencies:
memory.o: memory.c ../include/signal.h ../include/sys/types.h \
  ../include/asm/system.h ../include/linux/sched.h \
  ../include/linux/head.h ../include/linux/fs.h ../include/linux/mm.h \
  ../include/linux/kernel.h
