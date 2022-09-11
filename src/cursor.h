#ifndef _CURSOR
#define _CURSOR

#include "table.h"
#include "pager.h"
#include "b_tree.h"
#include <stdlib.h>
#include <sys/types.h>

extern const u_int32_t LEAF_NODE_MAX_CELLS;

/* cursor object which represent a location in the table */
typedef struct {
  Table *table;
  u_int32_t page_num;
  u_int32_t cell_num;
  u_int8_t end_of_table; // indicate position one past the last element
} Cursor;

Cursor *table_start(Table *table);
Cursor *table_end(Table *table);

void *cursor_value(Cursor *cursor);
void cursor_advance(Cursor *cursor);


/* remove */
void leaf_node_insert(Cursor *cursor, u_int32_t key, Row *value);

#endif //_CURSOR
