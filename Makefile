SUF = so
CC = gcc

LINKSO = gcc -O3 -shared -std=c99
LINKA = ar -rc
LINK = $(LINKSO)

INCLUDE = ./include
CFLAGS = -O3 -Wall -c -std=c99 -fPIC -I$(INCLUDE)

LIBPATH = /usr/local/lib/ 	#生成的.so文件,直接放到系统库中

vpath %.h $(INCLUDE) ../Adaptive/include
vpath %.c ./src

#OBJS := $(patsubst %.c,%.o, $(wildcard ./sc/*.c))


SOS = libcommon.so libbinary.so libadt.so libmem.so
LIBS = $(SOS) #-ladt -lcommon -lm -lbinary 

a.out: 	main.o $(SOS)
	$(CC) $(CFLAGS) -ladt -lcommon -lm -lbinary -lmem -o $@
	rm *.o

main.o: main.c makedata.h binary.h adt.h
	$(CC) $(CFLAGS) $< -o $@


#构建.o文件
OBJS = common.o binary.o stack.o queue.o arith.o makedata.o mem.o
$(OBJS): common.h mem.h
$(OBJS): %.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

#通过.o文件,构建.so文件
libmem.$(SUF): mem.o
	$(LINK) $^ -o $(LIBPATH)$@

libcommon.$(SUF): common.o
	$(LINK) $^ -o $(LIBPATH)$@

libbinary.$(SUF): binary.o
	$(LINK) $^ -o $(LIBPATH)$@

ADT = stack.o queue.o
libadt.$(SUF): $(ADT)
	$(LINK) $^ -o $(LIBPATH)$@

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

