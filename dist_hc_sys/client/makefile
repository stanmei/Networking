CC       = gcc
INCLUDES = -I ./inc -I ../conn/inc -I ../lib/cjson/inc
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = -L ../lib/cjson -L ../conn/src

LIBS 	 = -lcjson

SRCS      = ./src/client.c ./src/connserver.c ./src/userlogin.c ./src/authenuser.c 
OBJS      = ./src/client.o ./src/connserver.o ./src/userlogin.o ./src/authenuser.o ../conn/src/connexch.o 
TARGETS   = client 

all:$(TARGETS) 

$(TARGETS): $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS) $(LDFLAGS) $(LDPATH) $(LIBS)

clean:
		rm -rf ./src/*.o $(TARGETS)
