CC := g++
LIBDIR := -L/usr/libmysql
LIB := -lpthread -lmysqlclient
IDIR := -I/usr/include/mysql

serv_src := $(wildcard ./src/*.cpp)

serv_obj := $(patsubst %.cpp,%.o,$(serv_src))

.PHONY : all
all: serv

serv : $(serv_obj)
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) -std=c++11
%.o:%.c
	$(CC) -c $^ $(IDIR)  -o $@ -std=c++11


.PHONY:clean
clean:
	rm ./src/*.o 
	rm ./serv 

