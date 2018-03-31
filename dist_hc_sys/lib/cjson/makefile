CC       = gcc
AR  	 = ar

INCLUDES = -I ./inc
CFLAGS   = -g -Wall -fPIC $(INCLUDES) 
LDFLAGS  = -lm
LDPATH   = 

SRCS      = ./src/cJSON.c
OBJS      = cJSON.o
TARGETS   = libcjson.a 

all:$(TARGETS) 

$(TARGETS): $(OBJS) 
	$(AR) rcs $(TARGETS) $(OBJS)

$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) -c $(SRCS)
	
clean:
		rm -rf ./src/*.o $(TARGETS) 
