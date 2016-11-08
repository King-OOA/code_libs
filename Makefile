CC = gcc

#路径后面不能有任何空白字符.最好用绝对路径
LIBPATH_A = /home/pz/code/libs/a/
LIBPATH_SO = /home/pz/code/libs/so/

INCLUDE = ./include
vpath %.h ./include
vpath %.c ./src

#OBJS := $(patsubst %.c,%.o, $(wildcard ./sc/*.c))

SOs = libcommon.so libbits.so libadt.so libpatset.so libmakedata.so libmem.so #....
As = libcommon.a libbits.a libadt.a libpatset.a libmakedata.a libmem.a #....
LIBS = $(SOs) $(As)

# 构建目标文件
a.out: 	main.o $(SOs) $(As)
	$(CC) $< -L$(LIBPATH_A) -static -lpatset -ladt -lmakedata -lcommon -lm -lbits -lmem -o $@ #....
	rm *.o


#构建.o文件
CFLAGS = -g -Wall -fPIC -c -std=c99 -I$(INCLUDE)

main.o: main.c mem.h bits.h adt.h patset.h makedata.h #....
	$(CC) $(CFLAGS) $< -o $@

OBJS = common.o bits.o list.o makedata.o patset.o mem.o #....
$(OBJS): common.h mem.h
$(OBJS): %.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@



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


libpatset.so: patset.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libpatset.a: patset.o
	$(LINK_A) $(LIBPATH_A)$@ $^


libmakedata.so: makedata.o
	$(LINK_SO) $^ -o $(LIBPATH_SO)$@
libmakedata.a: makedata.o
	$(LINK_A) $(LIBPATH_A)$@ $^


ADT = list.o
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

