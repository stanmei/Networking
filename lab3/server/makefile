C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = 
TARGET1  = server_tcp 
OBJ1      = server_tcp.o 

all:	$(TARGET1)

$(TARGET1):	$(OBJ1) 
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(LDPATH) $(LDFLAGS)

clean:
		rm -rf *.o rx_*.txt $(TARGET1) 
