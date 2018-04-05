C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = 
TARGET1  = client_tcp 
OBJ1      = client_tcp.o 

all:	$(TARGET1)

mpr:	$(TARGET1)

$(TARGET1):	$(OBJ1) 
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(LDPATH) $(LDFLAGS)

clean:
		rm -rf *.o rx_*.txt $(TARGET1) 
