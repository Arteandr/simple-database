#include "table.h"

void serialize_row(Row *src, void *dest) {
  memcpy(dest + ID_OFFSET, &(src->id), ID_SIZE);
  memcpy(dest + USERNAME_OFFSET, &(src->username), USERNAME_SIZE);
  memcpy(dest + EMAIL_OFFSET, &(src->email), EMAIL_SIZE);
}

void deserialize_row(void *src, Row *dest) {
  memcpy(&(dest->id), src + ID_OFFSET, ID_SIZE);
  memcpy(&(dest->username), src + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(dest->email), src + EMAIL_OFFSET, EMAIL_SIZE);
}

/* Representation of serealized row:
 *
 * column   | size | offset
 * id       | 4    | 0
 * username | 32   | 4
 * email    | 255  | 36
 * total    | 291  |
 */

void *row_slot(Table *table, u_int32_t row_num) {
  u_int32_t page_num = row_num / ROWS_PER_PAGE;

  void *page = table->pages[page_num];
  if (page == NULL) {
    /* allocate memory only when try to access page */
    page = table->pages[page_num] = malloc(PAGE_SIZE);
  }

  u_int32_t row_offset = row_num % ROWS_PER_PAGE;
  u_int32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}

void print_row(Row *row) {
  printf("(%d, %s, %s)", row->id, row->username, row->email);
}

Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->num_rows = 0;
  for (u_int32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    table->pages[i] = NULL;
  }

  return table;
}

void free_table(Table *table) {
  for (int i = 0; table->pages[i]; i++) {
    free(table->pages[i]);
  }

  free(table);
}
