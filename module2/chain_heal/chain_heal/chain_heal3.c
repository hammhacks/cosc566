// chain_heal.c
// Implements Chain Heal optimal-path search for the lab assignment.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

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
  struct node **best;    // array of node pointers for best path
  struct node **current; // array of node pointers for current path
  int best_healing;
};

/* Read all nodes from stdin and return an allocated array of node pointers.
 * The number of nodes is stored in *n.
 * Input format per line:
 *   X Y Current_PP Max_PP Name
 * We allocate each struct node and name string; caller must free them.
 */
struct node **create_nodes_array(size_t *n) {
  *n = 0;
  size_t cap = 0;
  struct node **arr = NULL;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  while ((linelen = getline(&line, &linecap, stdin)) != -1) {
    if (linelen == 0) continue;

    // skip blank lines or lines that start with whitespace only
    char *p = line;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\n' || *p == '\0') continue;

    int x, y, cur, max;
    char *name = NULL;
    int matched = sscanf(line, "%d %d %d %d %ms", &x, &y, &cur, &max, &name);
    if (matched < 4) {
      // ignore malformed lines (assignment says input is correct, so this shouldn't happen)
      free(name);
      continue;
    }
    if (matched == 4) {
      // name not parsed; set empty name (shouldn't happen per spec)
      name = strdup("");
    }

    struct node *nd = malloc(sizeof(struct node));
    if (!nd) { perror("malloc"); exit(1); }
    nd->name = name;
    nd->x = x;
    nd->y = y;
    nd->cur_PP = cur;
    nd->max_PP = max;
    nd->adj_size = 0;
    nd->visited = false;
    nd->adj_list = NULL;

    if (*n + 1 > cap) {
      cap = cap ? cap * 2 : 16;
      struct node **tmp = realloc(arr, cap * sizeof(struct node *));
      if (!tmp) { perror("realloc"); exit(1); }
      arr = tmp;
    }
    arr[(*n)++] = nd;
  }

  free(line);
  return arr;
}

/* Return true if a and b are within jump_range (squared distance test) */
bool adjacent(int jump_range, struct node *a, struct node *b) {
  long long dx = (long long)a->x - (long long)b->x;
  long long dy = (long long)a->y - (long long)b->y;
  long long lhs = dx*dx + dy*dy;
  long long rhs = (long long)jump_range * (long long)jump_range;
  return lhs <= rhs;
}

/* Build adjacency lists for all nodes (undirected graph) using jump_range */
void setup_graph(int jump_range, size_t num_nodes, struct node **nodes) {
  if (num_nodes == 0) return;

  // First pass: compute adjacency sizes
  for (size_t i = 0; i < num_nodes; ++i) {
    nodes[i]->adj_size = 0;
  }
  for (size_t i = 0; i < num_nodes; ++i) {
    for (size_t j = i + 1; j < num_nodes; ++j) {
      if (adjacent(jump_range, nodes[i], nodes[j])) {
        nodes[i]->adj_size++;
        nodes[j]->adj_size++;
      }
    }
  }

  // Allocate adj_list for each node
  for (size_t i = 0; i < num_nodes; ++i) {
    if (nodes[i]->adj_size > 0) {
      nodes[i]->adj_list = malloc(nodes[i]->adj_size * sizeof(struct node *));
      if (!nodes[i]->adj_list) { perror("malloc"); exit(1); }
    } else {
      nodes[i]->adj_list = NULL;
    }
    // reuse adj_size as an insertion index initially
    nodes[i]->adj_size = 0;
  }

  // Second pass: fill adjacency lists
  for (size_t i = 0; i < num_nodes; ++i) {
    for (size_t j = i + 1; j < num_nodes; ++j) {
      if (adjacent(jump_range, nodes[i], nodes[j])) {
        nodes[i]->adj_list[nodes[i]->adj_size++] = nodes[j];
        nodes[j]->adj_list[nodes[j]->adj_size++] = nodes[i];
      }
    }
  }
}

/* Optional debugging helper: prints adjacency lists */
void print_adjacency(size_t num_nodes, struct node **nodes) {
  for (size_t i = 0; i < num_nodes; ++i) {
    printf("%s\n", nodes[i]->name);
    for (int j = 0; j < nodes[i]->adj_size; ++j) {
      printf("  %s\n", nodes[i]->adj_list[j]->name);
    }
  }
}

/* Simulate healing for the path contained in a->current (length current_path_length)
 * using parameters p. Return the total healing amount.
 */
static int simulate_healing(struct params *p, struct answer *a) {
  int total = 0;
  double power = (double)p->initial_power;
  for (int i = 0; i < a->current_path_length; ++i) {
    struct node *nd = a->current[i];
    int pot = (int)(power); // truncate toward zero
    if (pot > 0) {
      int need = nd->max_PP - nd->cur_PP;
      if (need < 0) need = 0;
      int healed = pot < need ? pot : need;
      total += healed;
    }
    // reduce power for next jump
    power = power * (1.0 - p->power_reduction);
    if (power < 0.0) power = 0.0;
  }
  return total;
}

/* If current path is better than best, copy it to best. */
void check_best(struct params *p, struct answer *a) {
  int total = simulate_healing(p, a);
  if (total > a->best_healing) {
    // copy current to best
    a->best_healing = total;
    a->best_path_length = a->current_path_length;
    if (a->best_path_length > 0) {
      memcpy(a->best, a->current, a->best_path_length * sizeof(struct node *));
    }
  }
}

/* Print the best path and total healing in required format */
void print_best(struct params *p, struct answer *a) {
  // Need to compute per-player healed amounts to print lines
  double power = (double)p->initial_power;
  int total = 0;
  for (int i = 0; i < a->best_path_length; ++i) {
    struct node *nd = a->best[i];
    int pot = (int)(power);
    int need = nd->max_PP - nd->cur_PP;
    if (need < 0) need = 0;
    int healed = pot < need ? pot : need;
    if (pot <= 0) healed = 0;
    printf("%s %d\n", nd->name, healed);
    total += healed;
    power = power * (1.0 - p->power_reduction);
    if (power < 0.0) power = 0.0;
  }
  printf("Total_Healing %d\n", total);
}

/* Depth-first search that enumerates simple paths (no repeated nodes).
 * depth is unused (kept for skeleton compatibility).
 * We use a->current_path_length to manage current path and a->current array.
 */
void dfs(struct params *p, struct answer *a, int depth, struct node *n) {
  if (n->visited)
    return;

  // add node to current path
  n->visited = true;
  a->current[a->current_path_length++] = n;

  // If we've reached maximum allowed jumps (num_jumps), evaluate
  if (a->current_path_length >= p->num_jumps) {
    check_best(p, a);
    // backtrack
    a->current_path_length--;
    n->visited = false;
    return;
  }

  // Recurse to all adjacent nodes
  bool progressed = false;
  for (int i = 0; i < n->adj_size; ++i) {
    struct node *nbr = n->adj_list[i];
    if (!nbr->visited) {
      progressed = true;
      dfs(p, a, depth + 1, nbr);
    }
  }

  // Even if we didn't advance further, the current (shorter) path could be best
  check_best(p, a);

  // backtrack
  a->current_path_length--;
  n->visited = false;
}

int main(int argc, char **argv) {
  if (argc < 6) {
    fprintf(stderr, "usage: %s initial_range jump_range num_jumps initial_power power_reduction < input_file\n", argv[0]);
    return 0;
  }

  struct params p;
  p.initial_range = atoi(argv[1]);
  p.jump_range = atoi(argv[2]);
  p.num_jumps = atoi(argv[3]);
  p.initial_power = atoi(argv[4]);
  p.power_reduction = atof(argv[5]);

  // Read nodes
  size_t n_nodes = 0;
  struct node **nodes = create_nodes_array(&n_nodes);

  // If no nodes, nothing to do
  if (n_nodes == 0) {
    fprintf(stderr, "No nodes in input\n");
    return 0;
  }

  // Build graph by jump_range
  setup_graph(p.jump_range, n_nodes, nodes);

  // locate Urgosa
  int urgosa_idx = -1;
  for (size_t i = 0; i < n_nodes; ++i) {
    if (strcmp(nodes[i]->name, "Urgosa_the_Healing_Shaman") == 0) {
      urgosa_idx = (int)i;
      break;
    }
  }
  if (urgosa_idx == -1) {
    fprintf(stderr, "Urgosa_the_Healing_Shaman not found in input\n");
    // cleanup before exit
    for (size_t i = 0; i < n_nodes; ++i) {
      free(nodes[i]->name);
      free(nodes[i]->adj_list);
      free(nodes[i]);
    }
    free(nodes);
    return 0;
  }

  // Prepare answer structure
  struct answer a;
  a.best_path_length = 0;
  a.current_path_length = 0;
  a.best_healing = INT_MIN; // allow negative? but healing is non-negative; ensures update
  a.best = malloc(n_nodes * sizeof(struct node *));
  a.current = malloc(n_nodes * sizeof(struct node *));
  if (!a.best || !a.current) { perror("malloc"); exit(1); }

  // For each node within initial_range of Urgosa, run DFS starting at that node
  for (size_t i = 0; i < n_nodes; ++i) {
    // distance test using squared distances
    long long dx = (long long)nodes[i]->x - (long long)nodes[urgosa_idx]->x;
    long long dy = (long long)nodes[i]->y - (long long)nodes[urgosa_idx]->y;
    long long lhs = dx*dx + dy*dy;
    long long rhs = (long long)p.initial_range * (long long)p.initial_range;
    if (lhs <= rhs) {
      // Start DFS at nodes[i]
      dfs(&p, &a, 0, nodes[i]);
    }
  }

  // If best_healing remains INT_MIN (no path found), set to 0 and best_path_length 0
  if (a.best_healing == INT_MIN) {
    a.best_healing = 0;
    a.best_path_length = 0;
  }

  // Print best path (as required)
  print_best(&p, &a);

  // cleanup
  free(a.best);
  free(a.current);
  for (size_t i = 0; i < n_nodes; ++i) {
    free(nodes[i]->name);
    free(nodes[i]->adj_list);
    free(nodes[i]);
  }
  free(nodes);

  return 0;
}
