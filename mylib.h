#ifndef MYLIB_H
#define MYLIB_H
#include <stdbool.h>

void pwd();
int echo();
int ls(bool);
// int whoami();
int cd(const char *dirOrFileName);
void cat_file(const char *filename);

#endif
