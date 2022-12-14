#ifndef _BTREE
#define _BTREE

#include <sys/types.h>
#include <stdio.h>

/* diff layouts */
typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

extern const u_int32_t LEAF_NODE_NUM_CELLS_OFFSET;
extern const u_int32_t LEAF_NODE_HEADER_SIZE;
extern const u_int32_t LEAF_NODE_CELL_SIZE;
extern const u_int32_t LEAF_NODE_KEY_SIZE;

u_int32_t *leaf_node_num_cells(void *node);
void *leaf_node_cell(void *node, u_int32_t cell_num);
u_int32_t *leaf_node_key(void *node, u_int32_t cell_num);
void *leaf_node_value(void *node, u_int32_t cell_num);
void initialize_leaf_node(void *node);
void print_leaf_node(void *node);

#endif // !BTREE
