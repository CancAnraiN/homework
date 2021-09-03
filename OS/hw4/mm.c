#include "mm.h"
#define NALLOC 1024 


union header
{ 
	struct
	{
		union header *next_ptr; 
		size_t size; 
	} s;

	long x; 
};

typedef union header Header;

static Header base; 
static Header *freep = NULL; 


static Header *morecore(size_t new_units)
{
	char *p;    
	Header *upper_ptr;
	
	if (new_units < NALLOC)
		new_units = NALLOC;
	
	p = sbrk(new_units * sizeof(Header));
	
	if (p == (char *) -1) 
		return NULL;
	

	upper_ptr = (Header *)p;
	upper_ptr->s.size = new_units;
	

	myfree((void *)(upper_ptr+1));
	
	return freep;
}

void *mymalloc(size_t size)
{
	Header *p, *prevp;
	Header *morecore(size_t);
	
	size_t new_units; 
	
	new_units = (size + sizeof(Header)-1) / sizeof(Header) + 1; 

	
	if ((prevp = freep) == NULL)
	{ 
		base.s.next_ptr = freep = prevp = &base;
		base.s.size = 0;
	}

	for (p = prevp->s.next_ptr ; ; prevp = p, p = p->s.next_ptr)
	{
		
		if(p->s.size >= new_units)
		{
			if (p->s.size == new_units) 
				prevp->s.next_ptr = p->s.next_ptr;
			else   
			{
				p->s.size -= new_units;
				p += p->s.size;
				p->s.size = new_units;
			}
	
			freep = prevp;
			return (void *)(p+1);
		}

		
		if (p == freep)
			if ((p = morecore(new_units)) == NULL) 
				return NULL;
	}
}

void myfree(void *ptr)
{
	Header *bp, *p;

	bp = (Header *)ptr - 1;         

	for (p = freep ; !(bp > p && bp < p->s.next_ptr) ; p = p->s.next_ptr)
	{
		if (p >= p->s.next_ptr && (bp > p || bp < p->s.next_ptr))
			break; 
	}
	
	
	if (bp + bp->s.size == p->s.next_ptr)
	{ 
		bp->s.size += p->s.next_ptr->s.size;
		bp->s.next_ptr = p->s.next_ptr->s.next_ptr;
	}
	else
		bp->s.next_ptr = p->s.next_ptr;
	
	
	if (p + p->s.size == bp)
	{
		p->s.size += bp->s.size;
		p->s.next_ptr = bp->s.next_ptr;
	}
	else
		p->s.next_ptr = bp;
	
	freep = p;
}

void *myrealloc(void *ptr, size_t size)
{
	Header *bp;
	size_t new_units;

	if(ptr == NULL)             
		return mymalloc(size);

	bp = (Header *)ptr - 1;      
	new_units = (size + sizeof(Header) - 1) / sizeof(Header) + 1;  

	if(bp->s.size >= new_units)
		return ptr;

	void *new_ptr = mymalloc(size);

	if(new_ptr == NULL)
		return NULL;

	memcpy(new_ptr, ptr, (bp->s.size - 1) * sizeof(Header));

	myfree(ptr);

	return new_ptr;
}

void *mycalloc(size_t nmemb, size_t size)
{
	size_t all = nmemb * size;
	void *new_ptr = mymalloc(all);
	if(new_ptr == NULL)
		return NULL;
	memset(new_ptr,0,all);
	return new_ptr;
}
