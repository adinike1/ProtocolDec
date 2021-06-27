all:
	gcc -g -pthread crc16.c linebuf.c list.c  protocol_hal.c link_hal.c protocol_rvc.c main.c -o main.out

clean:
	rm -rf *.o *.out
