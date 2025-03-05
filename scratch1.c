#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct node234* node234;

struct frac{ /* Stores a fraction */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
  unsigned long long int a; /* Numerator */
  unsigned long long int b; /* Denominator */
#pragma GCC diagnostic pop
};

struct node234
{
  struct frac V[3]; /* Key values */
  node234 p[4]; /* Pointers */
};


/* Function to process file input and execute corresponding commands */
void process_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
      perror("Error opening file");
      return;
  }
  
  char line[256];
  while (fgets(line, sizeof(line), file)) {
      if (line[0] == '#') {
          printf("%s", line); // Print comment lines
          continue;
      }
      
      char command;
      struct frac f;
      int index;
      
      if (sscanf(line, " %c", &command) != 1) continue;
      
      switch (command) {
          case 'S':
              if (sscanf(line, "S %d", &index) == 1) {
                  printf("Show node at index %d\n", index);
                  // Call showNode(index);
              }
              break;
          case 'F':
              if (sscanf(line, "F %llu/%llu", &f.a, &f.b) == 2) {
                  printf("Searching for fraction %llu/%llu\n", f.a, f.b);
                  // Call search function and return index
              }
              break;
          case 'N':
              printf("Printing tree in inorder\n");
              // Call inorder print function
              break;
          case 'P':
              printf("Printing tree in preorder\n");
              // Call preorder print function
              break;
          case 'I':
              if (sscanf(line, "I %llu/%llu", &f.a, &f.b) == 2) {
                  printf("Inserting fraction %llu/%llu\n", f.a, f.b);
                  // Call insert function
              }
              break;
          case 'D':
              if (sscanf(line, "D %llu/%llu", &f.a, &f.b) == 2) {
                  printf("Deleting fraction %llu/%llu\n", f.a, f.b);
                  // Call delete function
              }
              break;
          case 'L':
              printf("Loading tree configuration\n");
              // Call function to load tree
              break;
          case 'X':
              printf("Terminating program\n");
              fclose(file);
              return;
          default:
              printf("Unknown command: %c\n", command);
      }
  }
  
  fclose(file);
}

/* Example usage */
int main(int argc, char* argv[]) {
  if (argc < 2) {
      fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
      return 1;
  }
  
  process_file(argv[1]);
  return 0;
}

