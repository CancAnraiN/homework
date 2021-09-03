CC = gcc 
t1 = write
t2 = read

all:
	$(CC) $(t1).c -o write
	$(CC) $(t2).c -o read

dep:
	gcc -MM $(t1).c $(t2).c > dep
	
clean:
	rm -f write
	rm -f read
	
