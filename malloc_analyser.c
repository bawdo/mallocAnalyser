#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Maximum derived functionality the application will support
 */
#define MAX_FUNC 2
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

/* 
 * FIXME: Functionality not now
 * */
typedef enum {
	T_FREE = 0,
	T_MALLOC,
	T_ALLOC_MAX
}alloc_type;

struct functionality;

typedef struct _config{
	char *filename;
	char *new_path;
	char file_prepend[MAX_FILENAME]; /*The name in which file will be generated*/
	FILE *fd;
	unsigned int pos; /*token position*/
	char per_line[STR_MAX];	/*per string */

	/* Add semantic to the bucketing, 
	 * i.e., what additional functionality is expected on the process of buketing?*/
	unsigned int derived_fun;
	struct functionality *fun;
}config;

struct  functionality {
	unsigned int (*init) (config *c, struct functionality *diese); 
	unsigned int (*f) (config *c, struct functionality *diese);
	unsigned int (*clean) (config *c, struct functionality *diese);
	unsigned int pos;
	void *data;
};

typedef struct _bucket {	unsigned long long tok;
	FILE *fptr;
	struct _bucket *nxt;
}bucket;

typedef enum _function_type {
	ANALYSE = 0,
	STAT,
	BUCKET,
	FUN_MAX
} function_type;

struct functionality func[FUN_MAX] = { };

static inline void append_bucket_list(bucket **head, bucket *node)
{
	node->nxt = *head;
	*head = node;
}

static bucket *create_bucket(config *c, const unsigned long long tok)
{
	bucket *b = NULL;
	char num[50];
	char file[MAX_FILENAME];

	strcpy(file, c->file_prepend);

	b = (bucket *)malloc(sizeof(*b));
	if(!b) {
		fprintf(stderr, "%s\n", "Bucket creation failure");
		exit (-1);
	}

	b->tok = tok;
	sprintf(num,"%lld", tok);

	strcat(file, num);
	// printf("%s: %s\n", "file is", file);

	b->fptr = fopen(file, "w");
	if(!b->fptr) {
		fprintf(stderr, "%s %s\n", "file Error:", file);
		exit (-1);
	}
	b->nxt = NULL;
}

static bucket *check_bucket(bucket **head, const unsigned long long tok)
{
	bucket *iter = *head;

	while(iter) {
		if(tok == iter->tok)
			break;

		iter = iter->nxt;
	}

	return iter;
}

static bucket *get_bucket(config *c, bucket **head, const unsigned long long tok)
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
	//printf("AFTER: %s", str);
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
		        unsigned int fun,
			unsigned int add_pos,	
			config *c)
{
	memset(c, 0x00, sizeof(*c));

	c->filename = filename;
	c->new_path = new_path;
	c->pos = atoi(pos);
	
	//printf("Functionality : %d\n", fun);
	c->derived_fun = fun;
#if 0
	/* FIXME: Needs a better invariance check*/
	if(fun < BUCKET && fun >= 0) {
		c->fun = &func[fun];
		if(add_pos >= 0xFF) {
			fprintf(stderr, "%s", "Wrong params for derived functionality\n");
			exit (-1);
		}
		c->fun->pos = add_pos;
	}
#endif
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

static void analyse_compute (config *c, signed int analysis)
{
	if(analysis) {
		fprintf(stdout, "%s\n", c->filename);
		//fprintf(stdout, "possible leakage in: %s, leak count: %d\n", c->filename, analysis);
	}
	return;
}

static unsigned int get_nxt_line(config *c)
{
	if(!(fgets(c->per_line, STR_MAX, c->fd)) )
		return 0;
	//printf("%s", c->per_line);
	return 1;
}

/*
 * Gets the token from the position. 
 */
static unsigned long parse_tok(config *c)
{
	unsigned int pos = c->pos;
	char tok[MAX_FILENAME];
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
static config c;
static bucket *head = NULL;
static void bucket_stream(char *filename,
			  char *new_path,
			  char *pref_filename,
			  char *pos,
			  unsigned int func,
			  unsigned int add_pos)
{
	signed long analyse = 0;

	init_config(filename, new_path, pref_filename,
		pos, func, add_pos, &c);

	while(get_nxt_line(&c)) {
		unsigned long long tok = 0xFFF;
		
		tok = parse_tok(&c);
		
		switch(c.derived_fun) {
		case ANALYSE:
		{
			unsigned int type = T_ALLOC_MAX;

			if(T_FREE == tok) {
				analyse--;
			}
			else if(T_MALLOC == tok) {
				analyse++;
			}
			else {
				fprintf(stderr, "%s\n", "Unknown alloc Functionality");
				exit (-1);
			}
		}
		break;
		case STAT:
			fprintf(stderr, "%s\n", "Functionality not implemented");
			exit (-1);
		break;
		case BUCKET:
		{
			bucket *b;
			//tok = parse_tok(&c);
			b = get_bucket(&c, &head, tok);
			append_bucket(b, c.per_line);
		}
		break;
		default:
			fprintf(stderr, "%s:%d\n", "Unknown Functionality", c.derived_fun);
			exit (-1);
		break;
		}
#if 0
		if(c.derived_fun < MAX_FUNC) {
			c.f(&c, c.fun);
		}
#endif
	}

	if(BUCKET == c.derived_fun)
		clean_bucket_list (head);
	else if(ANALYSE == c.derived_fun)
		analyse_compute(&c, analyse);


}

/*
 *  i : input file that needs to be processed.
 *  p : The path in which the information needs to be bucketed into files
 *  n : Preferred file name of the generated bucket files
 *  l : Position where Token, that is used for the bucketting the input file.
 *  o : 2 - Simply bucket [default].
 *      0 - Stat the data
 *      1 - Analyse for leak
 *      P.S. Functionality 0 and 1 needs an additional i/p for stating or analysing.
 *  a : >. will take the position of the size in the string in case of "-o 0",
 *         (we consider only stat with size for now)
 *      >. will take the poistion of the type of the operation in the string in case "-o 1"
 *
 */
#define MIN_ARG 4
#define OPT "i:p:n:l:o:"

int main(int argc, char *argv[])
{
	int arg;
	char *filename;
	char *generate_path;
	char *pref_filename;
	char *delim_pos;
	unsigned int functionality = 2; /*setting to default*/
	unsigned int add_pos = 0xFFF;

	//if(argc < MIN_ARG) {
	//	fprintf(stderr, "%s\n", "Enter the right args");
	//	exit (-1);
	//}

	while((arg = getopt(argc, argv, OPT)) != -1) {
		switch(arg) {

		case 'i':
			filename = optarg;
		break;

	unsigned long long tok = 0xFFF;
		case 'p':
			generate_path = optarg;
		break;

		case 'n':
			pref_filename = optarg;
		break;

		case 'l':
			delim_pos = optarg;
		break;
		
		case 'o':
			functionality = atoi(optarg);
			if(functionality > MAX_FUNC) {
				fprintf(stderr, "%s\n", "Wrong option for -o <= 2");
				exit (-1);
			}
		break;
/*
		case 'a':
			add_pos = atoi(optarg);
		break;
*/
		default:
			fprintf(stderr, "%s\n", "Wrong arg");
			exit (-1);
		break;
		}
	}
	//printf("%d\n", STR_MAX);
	bucket_stream(filename, generate_path, pref_filename, 
			delim_pos, functionality, add_pos);

	return 0;
}
