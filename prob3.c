#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

void Open(char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
      return;
    }
    struct dirent *d;
    while ((d = readdir(dir)) != NULL) {
        if(strcmp(d->d_name, "..") && strcmp(d->d_name, ".")) {
            char s[100];
            strcpy(s, dirname);
            strcat(s, "/");
            strcat(s, d->d_name);
            if(isDirectory(s))
              Open(s);
            else {
              printf("%s\n", s);
              char file_name[100];
              strcpy(file_name, d->d_name);
              strcat(file_name, "_snapshot");
              FILE *fp;
              fp = fopen (file_name, "w");

            }
        }

    }
}



int main(int argc, char **argv)
{

    for(int i = 1; i < argc; i++) {
      Open(argv[i]);
    }
 
    return 0;
}