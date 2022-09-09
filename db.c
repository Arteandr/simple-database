#include "db.h"

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

CommandResult do_command(InputBuffer *input) {
  if (strcmp(input->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return COMMAND_UNRECOGNIZED_COMMAND;
  }
}
PrepareResult prepare_statement(InputBuffer *input, Statement *statement) {
  if (strcmp(input->buffer, "insert") == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }

  if (strcmp(input->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_COMMAND;
}

void execute_statement(Statement *statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      printf("This is for insert \n");
      break;
    case (STATEMENT_SELECT):
      printf("This is for select \n");
      break;
  }
}

void close_input_buffer(InputBuffer *input) {
  free(input->buffer);
  free(input);
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input) {
  ssize_t bytes_read = 
    getline(&(input->buffer),&(input->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  /* ignore trailing newline */
  input->input_length = bytes_read - 1;
  input->buffer[bytes_read - 1] = 0;
}


int main(int argc, char* argv[]) {
  InputBuffer *input = new_input_buffer();
  
  while (1) {
    print_prompt();
    read_input(input);

    if (input->buffer[0] == '.') {
      switch (do_command(input)) {
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
      case (PREPARE_UNRECOGNIZED_COMMAND):
        printf("Unrecognized keyword at start of '%s'.\n", input->buffer);
        continue;
    }

    execute_statement(&statement);
    printf("Executed.\n");
  }
}
