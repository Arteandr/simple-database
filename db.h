#ifndef DB
#define DB

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
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
void execute_statement(Statement *statement);

#endif // !DB
