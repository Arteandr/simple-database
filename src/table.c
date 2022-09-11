#include "table.h"

const u_int32_t ID_SIZE = size_of_attribute(Row, id);
const u_int32_t USERNAME_SIZE = size_of_attribute(Row, username);
const u_int32_t EMAIL_SIZE = size_of_attribute(Row, email);
const u_int32_t ID_OFFSET = 0;
const u_int32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const u_int32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const u_int32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_OFFSET;

/* TODO: remove */
/* common node header */
const u_int32_t NODE_TYPE_SIZE = sizeof(u_int8_t);
const u_int32_t NODE_TYPE_OFFSET = 0;
const u_int32_t IS_ROOT_SIZE = sizeof(u_int8_t);
const u_int32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const u_int32_t PARENT_POINTER_SIZE = sizeof(u_int32_t);
const u_int32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const u_int8_t COMMON_NODE_HEADER_SIZE =
    NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

/* leaf node header */
const u_int32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(u_int32_t);
const u_int32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const u_int32_t LEAF_NODE_HEADER_SIZE =
    COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

/* leaf node body */
const u_int32_t LEAF_NODE_KEY_SIZE = sizeof(u_int32_t);
const u_int32_t LEAF_NODE_KEY_OFFSET = 0;
extern const u_int32_t ROW_SIZE;
const u_int32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const u_int32_t LEAF_NODE_VALUE_OFFSET =
    LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const u_int32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const u_int32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const u_int32_t LEAF_NODE_MAX_CELLS =
    LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

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

void print_row(Row *row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

Table *db_open(const char *filename) {
  Pager *pager = pager_open(filename);

  Table *table = malloc(sizeof(Table));
  table->pager = pager;
  table->root_page_num = 0;

  if (pager->num_pages == 0) {
    void *root_node = get_page(pager, 0);
    initialize_leaf_node(root_node);
  }

  return table;
}

void db_close(Table *table) {
  Pager *pager = table->pager;

  for (u_int32_t i = 0; i < pager->num_pages; i++) {
    if (pager->pages[i] == NULL)
      continue;

    pager_flush(pager, i);
    free(pager->pages[i]);
    pager->pages[i] = NULL;
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
