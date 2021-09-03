//server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <math.h>

#define N 256

typedef struct sockaddr SSA;


void decode(char *command)
{
	FILE *in,*out;
	int index,count,freq[N],sum;
	for(int i = 0;i < N;i ++)
	{
		freq[i] = 0;
	}
	float rate;
	char chara,space,newline; // to get useless (and trash) space & newline
	in = fopen("output.txt","r");
	if(!in)
	{
		printf("File open failed.(1)\n");
		exit(1);
	}
	fscanf(in,"%d",&index);		//get all character in "output.txt"
	fscanf(in,"%c",&space);		//(all line with fscanf)
	fscanf(in,"%d",&count);
	fscanf(in,"%c",&space);
	fscanf(in,"%d",&sum);
	fscanf(in,"%c",&newline);
	char word[count];
	for(int i = 0;i < count;i ++) word[i] = '0';//initialize the characters' huffman number
		
	for(int i = 0;i < count;i ++)
	{
		int n,d=0,remainder,i = 0;
		fscanf(in,"%c",&chara);
		fscanf(in,"%c",&space);
		fscanf(in,"%d",&n);
		fscanf(in,"%c",&space);
		fscanf(in,"%d",&freq[(int)chara]);
		fscanf(in,"%c",&space);
		fscanf(in,"%f",&rate);
		fscanf(in,"%c",&newline);
		
		while(n != 0) // To convert binary to decimal
		{
			int s = 1;
			remainder = n % 10;
			n /= 10;
			for(int k = 0; k < i; k++)
				s *= 2;
			d += remainder * s;
			i++;
		}
		word[d] = chara;
		
	}
	fclose(in);

	in = fopen("coded.txt","r");
	out = fopen(command + 7,"w");
	if(!in)
	{
		printf("File open failed.(2)\n");
		exit(1);
	}
	for(int i = 0;i < sum; i++)
	{
		int s,n = 0;
		for(int j = index - 1;j >= 0 ;j--)
		{	
			int d = 1;
			fscanf(in,"%1d",&s);
			for(int k = 0; k < j; k++)
				d *= 2;
			n += s * d;
		}
		fprintf(out,"%c",word[n]);		//start write the things to file
	}
	fclose(in);
	fclose(out);
	printf("Decoded successful with \"%s\"!(",command + 7);
	printf("%s)\n",word);
	
}

int main(int arg, char *argv[])
{
    int ser_socketfd,cli_socketfd;
    struct sockaddr_in ser_addr,cli_addr;
    char* cli_address;
    int cli_port;
    char command [N];
    socklen_t cli_len;
    bzero(command,N);//將command所指向的字串的前N個位元組置為0，包括'\0'

    if((ser_socketfd=socket(AF_INET, SOCK_STREAM, 0) ) < 0)
    {
        printf("Sokcet Error!\n");
        return -1;
    }

    bzero(&ser_addr,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);//在TCP連線中，此處類似於自動獲取ip地址
                                                //在繫結ip時，自動選擇ip地址
    ser_addr.sin_port = htons ( 8990 );
    //將ip地址與套接字繫結
    if((bind(ser_socketfd, (SSA *)&ser_addr, sizeof(ser_addr))) < 0)
    {
        printf("Bind Error!\n");
        return -1;
    }
    //伺服器端監聽
    if(listen(ser_socketfd, 5) < 0)
    {
        printf("Listen Error!\n");
        return -1;
    }
    else
    {
        printf("Listening~\n");
    }

    
    cli_len = sizeof(cli_addr);
    
    //伺服器端接受來自客戶端的連線，返回一個套接字，此套接字為新建的一個，並將客戶端的地址等資訊存入cli_addr中
    //原來的套接字仍處於監聽中
    if((cli_socketfd=accept(ser_socketfd, (SSA *)&cli_addr, &cli_len)) < 0)
    {
        printf("Accept Error!\n");
        exit(1);
    }
    else
    {
        cli_address = inet_ntoa(cli_addr.sin_addr);
        cli_port = ntohs(cli_addr.sin_port);
        printf("A client \"%s\" has connected via port num %d using SOCK_STREAM(TCP). \n", cli_address, cli_port);
    }
        

    while(1)
    {

        //由套接字接收資料時，套接字把接收的資料放在套接字緩衝區，再由使用者程式把它們複製到使用者緩衝區，然後由read函式讀取
        //write函式同理
     
        if(read(cli_socketfd, command, N) < 0)  //read函式從cli_sockfd中讀取N個位元組資料放入command中
        {
            printf("Read Error!\n");
            exit(1);
        }

        if(strncmp(command,"leave", 5) == 0 ) //End the connect 
        {
           printf("The client \"%s\" with port %d has terminated the connection.\n",cli_address,cli_port);
           close(cli_socketfd);
           close(ser_socketfd);
           break; 
        }
        else if(strncmp(command, "send" , 4) == 0) // get "coded.txt" from the client
        {
            FILE *fptr;
            char buffer[N]; //N=256
            char size_buf[1];
            int size;
                    
	        if(read(cli_socketfd,size_buf,sizeof(size_buf)) < 0)
	        {
	            printf("read error.");
	            exit(1);
	        }
            size = (int)size_buf[0];

            fptr = fopen("coded.txt", "w+");
            //bzero(command, N);
            if(!fptr)
            {
                printf("Fail to open the file.\n");
                exit(1);
            }


            //for(int i = size; i > 0; i-=size)
            //{
                
                if(read(cli_socketfd, buffer,sizeof(buffer)) < 0) break;
                //for(int i= 0;i < sizeof(buffer);i++) printf("%c  fuck\n",buffer[i]);
                printf("%s\n",buffer);
                fwrite(buffer,sizeof(char),size,fptr);
                bzero(buffer,sizeof(buffer));
            //}
            
            printf("The client send a file \"%s\" with size of %d bytes.\n",command + 5,size % 8 ? (size/8 +1) : (size/8));
            fclose(fptr);
        }
        else if(strncmp(command, "output", 6) == 0) //Get the "output.txt from the client"
        {
            FILE *fptr;
            char buffer[N]; //N=256
            char size_buf[1];
            int size;
                    
	        if(read(cli_socketfd,size_buf,sizeof(size_buf)) < 0)
	        {
	            printf("read error.");
	            exit(1);
	        }
            size = (int)size_buf[0];

            fptr = fopen("output.txt", "w+");
            //bzero(command, N);
            if(!fptr)
            {
                printf("Fail to open the file.\n");
                exit(1);
            }


            //for(int i = size; i > 0; i-=size)
            //{
                
                if(read(cli_socketfd, buffer,sizeof(buffer)) < 0) break;
                //for(int i= 0;i < sizeof(buffer);i++) printf("%c  fuck\n",buffer[i]);
                printf("%s\n",buffer);
                fwrite(buffer,sizeof(char),size,fptr);
                bzero(buffer,sizeof(buffer));
            //}
            
            printf("The client send a file \"%s\" with size of %d bytes.\n","output.txt",size);
   
            fclose(fptr);
        }
        else if(strncmp(command, "decode", 6) == 0)//(decode "xxx.txt")<---need to type like this
        {										   //decode the file 
            decode(command);
            
        }
        bzero(command,N);
    }
    return 0;
}


