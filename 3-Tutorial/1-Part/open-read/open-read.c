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

int main(int argc, char *argv[])
{
   /*
    *
    * the attributes are:
    * - O_RDONLY: open the file for reading
    *
    */
   FILE *fd = fopen("0", "r");
   if (fd < 0)
   {
      printf("open error: %s\n", strerror(errno));
      return -1;
   }
   int bytes_read = -1;
   while (bytes_read != 0)
   {
      char buffer[128];
      memset(buffer, 0, sizeof(buffer));

      /* read the contents of the file */
      bytes_read = fread(buffer, 127, sizeof(char), fd);

      printf("%s", buffer);
   }

   /* close the file */
   fclose(fd);

   return 0;
}
