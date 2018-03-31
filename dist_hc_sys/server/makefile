CC       = gcc
INCLUDES = -I ./inc -I ../conn/inc -I ../lib/cjson/inc -I/usr/include/
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = -L ../lib/cjson -L ../conn/src -L/usr/lib/x86_64-linux-gnu/

LIBS 	 = -lcjson -lmysqlclient  

SRCS      = ./src/server.c ./src/clientoperation.c ./src/sqldbop.c
OBJS      = ./src/server.o ./src/clientoperation.o ./src/sqldbop.o ../conn/src/connexch.o 
TARGETS   = server 

all:$(TARGETS) 

$(TARGETS): $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS) $(LDFLAGS) $(LDPATH) $(LIBS)

clean:
		rm -rf ./src/*.o $(TARGETS)
