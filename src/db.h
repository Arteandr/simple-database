#ifndef _DB
#define _DB

#include "cursor.h"
#include "table.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;

typedef struct {
  StatementType type;
  Row insert_row;
} Statement;

typedef enum { COMMAND_SUCCESS, COMMAND_UNRECOGNIZED_COMMAND } CommandResult;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer();
void close_input_buffer(InputBuffer *input);

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_NEGATIVE_ID,
  PREPARE_UNRECOGNIZED_COMMAND,
  PREPARE_STRING_TOO_LONG,
  PREPARE_SYNTAX_ERROR
} PrepareResult;

PrepareResult prepare_statement(InputBuffer *input, Statement *statement);
ExecuteResult execute_statement(Statement *statement, Table *table);

#endif //_DB
