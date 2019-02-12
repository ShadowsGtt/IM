CC := g++
LIBDIR := -L/usr/libmysql
LIB := -lpthread -lmysqlclient
IDIR := -I/usr/include/mysql
CFLAG := -std=c++11

serv_src = $(wildcard ./src/*.cpp)

serv_obj = $(patsubst %.cpp,%.o,$(serv_src))

.PHONY : all
all: serv

serv : $(serv_obj)
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

%.o : %.cpp
	$(CC) -c $^ $(CFLAG) $(IDIR)  -o $@


CLR = $(wildcard ./src/*.o)
CLR_EXEC = $(wildcard ./serv)

.PHONY:clean
clean:
	$(if $(CLR) $(shell rm ./src/*.o ),) 
	$(if $(CLR_EXEC) $(shell rm ./serv ),)

