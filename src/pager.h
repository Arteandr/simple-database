#ifndef _PAGER
#define _PAGER

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define TABLE_MAX_PAGES 100
/* 4kb size used in the VM systems on most computer arch */
#define PAGE_SIZE 4096

/* Pager is abstraction over pages */
typedef struct {
  int file_descriptor;
  u_int32_t file_length;
  u_int32_t num_pages;
  void *pages[TABLE_MAX_PAGES];
} Pager;

Pager *pager_open(const char *filename);
void pager_flush(Pager *pager, u_int32_t page_num);
void *get_page(Pager *pager, u_int32_t page_num);

#endif //_PAGER
