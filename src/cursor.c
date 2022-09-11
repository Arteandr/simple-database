#include "cursor.h"

Cursor *table_start(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->page_num = table->root_page_num;
  cursor->cell_num = 0;

  void *root_node = get_page(table->pager, table->root_page_num);
  u_int32_t num_cells = *leaf_node_num_cells(root_node);
  cursor->end_of_table = (num_cells == 0);

  return cursor;
}

Cursor *table_end(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->page_num = table->root_page_num;

  void *root_node = get_page(table->pager, table->root_page_num);
  u_int32_t num_cells = *leaf_node_num_cells(root_node);

  cursor->cell_num = num_cells;
  cursor->end_of_table = 1;

  return cursor;
}

extern u_int32_t ROWS_PER_PAGE;
extern u_int32_t ROW_SIZE;

void *cursor_value(Cursor *cursor) {
  u_int32_t page_num = cursor->page_num;
  void *page = get_page(cursor->table->pager, page_num);
  return leaf_node_value(page, cursor->cell_num);
}

void cursor_advance(Cursor *cursor) {
  u_int32_t page_num = cursor->page_num;
  void *node = get_page(cursor->table->pager, page_num);

  cursor->cell_num += 1;
  if (cursor->cell_num >= (*leaf_node_num_cells(node))) {
    cursor->end_of_table = 1;
  }
}

void leaf_node_insert(Cursor *cursor, u_int32_t key, Row *value) {
  void *node = get_page(cursor->table->pager, cursor->page_num);

  u_int32_t num_cells = *leaf_node_num_cells(node);
  if (num_cells >= LEAF_NODE_MAX_CELLS) {
    printf("implement \n");
    exit(EXIT_FAILURE);
  }

  if (cursor->cell_num < num_cells) {
    for (u_int32_t i = num_cells; i > cursor->cell_num; i--) {
      memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1),
             LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_node_num_cells(node)) += 1;
  *(leaf_node_key(node, cursor->cell_num)) = key;
  serialize_row(value, leaf_node_value(node,cursor->cell_num));
}
