EXEC = gnet-imx28-usb-downloader
CC = gcc

all: $(EXEC)

clean:
	rm *.o $(EXEC)

$(EXEC): main.o libusb_handler.o
	$(CC) $(LDFLAGS) main.o libusb_handler.o -o $(EXEC) -lusb

main.o: main.c main.h libusb_handler.h
	$(CC) -c $(CFLAGS) -Wall -Wno-format-zero-length main.c

libusb_handler.o: libusb_handler.c libusb_handler.h
	$(CC) -c $(CFLAGS) -Wall libusb_handler.c
