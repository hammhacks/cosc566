#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node {
  char *name;
  int x, y;
  int cur_PP, max_PP;
  int adj_size;
  bool visited;
  struct node **adj_list;
};

struct params {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
};

struct answer {
  int best_path_length;
  int current_path_length;
  struct node **best;
  struct node **current;
  int best_healing;
};

struct node **create_nodes_array(size_t *n) {
}

bool adjacent(int jump_range, struct node *a, struct node *b) {
}

void setup_graph(int jump_range, size_t num_nodes, struct node **nodes) {
}

void print_adjacency(size_t num_nodes, struct node **nodes) {
}

void check_best(struct params *p, struct answer *a) {
}

void print_best(struct params *p, struct answer *a) {
}

void dfs(struct params *p, struct answer *a, int depth, struct node *n) {
}

int main(int argc, char **argv) {
  if(argc < 6) {
    fprintf(stderr, "usage: %s initial_range jump_range num_jumps initial_power power_reduction < input_file\n", argv[0]);
    return 0;
  }

  struct params p;
  struct answer a = {0};


cleanup:

  return 0;
}
