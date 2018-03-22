C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = 
TARGET1  = client_udp 
TARGET2  = server_udp_iter 
TARGET3  = server_udp_mpr 
TARGET4  = server_udp_mth 
TARGET5  = server_udp_ppr 
TARGET6  = server_udp_pth 
OBJ1      = client_udp.o 
OBJ2      = server_udp_iter.o 
OBJ3      = server_udp_mpr.o 
OBJ4      = server_udp_mth.o 
OBJ5      = server_udp_ppr.o 
OBJ6      = server_udp_pth.o 

all:	$(TARGET1) $(TARGET2)	$(TARGET3) $(TARGET4)	$(TARGET5) $(TARGET6)

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
