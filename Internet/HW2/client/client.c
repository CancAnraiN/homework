//client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#define N 256

typedef struct sockaddr SSA;

void encode(FILE *in,FILE *out_code,FILE *out,char command[N],int size)
{
    int number=0; //how many word(type) do i have
    int bit_num; // how many bit do i need (base on "number")
    int index=0;//bit index
    int sum=0;// how many word(amount) do i have
    int freq[N],word[N];
    int chara;


    in = fopen(command + 5, "r");
    if(!in)
    {
        printf("Fail to open the file.(1)\n");
        exit(1);
    }
    
    for(int i = 0;i < N;i++)
    {
        freq[i]=0;
    }
    while((chara = fgetc(in)) != EOF)
    {
        //printf("%d",chara);ascii 10=LF(Line Feed(\n))
        if(freq[chara] == 0) number++;
        freq[chara]++;
        //printf("%d",freq[chara]);
        sum++;
    }
    bit_num = number;
    while(bit_num!=0)//calculate how many bits
    {
        index++;
        bit_num/=2;
    }
    int str[index],count = number;
    for(int i=0; i<N;i++)
    {
        if(freq[i] != 0)
        {
            word[i] = count - number;
            number--;
        }
        else word[i]=0;
    }
    fseek(in,0,SEEK_SET);//reset in_pointer
    
    out_code = fopen("coded.txt","w");
    out = fopen("output.txt","w");
    if(!out_code)
    {
        printf("Fail to open the file.(2)\n");
        exit(1);
    }
    int word_temp;
    fprintf(out,"%d %d %d\n",index,count,sum);
    
    while((chara = fgetc(in))!=EOF)
    {
        int b = 0;
        for(int i = 0; i < index; i++) str[i] = 0;
        
        word_temp = word[chara];
        while(word_temp > 0)
        {
            str[b] = word_temp % 2;
            b++;
            word_temp = word_temp / 2;
            
        }
		b = index - 1;   
        for(; b >= 0; b--)
        {
            fprintf(out_code,"%d",str[b]);
        }    
    }
    
    for(int i = 0; i < N; i++)
    {
       
        if(freq[i] != 0)
        {
            float compress_rate = (double)freq[i]/sum;
            int b = 0;
            fprintf(out,"%c ",i); // print each character to file
            for(int k=0;k < index; k++) str[k] = 0;
            
            word_temp = word[i];
            
            while(word_temp > 0)
            {
                str[b] = word_temp % 2;
                b++;
                word_temp = word_temp / 2;
            }
            b = index - 1;
            for(; b >= 0; b--)
            {
                fprintf(out,"%d",str[b]);
            }
            fprintf(out," %d %3.1f",freq[i],compress_rate * 100);
			fprintf(out,"\n");
        }
    }
    //==========================================
    int sizeOfFile,sizeOfCode;
    fseek(in,0,SEEK_END);
    sizeOfFile = ftell(in);
    fseek(in,0,SEEK_SET);
    
    fseek(out_code,0,SEEK_END);
    sizeOfCode = ftell(out_code);
    fseek(out_code,0,SEEK_SET);
    //==============get file size===============
    
    int Codesize = sizeOfCode % 8 ? (sizeOfCode / 8) + 1 : sizeOfCode / 8;
    float ratio = (1-((double)sizeOfCode / (double)(sizeOfFile*8)))*100;
    fclose(in);
    fclose(out_code);
    fclose(out);
    
    printf("Original file size: %d(%d bits)\n",sizeOfFile,sizeOfFile*8);
    printf("Coded file size: %d(%d bits)\n",Codesize,sizeOfCode);
    printf("ratio: %f%c\n", ratio,'%');
 

}

int main(int argc, char *argv[])
{
    int socket_fd;
    char command[N];
   
    struct sockaddr_in addr;

    while(1)
    {
        printf("ftp>");
        //fgets函式從stdin流中讀取N-1個字元放入commd中
        if(fgets(command,N,stdin) == NULL)
        {
            printf("Fgets Error!\n");
            return -1;
        }
        command[strlen(command) - 1]='\0';    //fgets函式讀取的最後一個字元為\n，此處將其替換為'\0'
	
        if(strncmp(command,"link",4) == 0) //比較兩個字串前4個位元組，若相等則返回0
        {
            
            if((socket_fd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                printf("Socket Error!\n");
                exit(1);
            }
            bzero(&addr, sizeof(addr));     //將＆addr中的前sizeof（addr）位元組置為0，包括'\0'
            addr.sin_family = AF_INET;      //AF_INET代表TCP／IP協議
            addr.sin_addr.s_addr = inet_addr(command + 5); //將點間隔地址轉換為網路位元組順序
            addr.sin_port = htons(8990);    //轉換為網路位元組順序
            
            if(connect(socket_fd, (SSA *)&addr, sizeof(addr)) < 0)
            {
                printf("Connect Error!\n");
                exit(1);
            }
            else
            {
                printf("The server with IP address \"%s\" has accepted your connection \n", command + 5);
            }
        }
	    else if(strncmp(command, "leave",5) == 0)
        {
            if(write(socket_fd,command,N) < 0)
            {
                //printf("Write error!\n");
                exit(1);
            }

            printf("Bye bye.\n");
            close(socket_fd);
            exit(0);   //結束程序
        }
	    else if(strncmp(command, "send" , 4) == 0)
        {
            FILE *fptr,*fptr_code,*fptr_out;
            char buffer[N]; //N=256
            int size;
            char size_buf[1];
            char size_ori[1];
            //--------------------------------------------------------------
            const char *wday[]={"SUN","MON","TUE","WEN","THU","FRI","SAT"};
            time_t timep;
            struct tm *p;
            time(&timep);
            p=localtime(&timep);   
            //------------------------------------------------------for TIME
            if(write(socket_fd,command,N) < 0)
            {
                printf("Write error!\n");
                exit(1);
            }
            
            printf("\n");
            encode(fptr,fptr_code,fptr_out,command,size);


            fptr = fopen("coded.txt", "r");

            if(!fptr)
            {
                printf("Fail to open the file.\n");
                exit(1);
            }
            
            //=====================================================
            fseek(fptr,0,SEEK_END);
            size = ftell(fptr);
            fseek(fptr,0,SEEK_SET);
            int sizeTobytes = size % 8 ? (size/8 +1) : (size/8);
            size_ori[0] = (char)size;
            size_buf[0] = (char)sizeTobytes;
            //=================get file size=======================
            
            
            while(!feof(fptr))
            {
                bzero(buffer,N);
                fread(buffer, sizeof(char), sizeof(buffer), fptr);
                //printf("%s %d\n",buffer,size);
                
                
                write(socket_fd,size_ori,sizeof(size_ori));
                
                if(write(socket_fd,buffer,sizeof(buffer)) < 0)
                {
                    printf("file write error.\n");
                    fclose(fptr);
                    exit(1);
                }
                
            }
            printf("Done.Had already send a file \"%s\"(coded) with size of %d bytes.\n",command +5,sizeTobytes );
            printf("Time to upload:%d/%d/%d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
            printf(" %s %d:%d:%d\n\n",wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
            fclose(fptr);
        }
        else if(strncmp(command, "output", 6) == 0)
        {
            FILE *fptrr;
            char buffer[N]; //N=256
            int size;
            char size_buf[1];
            //--------------------------------------------------------
            const char *wday[]={"SUN","MON","TUE","WEN","THU","FRI","SAT"};
            time_t timep;
            struct tm *p;
            time(&timep);
            p=localtime(&timep);   
            //-----------------------------------------------for TIME
            if(write(socket_fd,command,N) < 0)
            {
                printf("Write error!\n");
                exit(1);
            }

            printf("\n");
            fptrr = fopen("output.txt", "r");

            if(!fptrr)
            {
                printf("Fail to open the file.\n");
                exit(1);
            }
            
            //=========================================================
            fseek(fptrr,0,SEEK_END);
            size = ftell(fptrr);
            fseek(fptrr,0,SEEK_SET);
            size_buf[0] = (char)size;
            //==================get file size==========================
            
            
            while(!feof(fptrr))
            {
                bzero(buffer,N);
                fread(buffer, sizeof(char), sizeof(buffer), fptrr);
                printf("%s \n",buffer);
                
                
                write(socket_fd,size_buf,sizeof(size_buf));
                
                if(write(socket_fd,buffer,sizeof(buffer)) < 0)
                {
                    printf("file write error.\n");
                    fclose(fptrr);
                    exit(1);
                }
            }
            printf("Done.Had already send a file \"%s\" with size of %d bytes.\n","output.txt",size);
            printf("Time to upload:%d/%d/%d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
            printf(" %s %d:%d:%d\n\n",wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
            fclose(fptrr);
        }
        else if(strncmp(command, "decode", 6) == 0) //"decode "xxx.txt"<--- need to type command like this"
        {
            if(write(socket_fd,command,N) < 0)
            {
                //printf("Write error!\n");
                exit(1);
            }
            
        }
        else if(strncmp(command, "help",4) == 0)
        {
            printf("\n=------------- Welcome to use the Ftp ----------------=\n");
            printf("|                                                     |\n");
            printf("|  help :Dispaly All command for you                  |\n");
            printf("|                                                     |\n");
            printf("|  link \"IP ADDRESS\": To make a connect to server     |\n");
            printf("|                                                     |\n");
            printf("|  send \"FILE\": Send encode file \"coded.txt\" to server|\n");
            printf("|                                                     |\n");
            printf("|  output : send file \"output.txt\" to server          |\n");
            printf("|                                                     |\n");
            printf("|  decode \"FILE\": Make server decode the file         |\n");
            printf("|                                                     |\n");
            printf("|  leave : Quit the Ftp                               |\n");
        }
        else
        {
            printf("Command Is Error!Please Try Again!\n");
        }
        
        bzero(command,N);

    }

    return 0;
}


