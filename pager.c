#include "pager.h"

/* open the database file and keeps track of its size
 * initialize the page cache to all NULLs */
Pager *pager_open(const char *filename) {
  int fd = open(filename,
                O_RDWR | O_CREAT,   // r/w mode && create file if does not exist
                S_IWUSR | S_IRUSR); // user r/w permissions
  if (fd == -1) {
    printf("Unable to open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  off_t file_length = lseek(fd, 0, SEEK_END);

  Pager *pager = malloc(sizeof(Pager));
  pager->file_descriptor = fd;
  pager->file_length = file_length;

  for (u_int32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    pager->pages[i] = NULL;
  }

  return pager;
}
