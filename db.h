#ifndef DB
#define DB

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "table.h"

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;

typedef struct {
  StatementType type;
  Row insert_row;
} Statement;

typedef enum {
  COMMAND_SUCCESS,
  COMMAND_UNRECOGNIZED_COMMAND
} CommandResult;

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer();

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_COMMAND
} PrepareResult;

PrepareResult prepare_statement(InputBuffer *input, Statement *statement);
ExecuteResult execute_statement(Statement *statement, Table *table);

#endif // !DB
