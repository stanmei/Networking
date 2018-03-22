C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = 
TARGET1  = client_tcp 
TARGET2  = server_tcp_iter 
TARGET3  = server_tcp_mpr 
TARGET4  = server_tcp_mth 
TARGET5  = server_tcp_ppr 
TARGET6  = server_tcp_pth 
OBJ1      = client_tcp.o 
OBJ2      = server_tcp_iter.o 
OBJ3      = server_tcp_mpr.o 
OBJ4      = server_tcp_mth.o 
OBJ5      = server_tcp_ppr.o 
OBJ6      = server_tcp_pth.o 

all:	$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6)

iter:	$(TARGET1) $(TARGET2)
mpr:	$(TARGET1) $(TARGET3)
mth:	$(TARGET1) $(TARGET4)
ppr:	$(TARGET1) $(TARGET5)
pth:	$(TARGET1) $(TARGET6)

$(TARGET1):	$(OBJ1) 
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJ1) $(LDPATH) $(LDFLAGS)

$(TARGET2):	$(OBJ2) 
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJ2) $(LDPATH) $(LDFLAGS) 

$(TARGET3):	$(OBJ3) 
	$(CC) $(CFLAGS) -o $(TARGET3) $(OBJ3) $(LDPATH) $(LDFLAGS) 

$(TARGET4):	$(OBJ4) 
	$(CC) $(CFLAGS) -o $(TARGET4) $(OBJ4) $(LDPATH) $(LDFLAGS) 
$(TARGET5):	$(OBJ5) 
	$(CC) $(CFLAGS) -o $(TARGET5) $(OBJ5) $(LDPATH) $(LDFLAGS) 

$(TARGET6):	$(OBJ6) 
	$(CC) $(CFLAGS) -o $(TARGET6) $(OBJ6) $(LDPATH) $(LDFLAGS) 
clean:
		rm -rf *.o $(TARGET1) $(TARGET2)
