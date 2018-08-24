#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _config{
	char *filename;
	unsigned int pos;
	char *new_path;
}config;

typedef struct _bucket {
	unsigned int tok;
	FILE *fptr;
	struct _bucket *nxt;
}bucket;

static bucket *head = NULL;
static config c;

#define file_name "file_"
#define MAX_FILE_SZ 256
#define DELIM ','

static inline void append_bucket_list(bucket **head, bucket *node)
{
	node->nxt = *head;
	*head = node;
}

static bucket *create_bucket(const unsigned int tok) 
{
	bucket *b = NULL;
	char num[10];
	char file[256] = file_name;

	b = (bucket *)malloc(sizeof(*b));
	b->tok = tok;
	sprintf(num,"%d", tok);
	strcat(file, num);
	printf("%s\n", file);
	
	b->fptr = fopen(file, "w");
	if(!b->fptr) {
		printf("file Error\n");
		exit -1;
	}
	b->nxt = NULL;
}

static bucket *check_bucket(const unsigned int tok)
{
	bucket *iter = head;

	while(iter) {
		if(tok == iter->tok)
			break;

		iter = iter->nxt;	
	}

	return iter;
}

static bucket *get_bucket(const unsigned int tok)
{
	bucket *b;

	if(!(b = check_bucket(tok))) {
		b = create_bucket(tok);
		append_bucket_list(&head, b);		
	}

	return b;
}

static void append_bucket(bucket *b, char *str)
{
	fprintf(b->fptr, "%s\n", str);	
}

static void clean_bucket_list(bucket *head)
{
	while(head) {
		bucket *tmp = head;

		head = head->nxt;
		fclose(tmp->fptr);
		free(tmp);
	}
}


static void get_line(config *c, )
{

}

static void parse_tok(char *str, config *c, char *tok)
{
	unsigned int pos = c->pos;
	char *tmp = str;

	while(pos && (*tmp)) {

		if(DELIM == *tmp) {
			pos--;
		}
		tmp++;
	}

	while(*tmp) {
		*tok++ = *tmp++;
	        if(DELIM == *tmp)
			break;	
	}
	*tok = '\0';	
}



int main()
{
	char string[] = "1,2,3,4";
	char tmp[50] = "\0";
	unsigned int tok = 0xFFFFFFFF;
	bucket *b;

	c.pos = 0;

	parse_tok(string, &c, tmp);
	tok = atoi(tmp);

	b = get_bucket(tok);
	if(!b) {
		printf("Error\n");
		return -1;
	}
	append_bucket(b, string);
	b = get_bucket(tok);
	append_bucket(b, string);

	clean_bucket_list(head);
	printf("%s\n", tmp);

	return 0;
}
