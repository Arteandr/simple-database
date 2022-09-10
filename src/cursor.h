#ifndef _CURSOR
#define _CURSOR

#include "table.h"
#include <sys/types.h>
#include <stdlib.h>

/* cursor object which represent a location in the table */
typedef struct {
  Table *table;
  u_int32_t row_num;
  u_int8_t end_of_table; // indicate position one past the last element
} Cursor;

Cursor *table_start(Table *table);
Cursor *table_end(Table *table);

void *cursor_value(Cursor *cursor);
void cursor_advance(Cursor *cursor);

#endif //_CURSOR
