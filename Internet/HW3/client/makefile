CC = g++
TARGET1 = client
FLAG = -std=c++11

all: $(TARGET1).cpp
		$(CC) $(FLAG) -c $(TARGET1).cpp
		$(CC) $(FLAG) -o $(TARGET1) $(TARGET1).o -pthread
clean:
	rm -f $(TARGET1)
	rm -f $(TARGET1).o
