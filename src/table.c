#include "table.h"

const u_int32_t ID_SIZE = size_of_attribute(Row, id);
const u_int32_t USERNAME_SIZE = size_of_attribute(Row, username);
const u_int32_t EMAIL_SIZE = size_of_attribute(Row, email);
const u_int32_t ID_OFFSET = 0;
const u_int32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const u_int32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const u_int32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_OFFSET;

const u_int32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const u_int32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

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

// void *row_slot(Table *table, u_int32_t row_num) {
//   u_int32_t page_num = row_num / ROWS_PER_PAGE;
//   
//   void *page = get_page(table->pager, page_num);
//
//   u_int32_t row_offset = row_num % ROWS_PER_PAGE;
//   u_int32_t byte_offset = row_offset * ROW_SIZE;
//
//   return page + byte_offset;
// }

void print_row(Row *row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

Table *db_open(const char *filename) {
  Pager *pager = pager_open(filename);
  u_int32_t num_rows = pager->file_length / ROW_SIZE;

  Table *table = malloc(sizeof(Table));
  table->pager = pager;
  table->num_rows = num_rows;

  return table;
}


void db_close(Table *table) {
  Pager *pager = table->pager;
  u_int32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;

  for (u_int32_t i = 0; i < num_full_pages; i++) {
    if (pager->pages[i] == NULL)
      continue;

    pager_flush(pager, i, PAGE_SIZE);
    free(pager->pages[i]);
    pager->pages[i] = NULL;
  }

  u_int32_t num_addit_rows = table->num_rows % ROWS_PER_PAGE;
  if (num_addit_rows > 0) {
    u_int32_t page_num = num_full_pages;
    if (pager->pages[page_num] != NULL) {
      pager_flush(pager, page_num, num_addit_rows * ROW_SIZE);
      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }

  int result = close(pager->file_descriptor);
  if (result == -1) {
    printf("Error closing db file.\n");
    exit(EXIT_FAILURE);
  }

  for (u_int32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    void *page = pager->pages[i];
    if (page) {
      free(page);
      pager->pages[i] = NULL;
    }
  }

  free(pager);
  free(table);
}
