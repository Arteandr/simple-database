#include "b_tree.h"

u_int32_t *leaf_node_num_cells(void *node) {
  return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void *leaf_node_cell(void *node, u_int32_t cell_num) {
  return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

u_int32_t *leaf_node_key(void *node, u_int32_t cell_num) {
  return leaf_node_cell(node, cell_num);
}

void *leaf_node_value(void *node, u_int32_t cell_num) {
  return leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

void initialize_leaf_node(void *node) { *leaf_node_num_cells(node) = 0; }

void print_leaf_node(void *node) {
  u_int32_t num_cells = *leaf_node_num_cells(node);
  printf("leaf (size %d)\n", num_cells);
  for (u_int32_t i = 0; i < num_cells; i++) {
    u_int32_t k = *leaf_node_key(node, i);
    printf("  - %d : %d\n", i, k);
  }
}
