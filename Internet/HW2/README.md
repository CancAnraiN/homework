# Network Applications Programming - Homework2 (FTP program with data compression)
## Made By B073040011 柯冠宇

### Bulid:
+ Use `make` in both directory **client** and **server** to build this program .

### content:
+ Client's file:
  + **client.c**
  + **xxx.txt**(name by user)
    + The file which want to send
  + **coded.txt**
    + store the binary char after convert the `char` in `xxx.txt`
  + **output.txt**
    + Store each character's binary code 、 frequency 、   
  + **makefile**

+ Server's file:
  + **server.c**
  + **coded.txt**(Come from client)
    + store the binary char after convert the `char` in `xxx.txt`
  + **output.txt**(Come from client)
  + **xxx.txt**(name by user)
    + After convert `code.txt` by `output.txt`,you can store the origin data which is `xxx.txt` in client
  + **makefile**

### Description:
+ 1:
    Use `./client` to start the program.
    Use `./server` to activate server.
+ 2:
    client use `link` to connect with server .
+ 3:
    Then use `send xxx.txt` to send a encode file `coded.txt` to the server and create `output.txt`.
+ 4:
    Use `output` to send `output.txt`.
+ 5:
    Use `decode aaa.txt` to make server decode.Then server get the origin file store in `xxx.txt`.

~I hate huffman~
