all:fifo

fifo:fifo.c
	gcc -g fifo.c -o fifo

clean:
	rm fifo