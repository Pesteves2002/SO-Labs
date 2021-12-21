/*
 * open-read.c
 *
 * Simple example of opening and reading to a file.
 *
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_LEN 128

int main(int argc, char *argv[])
{
   /*
    *
    * the attributes are:
    * - O_RDONLY: open the file for reading
    *
    */
   FILE *fd = fopen("shrek.txt", "r");
   if (fd < 0)
   {
      printf("open error: %s\n", strerror(errno));
      return -1;
   }
   FILE *wd = fopen("test-out.txt", "w");

   char buffer[BUFFER_LEN];
   memset(buffer, 0, sizeof(buffer));
   int bytes_read = fread(buffer, sizeof(char), strlen(buffer) + 1, fd);

   while (bytes_read > 0)
   {
      /* read the contents of the file */
      fwrite(buffer, sizeof(char), strlen(buffer) + 1, wd);
      memset(buffer, 0, sizeof(buffer));
      bytes_read = fread(buffer, sizeof(char), strlen(buffer) + 1, fd);
   }

   /* close the file */
   fclose(fd);
   fclose(wd);

   return 0;
}
