CC := g++
LIBDIR := -L/usr/libmysql
LIB := -lpthread -lmysqlclient
IDIR := -I/usr/include/mysql
CFLAG := -std=c++11 -g

SRC = $(wildcard  ../src/*.cpp)

ALLSRC = $(filter-out ../src/main.cpp, $(SRC))

ALLSRC += $(wildcard ./*main.cpp)

ALLOBJ = $(patsubst %.cpp,%.o,$(ALLSRC) )

.PHONY : all
all : ThreadPool MutexLock Timestamp Thread Logging LogFile AsyncLogging

AsyncLogging : $(filter ./%eg.AsyncLogging.main.o ../src/%.o   , $(ALLOBJ) )
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

LogFile : $(filter ./%eg.LogFile.main.o ../src/%.o   , $(ALLOBJ) )
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

Thread : $(filter ./%eg.Thread.main.o ../src/%.o   , $(ALLOBJ) )
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

Logging : $(filter ./%eg.Logging.main.o ../src/%.o   , $(ALLOBJ) )
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

Timestamp : $(filter ./%eg.Timestamp.main.o ../src/%.o   , $(ALLOBJ) )
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

ThreadPool : $(filter ./%eg.ThreadPool.main.o ../src/%.o   , $(ALLOBJ) )
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

MutexLock : $(MutexLock_OBJ)$(filter ./%eg.MutexLock.main.o ../src/%.o   , $(ALLOBJ))
	$(CC) $^ $(LIBDIR) -o $@ $(LIB) 

%.o : %.cpp
	$(CC) -c $^ $(IDIR)  -o $@  $(CFLAG)

#ThreadPool_OBJ  =  $(filter %eg.ThreadPool.main.o ../src/%.o   , $(ALLOBJ) )
#MutexLock_OBJ  = $(filter %eg.MutexLock.main.o ../src/%.o   , $(ALLOBJ))




.PHONY:clean
clean:
	rm $(CLR)



#Debug
.PHONY:file
file:
	@echo $(MutexLock_OBJ)
	@echo $(ThreadPool_OBJ)
	@echo $(filter %eg.ThreadPool.main.o ../src/%.o   , $(ALLOBJ) )

CLR := $(filter-out %.cpp %Makefile ,  $(wildcard ../src/*.o , ./*))


