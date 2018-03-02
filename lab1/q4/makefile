C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES)
LDFLAGS  = -lm
LDPATH   = 
TARGET1  = client_tcp 
TARGET2  = server_tcp 
OBJ1      = client_tcp.o 
OBJ2      = server_tcp.o 

all:	$(TARGET1) $(TARGET2)

$(TARGET1):	$(OBJ1) 
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(LDPATH) $(LDFLAGS)

$(TARGET2):	$(OBJ2) 
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJ2) $(LDPATH) $(LDFLAGS) 

clean:
		rm -rf *.o $(TARGET1) $(TARGET2)
