

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

typedef unsigned int Sensor;
Sensor *replacePtr = NULL;
typedef unsigned int MsgBuffer;

// [MALLOC_DBG] File, address-id, function, line, type, size, ptr

#define my_malloc(size) (_my_malloc(__FILE__, __FUNCTION__, __LINE__, size))
#define my_free(ptr) (_my_free(__FILE__, __FUNCTION__, __LINE__, ptr))

static void *_my_malloc(const char *file, const char *function, unsigned int line, size_t size) 
{
	pid_t pid = getpid();
	void *ptr = malloc(size);

	fprintf(stdout, "[MALLOC_DBG] %d, %s, %s, %d, %ld, %d, %p\n", pid, file, function, line, size, 1, ptr);

	return ptr;
}

static void _my_free(const char *file, const char *function, unsigned int line, void *ptr)
{
	pid_t pid = getpid();

	fprintf(stdout, "[MALLOC_DBG] %d, %s, %s, %d, %d, %d, %p\n", pid, file, function, line, 0, 0, ptr);
	free(ptr);
}


void toggler(void)
{
   if (replacePtr != NULL) my_free(replacePtr);
   replacePtr = my_malloc(sizeof(Sensor));
}

static MsgBuffer  *manyBuffers[30];
static int numBuf = 0;

void growAndShrink(void)
{
   if (numBuf > 20)
   {
      for (int i = 0; i < 5; i++)
      {
         numBuf--;
         my_free(manyBuffers[numBuf]);
         manyBuffers[numBuf] = NULL;
      }
   }
   else
   {
      for (int i = 0; i < 5; i++)
      {
         manyBuffers[numBuf] = my_malloc(sizeof(MsgBuffer));
         numBuf++;
      }
   }
}

static MsgBuffer *manyBadBuffers[200];
static int numBadBuf = 0;

void growForever(void)
{
   for (int i = 0; i < 5 ; i++)
   {
      manyBadBuffers[numBadBuf] = my_malloc(sizeof(MsgBuffer));
      numBadBuf++;
   }
}



int main(void)
{
   char cbuf;

   for (int i = 0; i < 10; i++)
   {
      growAndShrink();
      //growForever();
      printf("End of iteration %d\n", i);
   }

   return 0;
}

