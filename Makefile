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


.PHONY:clean
clean:
	rm ./src/*.o 
	rm ./serv 

