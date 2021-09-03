#ifndef _MY_MM_H_INCLUDED_
#define _MY_MM_H_INCLUDED_
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
void *mymalloc(size_t size);
void myfree(void *ptr);
void *myrealloc(void *ptr, size_t size);
void *mycalloc(size_t nmemb, size_t size);

#endif
