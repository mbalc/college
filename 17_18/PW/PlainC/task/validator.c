#include <stdio.h>
#include <stdlib.h>
// #include <map>

int N, A, Q, U, F_count, start_state;

const int MAX_STATE_COUNT = 100, MAX_ALPHA_SIZE = 'z' - 'a';
// std::map<pair<int, int>, int> F;

void read() {
  char *line;
  size_t buffer_size = 32;
  line = (char*)calloc(buffer_size, sizeof(char));

  scanf ("%d%d%d%d%d\n", &N, &A, &Q, &U, &F_count);
  scanf ("%d\n", &start_state);

  getline(&line, &buffer_size, stdin);

  for (int i = 0; i < N - 1; ++i) {
    printf(">%s<\n", line);
  }
}

int main () {
  read();

  return 0;
}
