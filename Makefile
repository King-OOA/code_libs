CC = gcc

#路径后面不能有任何空白字符.最好用绝对路径
LIBPATH_A = /home/pz/code/libs/a/
LIBPATH_SO = /home/pz/code/libs/so/

INCLUDE = ./include
vpath %.h ./include
vpath %.c ./src

#OBJS := $(patsubst %.c,%.o, $(wildcard ./sc/*.c))

SOs = libcommon.so libbits.so libadt.so libfileio.so libfiledir.so	\
libstrlist.so  libmem.so libarray.so libseq.so		\
libarith.so libset.so libhashmap.so libatom.so libtools.so #....

As = libcommon.a libbits.a libadt.a libstrlist.a libfiledir.a		\
 libmem.a libfileio.a libarray.a libseq.a libarith.a	\
libset.a libhashmap.a libatom.a libtools.a #....

LIBS = $(SOs) $(As)

# 构建目标文件
a.out: 	main.o $(SOs) $(As)
	$(CC) $< -L$(LIBPATH_A) -static -lstrlist -ladt -lcommon -lfiledir -lfileio -lseq -larray -ltools -larith -lset -lhashmap -latom  -lm -lbits -lmem -lpthread -lrt -o $@ #....
	rm *.o


#构建.o文件
CFLAGS = -g -Wall -fPIC -c -std=c99 -I$(INCLUDE)

main.o: main.c mem.h bits.h adt.h strlist.h filedir.h fileio.h array.h seq.h set.h hashmap.h atom.h tools.h #....
	$(CC) $(CFLAGS) $< -o $@

OBJS = common.o bits.o  strlist.o mem.o  filedir.o fileio.o list.o array.o seq.o arith.o set.o hashmap.o atom.o tools.o #....
$(OBJS): common.h mem.h array.h array_low.h list.h adt.h atom.h 
$(OBJS): %.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

# seq.o: seq.c seq.h array.h stdhs.h mem.h
# 	$(CC) $(CFLAGS) $< -o $@

#通过.o文件,构建.so文件和.a文件 #....
LINK_SO = gcc -shared
LINK_A = ar -rc

libmem.so: mem.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@ 
libmem.a: mem.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libcommon.so: common.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@ 
libcommon.a: common.o
	$(LINK_A) $(LIBPATH_A)$@  $^


libbits.so: bits.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libbits.a: bits.o
	$(LINK_A) $(LIBPATH_A)$@ $^


libseq.so: seq.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libseq.a: seq.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libhashmap.so: hashmap.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libhashmap.a: hashmap.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libatom.so: atom.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libatom.a: atom.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libtools.so: tools.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@ 
libtools.a: tools.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libarith.so: arith.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libarith.a: arith.o
	$(LINK_A) $(LIBPATH_A)$@ $^


libarray.so: array.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libarray.a: array.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libstrlist.so: strlist.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libstrlist.a: strlist.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libset.so: set.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libset.a: set.o
	$(LINK_A) $(LIBPATH_A)$@ $^

# libmakedata.so: makedata.o
# 	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
# libmakedata.a: makedata.o
# 	$(LINK_A) $(LIBPATH_A)$@ $^

libfileio.so: fileio.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libfileio.a: fileio.o
	$(LINK_A) $(LIBPATH_A)$@ $^

libfiledir.so: filedir.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libfiledir.a: filedir.o
	$(LINK_A) $(LIBPATH_A)$@ $^

ADT = list.o seq.o array.o
libadt.so: $(ADT)
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libadt.a: $(ADT)
	$(LINK_A) $(LIBPATH_A)$@ $^



#清理
.PHONY: clean ro rso ra
clean : ro rso ra
	-rm a.out
ro :
	-rm *.o
rso:
	-rm $(LIBPATH_SO)*.so
ra:
	-rm $(LIBPATH_A)*.a


# libmakedata.$(SUF): makedata.o
# 	$(LINK) $^ -o $(LIBPATH)/$@

