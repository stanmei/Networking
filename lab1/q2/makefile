C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES)
LDFLAGS  = 
LDPATH   = 
TARGET   = daytime_client_udp 
OBJ      = daytime_client_udp.o 

all:	$(TARGET)
$(TARGET):	$(OBJ) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDPATH) $(LDFLAGS)

clean:
		rm -rf *.o $(TARGET)
