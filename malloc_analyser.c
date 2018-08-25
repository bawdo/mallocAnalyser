#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * The tool assumes: [MALLOC_DBG], as_id, file-name, function, line, size, ptr
 */
#define MAX_FILENAME (512)
#define MAX_FUNCTION_NAME (512)
#define MAX_SEARCH_STRING (20)
#define TOTAL_STRING_CNT (3)
#define MAX_EACH_NONSTRING_FIELD (5)
#define MAX_NONSTRING_FIELD_COUNT (4)
#define TOTAL_NONSTRING (4)
#define TOTAL_DELIMITER (TOTAL_STRING_CNT + TOTAL_NONSTRING)

#define file_name "file_"
#define DELIM ','

#define STR_MAX ( 								\
		MAX_FILENAME + MAX_FUNCTION_NAME + MAX_SEARCH_STRING + 		\
		 (MAX_EACH_NONSTRING_FIELD * MAX_NONSTRING_FIELD_COUNT) + 	\
		 (TOTAL_DELIMITER)						\
		)								


typedef struct _config{
	char *filename;
	char *new_path;
	char file_prepend[MAX_FILENAME]; /*The name in which file will be generated*/
	FILE *fd;
	unsigned int pos; /*token position*/
	char per_line[STR_MAX];	/*per string */
}config;

typedef struct _bucket {
	unsigned long tok;
	FILE *fptr;
	struct _bucket *nxt;
}bucket;

//static bucket *head = NULL;
//static config c;
static inline void append_bucket_list(bucket **head, bucket *node)
{
	node->nxt = *head;
	*head = node;
}

static bucket *create_bucket(config *c, const unsigned int tok)
{
	bucket *b = NULL;
	char num[10];
	char file[MAX_FILENAME];

	strcpy(file, c->file_prepend);

	b = (bucket *)malloc(sizeof(*b));
	if(!b) {
		fprintf(stderr, "%s\n", "Bucket creation failure");
		exit (-1);
	}

	b->tok = tok;
	sprintf(num,"%d", tok);
	
	strcat(file, num);
	printf("%s: %s\n", "file is", file);

	b->fptr = fopen(file, "w");
	if(!b->fptr) {
		fprintf(stderr, "%s %s\n", "file Error:", file);
		exit (-1);
	}
	b->nxt = NULL;
}

static bucket *check_bucket(bucket **head, const unsigned int tok)
{
	bucket *iter = *head;

	while(iter) {
		if(tok == iter->tok)
			break;

		iter = iter->nxt;
	}

	return iter;
}

static bucket *get_bucket(config *c, bucket **head, const unsigned int tok)
{
	bucket *b;

	if(!(b = check_bucket(head, tok))) {
		b = create_bucket(c, tok);
		append_bucket_list(head, b);
	}

	return b;
}

static void append_bucket(bucket *b, char *str)
{
	printf("AFTER: %s", str);
	fprintf(b->fptr, "%s", str);
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

#define PATH_SEP "/"
static void init_config(char *filename,
			char *new_path,
			char *pref_filename,
			char *pos, 
			config *c)
{
	memset(c, 0x00, sizeof(*c));

	c->filename = filename;
	c->new_path = new_path;
	c->pos = atoi(pos);

	strcpy(c->file_prepend, new_path);
	strcat(c->file_prepend, PATH_SEP);
	strcat(c->file_prepend, pref_filename);

	c->fd = fopen(filename,"r");
	if(!c->fd) {
		fprintf(stderr, "%s %s %s\n", "The file provided", filename, 
							"is not accessible");
		exit(-1);
	}
}

static unsigned int get_nxt_line(config *c)
{
	if(!(fgets(c->per_line, STR_MAX, c->fd)) )
		return 0;
	printf("%s", c->per_line);
	return 1;
}

/*
 * Gets the token from the position. 
 */
static unsigned long parse_tok(config *c)
{
	unsigned int pos = c->pos;
	char tok[MAX_EACH_NONSTRING_FIELD];
	char *tmp = c->per_line;
	unsigned long t;
	unsigned int i = 0;

	while(pos && (*tmp)) {
		if(DELIM == *tmp) {
			pos--;
		}
		tmp++;
	}

	while(*tmp) {
		tok[i] = *tmp++;
		i++;
	        if(DELIM == *tmp)
			break;	
	}
	tok[i] = '\0';
	t = atol(tok);
	return t; 
}

/*
 * Main function that will bucket the captured stream
 * based on the input request.
 */
static void bucket_stream(char *filename,
			  char *new_path,
			  char *pref_filename,
			  char *pos)
{
	config c;
	bucket *head = NULL;

	init_config(filename, new_path, pref_filename, pos, &c);

	while(get_nxt_line(&c)) {
		bucket *b;
		unsigned long tok = 0xFFFFFFFF;

		tok = parse_tok(&c);
		b = get_bucket(&c, &head, tok);
		append_bucket(b, c.per_line);
	}
	clean_bucket_list (head);
}

int main(int argc, char *argv[])
{
	char string[] = "1,2,3,4";
	char tmp[50] = "\0";
	unsigned int tok = 0xFFFFFFFF;
	bucket *b;
	config c;
	bucket *head = NULL;

	bucket_stream("gokul","./g", "hello_", "1");
	
#if 0
	init_config("gokul", "./g", "hello_", "1", &c);

	parse_tok(&c, tmp);

	tok = atoi(tmp);

	b = get_bucket(&c, head, tok);
	if(!b) {
		printf("Error\n");
		exit(-1);
	}
	append_bucket(b, string);
	b = get_bucket(&c, head, tok);
	append_bucket(b, string);

	clean_bucket_list(head);
	printf("%s\n", PATH_SEP);
	//printf("%s\n", tmp);
#endif
	return 0;
}
