CC := g++
LIBDIR := -L/usr/libmysql -L/usr/lib64/mysql/   #ubuntu , centos  
LIB := -lpthread -lmysqlclient -lprotobuf -lz 
IDIR := -I/usr/include/mysql
CFLAG := -std=c++11 -g 

serv_src = $(wildcard ./src/*.cc)

serv_obj = $(patsubst %.cc,%.o,$(serv_src))

.PHONY : all
all : serv


serv : $(serv_obj)
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

%.o : %.cc
	$(CC) -c $^ $(CFLAG) $(IDIR)  -o $@


#CLR = $(wildcard ./src/*.o)
#CLR_EXEC = $(wildcard ./serv)
#$(if $(CLR) $(shell rm ./src/*.o ),) 
#$(if $(CLR_EXEC) $(shell rm ./serv ),)

.PHONY:clean
clean:
	rm -rf ./src/*.o
	rm -rf ./serv

