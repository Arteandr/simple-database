#include "cursor.h"

Cursor *table_start(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = 0;
  cursor->end_of_table = (table->num_rows == 0);

  return cursor;
}

Cursor *table_end(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->row_num = table->num_rows;
  cursor->end_of_table = 1;

  return cursor;
}

extern u_int32_t ROWS_PER_PAGE;
extern u_int32_t ROW_SIZE;

void *cursor_value(Cursor *cursor) {
  u_int32_t row_num = cursor->row_num;
  u_int32_t page_num = row_num / ROWS_PER_PAGE;

  void *page = get_page(cursor->table->pager, page_num);
  u_int32_t row_offset = row_num % ROWS_PER_PAGE;
  u_int32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}

void cursor_advance(Cursor *cursor) {
  cursor->row_num += 1;
  if (cursor->row_num >= cursor->table->num_rows) {
    cursor->end_of_table = 1;
  }
}
