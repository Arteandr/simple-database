#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input) {
  ssize_t bytes_read = 
    getline(&(input->buffer),&(input->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }
}


int main(int argc, char* argv[]) {
  InputBuffer *input = new_input_buffer();
  
  while (1) {
    print_prompt();

    if (strcmp(input->buffer, ".exit") == 0) {
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", input->buffer);
    }
  }
}
