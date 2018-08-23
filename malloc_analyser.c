#include <stdio.h>
#include <stdlib.h>


typedef struct _bucket {
	size_t size;
	FILE *fptr;
	struct _bucket *nxt;
}bucket;

static bucket *head = NULL;

#define file_name "file"
#define MAX_FILE_SZ 256
static bucket* create_bucket(size_t size) 
{	
	bucket *b = NULL;
	char num[10];
	char file[256] = file_name;

	b = (bucket *)malloc(sizeof(*b));
	b->size = size;
	sprintf(num,"%d", size);
	strcat(file, num);

	b->fptr = fopen(file, "r");
	b->nxt = NULL;
}

static bucket* check_bucket(const size_t size)
{
	bucket *iter = head;
	
	while(iter) {
		if(size == iter->size)
			break;

		iter = iter->nxt;	
	}
	return iter;
}

static bucket* get_bucket(const size_t size)
{
	bucket *b;

	if(!(b = check_bucket))
		b = create_bucket(size);
	
	return b;
}

static void append_bucket(char *str, bucket *b)
{
	fscanf(b->fptr, "%s", str);
}


