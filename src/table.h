#ifndef _TABLE
#define _TABLE

#include "b_tree.h"
#include "pager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

typedef struct {
  u_int32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

void print_row(Row *row);

typedef struct {
  Pager *pager;
  u_int32_t root_page_num;
} Table;

Table *db_open(const char *filename);
void db_close(Table *table);

void serialize_row(Row *src, void *dest);
void deserialize_row(void *src, Row *dest);

#endif //_TABLE
