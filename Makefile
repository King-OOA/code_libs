SUF = so
CC = gcc

LINKSO = gcc -g -shared -std=c99
LINKA = ar -rc
LINK = $(LINKSO)

INCLUDE = ./include
CFLAGS = -g -Wall -c -std=c99 -fPIC -I$(INCLUDE)

#路径后面不能有任何空白字符.
LIBPATH = /usr/local/lib/

vpath %.h $(INCLUDE) #../Adaptive/include
vpath %.c ./src

#OBJS := $(patsubst %.c,%.o, $(wildcard ./sc/*.c))


SOS = libcommon.so libbits.so libadt.so libpatset.so libmem.so #....

a.out: 	main.o $(SOS)
	$(CC) $< -lpatset -ladt  -lcommon -lm -lbits -lmem -o $@ #....
	rm *.o

main.o: main.c mem.h bits.h adt.h patset.h #....
	$(CC) $(CFLAGS) $< -o $@


#构建.o文件
OBJS = common.o bits.o list.o patset.o mem.o #....
$(OBJS): common.h mem.h
$(OBJS): %.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

#通过.o文件,构建.so文件 #....
libmem.$(SUF): mem.o
	$(LINK) $^ -o $(LIBPATH)$@

libcommon.$(SUF): common.o
	$(LINK) $^ -o $(LIBPATH)$@

libbits.$(SUF): bits.o
	$(LINK) $^ -o $(LIBPATH)$@

libpatset.$(SUF): patset.o
	$(LINK) $^ -o $(LIBPATH)$@

ADT = list.o
libadt.$(SUF): $(ADT)
	$(LINK) $^ -o $(LIBPATH)$@

#清理
.PHONY: clean ro rso ra
clean : ro rso ra
	-rm a.out
ro :
	-rm *.o
rso:
	-rm $(SOPATH)/*.so
ra:
	-rm $(APATH)/*.a


# libmakedata.$(SUF): makedata.o
# 	$(LINK) $^ -o $(LIBPATH)/$@

