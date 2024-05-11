#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

ino_t returnINODE(const char *path) {
  struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return statbuf.st_ino;
}

time_t returnModificationTime(const char *path) {
    struct stat attr;
    if (stat(path, &attr) != 0)
      return 0;
    return attr.st_mtime;
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
              if(!fp) {
                printf("Could't open file: %s ", file_name);
              }
              else {
                time_t timeOfModification = returnModificationTime(s);
                ino_t inode = returnINODE(s);
                char time_string[100];
                strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&timeOfModification));
                fprintf(fp, "Inode: %ld \n", inode);
                fprintf(fp, "Last modified at: %s\n", time_string);
              }
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