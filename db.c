#include "table.h"
#include "db.h"

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

CommandResult do_command(InputBuffer *input, Table *table) {
  if (strcmp(input->buffer, ".exit") == 0) {
    close_input_buffer(input);
    free_table(table);
    exit(EXIT_SUCCESS);
  } else {
    return COMMAND_UNRECOGNIZED_COMMAND;
  }
}
PrepareResult prepare_statement(InputBuffer *input, Statement *statement) {
  if (strncmp(input->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;

    int args_assigned =
        sscanf(input->buffer, "insert %d %s %s", &(statement->insert_row.id),
               statement->insert_row.username, statement->insert_row.email);
    if (args_assigned < 3) {
      return PREPARE_SYNTAX_ERROR;
    }

    return PREPARE_SUCCESS;
  }

  if (strcmp(input->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_COMMAND;
}

extern u_int32_t TABLE_MAX_ROWS;

ExecuteResult execute_insert(Statement *statement, Table *table) {
  if (table->num_rows >= TABLE_MAX_ROWS)
    return EXECUTE_TABLE_FULL;

  Row *row = &(statement->insert_row);

  serialize_row(row, row_slot(table, table->num_rows));
  table->num_rows += 1;

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table) {
  Row row;
  for (u_int32_t i = 0; i < table->num_rows; i++) {
    deserialize_row(row_slot(table, i), &row);
    print_row(&row);
  }

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case (STATEMENT_INSERT):
    return execute_insert(statement, table);
  case (STATEMENT_SELECT):
    return execute_select(statement, table);
  }

  return EXECUTE_SUCCESS;
}

void close_input_buffer(InputBuffer *input) {
  free(input->buffer);
  free(input);
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input) {
  ssize_t bytes_read =
      getline(&(input->buffer), &(input->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  /* ignore trailing newline */
  input->input_length = bytes_read - 1;
  input->buffer[bytes_read - 1] = 0;
}

int main(int argc, char *argv[]) {
  Table *table = new_table();
  InputBuffer *input = new_input_buffer();

  while (1) {
    print_prompt();
    read_input(input);

    if (input->buffer[0] == '.') {
      switch (do_command(input, table)) {
      case (COMMAND_SUCCESS):
        continue;
      case (COMMAND_UNRECOGNIZED_COMMAND):
        printf("Unrecognized command '%s'\n", input->buffer);
        continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input, &statement)) {
    case (PREPARE_SUCCESS):
      break;
    case (PREPARE_SYNTAX_ERROR):
      printf("Syntax error. Could not parse statement.\n");
      continue;
    case (PREPARE_UNRECOGNIZED_COMMAND):
      printf("Unrecognized keyword at start of '%s'.\n", input->buffer);
      continue;
    }

    switch(execute_statement(&statement, table)) {
      case (EXECUTE_SUCCESS):
        printf("Done.\n");
        break;
      case (EXECUTE_TABLE_FULL):
        printf("Error: table full.\n");
        break;
    }
  }
}
