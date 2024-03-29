#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#define color "\x1b[1;33;1;1m"  //yellow
#define rst "\x1b[0;m"

int main(int argc, const char *argv[])
{    
    int fd = open("text", O_RDONLY);

    if(fd == -1)
    {
        printf("file open error!\n");
        exit(EXIT_FAILURE);
    }

    struct stat text_info = {0};
    fstat( fd, &text_info );
    int text_len = (int)text_info.st_size;

    char *map = (char *)mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    if(map == MAP_FAILED)
    {
        close(fd);
        printf("mapping error!\n");
        exit(EXIT_FAILURE);
    }

    if(!text_len)
        printf("empty file\n");

    while(text_len)
    {
        char c;
		printf("==============================================\n");
        printf("read(R,r)   or  quit(Q,q)\n");
        printf(": ");
        scanf("%c", &c);
        getchar();

        if(c == 'q' || c == 'Q')
            break;
        else if(c == 'r' || c == 'R')
		{
        	printf(color);
        	for(int i = 0; i < getpagesize(); ++i)
        	{
            	printf("%c", map[i]);
        	}
        	printf(rst);
        }
		else continue;

    }

    return 0;
}
