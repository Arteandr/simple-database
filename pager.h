#ifndef _PAGER
#define _PAGER

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define TABLE_MAX_PAGES 100

typedef struct {
  int file_descriptor;
  u_int32_t file_length;
  void *pages[TABLE_MAX_PAGES];
} Pager;

Pager *pager_open(const char *filename);

#endif //_PAGER
