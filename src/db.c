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
    db_close(table);
    exit(EXIT_SUCCESS);
  } else {
    return COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_insert(InputBuffer *input, Statement *statement) {
  statement->type = STATEMENT_INSERT;

  char *keyword = strtok(input->buffer, " ");
  char *id_string = strtok(NULL, " ");
  char *username = strtok(NULL, " ");
  char *email = strtok(NULL, " ");

  if (id_string == NULL || username == NULL || email == NULL)
    return PREPARE_SYNTAX_ERROR;

  int id = atoi(id_string);
  if (id <= 0)
    return PREPARE_NEGATIVE_ID;

  if (strlen(username) > COLUMN_USERNAME_SIZE)
    return PREPARE_STRING_TOO_LONG;

  if (strlen(email) > COLUMN_EMAIL_SIZE)
    return PREPARE_STRING_TOO_LONG;

  statement->insert_row.id = id;
  strcpy(statement->insert_row.username, username);
  strcpy(statement->insert_row.email, email);

  return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer *input, Statement *statement) {
  if (strncmp(input->buffer, "insert", 6) == 0)
    return prepare_insert(input, statement);

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
  Cursor *cursor = table_end(table);

  serialize_row(row, cursor_value(cursor));
  table->num_rows += 1;

  free(cursor);

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table) {
  Row row;
  Cursor *cursor = table_start(table);

  if (table->num_rows <= 0) {
    printf("Table is empty.\n");
    return EXECUTE_SUCCESS;
  }
  
  while(!(cursor->end_of_table)) {
    deserialize_row(cursor_value(cursor), &row);
    print_row(&row);
    cursor_advance(cursor);
  }

  free(cursor);

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
  if (argc < 2) {
    printf("Must supply a database filename.\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  Table *table = db_open(filename);

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
    case (PREPARE_NEGATIVE_ID):
      printf("ID must be positive.\n");
      continue;
    case (PREPARE_STRING_TOO_LONG):
      printf("String is too long.\n");
      continue;
    case (PREPARE_SYNTAX_ERROR):
      printf("Syntax error. Could not parse statement.\n");
      continue;
    case (PREPARE_UNRECOGNIZED_COMMAND):
      printf("Unrecognized keyword at start of '%s'.\n", input->buffer);
      continue;
    }

    switch (execute_statement(&statement, table)) {
    case (EXECUTE_SUCCESS):
      printf("Done.\n");
      break;
    case (EXECUTE_TABLE_FULL):
      printf("Error: table full.\n");
      break;
    }
  }
}
