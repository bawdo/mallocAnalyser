

#include "malloc.h"

typedef unsigned int Sensor;

Sensor *replacePtr = NULL;

void toggler(void)
{
   if (replacePtr != NULL) Free(replacePtr);
   replacePtr = Malloc(sizeof(Sensor));
}


MsgBuffer *manyBuffers[30];
int numBuf = 0;

void growAndShrink(void)
{
   if (numBuf > 20)
   {
      for (int i = 0; i < 5; i++)
      {
         numBuf--;
         mFree(manyBuffers[numBuf]);
         manyBuffers[numBuf] = NULL;
      }
   }
   else
   {
      for (int i = 0; i < 5; i++)
      {
         manyBuffers[numBuf] = malloc(sizeof(MsgBuffer));
         numBuf++;
      }
   }
}

MsgBuffer *manyBadBuffers[200];
int numBadBuf = 0;

void growForever(void)
{
   for (int i = 0; i < 5 ; i++)
   {
      manyBadBuffers[numBadBuf] = malloc(sizeof(BadBuffer));
      numBadBuf++;
   }
}



int main(void)
{
   char cbuf;

   for (int i = 0; i < 10; i++)
   {
      replacer();
      growAndShrink();
      growForever();
      printf("\n End of iteration %d", i);
      mDisplayTable();
   }

   return 0;
}

