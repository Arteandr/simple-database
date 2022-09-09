#ifndef _TABLE
#define _TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define TABLE_MAX_PAGES 100
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

typedef struct {
  u_int32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

void print_row(Row *row);

typedef struct {
  u_int32_t num_rows;
  void *pages[TABLE_MAX_PAGES];
} Table;

Table *new_table();
void free_table(Table *table);

void serialize_row(Row *src, void *dest);
void deserialize_row(void *src, Row *dest);
void *row_slot(Table *table, u_int32_t row_num);

#endif //_TABLE
