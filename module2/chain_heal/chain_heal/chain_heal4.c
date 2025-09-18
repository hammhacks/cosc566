
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
  *n = 0;
  size_t capacity = 0;
  struct node **arr = NULL;

  char *line = NULL;
  ssize_t len;
  ssize_t read;

  while ((read = getline(&line, &len, stdin)) != -1) {

    int x;
    int y;
    int cur;
    int max;
    char *name = NULL;
    int matched = sscanf(line, "%d %d %d %d %ms", &x, &y, &cur, &max, &name);
    
    struct node *base_node = malloc(sizeof(struct node));//check later--this works without allocating node pointer size

    base_node->name = name;
    base_node->x = x;
    base_node->y = y;
    base_node->cur_PP = cur;
    base_node->max_PP = max;
    base_node->adj_size = 0;
    base_node->visited = false;
    base_node->adj_list = NULL;

    if (*n + 1 > capacity) {
      if (capacity)
        capacity *= 2;
      else
        capacity = 16;

      struct node **tmp = realloc(arr, capacity * sizeof(struct node *));

      arr = tmp;
    }

    arr[(*n)++] = base_node;
  }

  free(line);
  return arr;
}

bool adjacent(int jump_range, struct node *a, struct node *b) {
  int node_a_x;
  int node_a_y;
  int node_b_x;
  int node_b_y;

  bool return_value;

  node_a_x = a->x;
  node_a_y = a->y;
  node_b_x = b->x;
  node_b_y = b->y;

  int y_distance = node_a_y - node_b_y;
  int x_distance = node_a_x - node_b_x;


  if ((x_distance * x_distance) + (y_distance * y_distance) <= jump_range * jump_range)
    return_value = true;
  else
    return_value = false;


  return return_value;
}

void setup_graph(int jump_range, size_t num_nodes, struct node **nodes) {

  // First pass: compute adjacency sizes
  for (int i=0; i<num_nodes; i++) {
    nodes[i]->adj_size = 0;
  }

  for (int i=0; i<num_nodes; i++) {
    for (int j=i+1; j<num_nodes; j++) {
      if (adjacent(jump_range, nodes[i], nodes[j])) {
        nodes[i]->adj_size++;
        nodes[j]->adj_size++;
      }
    }
  }

  for (int i=0; i<num_nodes; i++) {
    if (nodes[i]->adj_size > 0) {
      nodes[i]->adj_list = malloc(nodes[i]->adj_size * sizeof(struct node *));
      if (!nodes[i]->adj_list) { perror("malloc"); exit(1); }
    } 
    else {
      (*(nodes+i))->adj_list = NULL;
    }
    (*(nodes+i))->adj_size = 0;
  }

  for (int i=0;i<num_nodes;i++) {
    for (int j=i+1; j<num_nodes; j++) {
      if (adjacent(jump_range, nodes[i], nodes[j])) {
        nodes[i]->adj_list[nodes[i]->adj_size++] = nodes[j];
        nodes[j]->adj_list[nodes[j]->adj_size++] = nodes[i];
      }
    }
  }
}

void print_adjacency(size_t num_nodes, struct node **nodes) {
  for (int i=0; i<num_nodes; i++) {
    printf("%s\n", (*(nodes+i))->name);
    for (int j = 0; j < (*(nodes+i))->adj_size; j++) {
      printf("  %s\n", (*(nodes+i))->adj_list[j]->name);
    }
  }
}

static int simulate_healing(struct params *p, struct answer *a) {
  
  int healing_total = 0;
  
  //
  double power = (double)p->initial_power;

  for (int i=0; i<a->current_path_length; i++) {
    struct node *temp = a->current[i];
    int pot = (int)(power);
    if (pot > 0) {

      int healing_need = temp->max_PP - temp->cur_PP;
      if (healing_need < 0)
        healing_need = 0;
      int healed;
      if (pot < healing_need)
        healed = pot;
      else
        healed = healing_need;

      healing_total += healed;
    }

    power = power * (1 - p->power_reduction);
    if (power < 0)
      power = 0;
  }
  return healing_total;
}

void check_best(struct params *p, struct answer *a) {
  int total = simulate_healing(p, a);
  if (total > a->best_healing) {

    a->best_healing = total;
    a->best_path_length = a->current_path_length;

    if (a->best_path_length > 0) {
      memcpy(a->best, a->current, a->best_path_length * sizeof(struct node *));
    }
  }
}

void print_best(struct params *p, struct answer *a) {

  double power = (double)p->initial_power;
  int total = 0;
  for (int i=0; i<a->best_path_length; i++) {
    struct node *nd = a->best[i];
    int potential = (int)(power);
    int need = nd->max_PP - nd->cur_PP;
    if (need < 0) 
      need = 0;
    int healed;
    if(potential<need)
      healed = potential;
    else
      healed = need;

    if (potential <= 0) 
      healed = 0;

    printf("%s %d\n", nd->name, healed);
    total += healed;
    power = power * (1 - p->power_reduction);

    if (power < 0)
      power = 0;
  }
  // lol don't change this line otherwise autograder doesn't work :(
  printf("Total_Healing %d\n", total);
}

void dfs(struct params *p, struct answer *a, int depth, struct node *n) {
  
  if (n->visited)
    return;


  n->visited = true;
  a->current[a->current_path_length++] = n;


  if (a->current_path_length >= p->num_jumps) {
    check_best(p, a);

    a->current_path_length--;
    n->visited = false;

    return;
  }


  bool progressed = false;
  for (int i=0; i< n->adj_size; i++) {
    struct node *neighbor = n->adj_list[i];

    if (!neighbor->visited) {
      progressed = true;
      dfs(p, a, depth+1, neighbor);
    }
  }


  check_best(p, a);


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


  size_t n_nodes = 0;
  struct node **nodes = create_nodes_array(&n_nodes);

  setup_graph(p.jump_range, n_nodes, nodes);

  int urgosa_loc_id;
  for (int i=0; i<n_nodes; i++) {
    if (strcmp((*(nodes+i))->name, "Urgosa_the_Healing_Shaman") == 0) {
      urgosa_loc_id = i;
      break;
    }
  }

  struct answer a;
  a.best_path_length = 0;
  a.current_path_length = 0;
  a.best_healing = 0;
  a.best = malloc(n_nodes * sizeof(struct node *));
  a.current = malloc(n_nodes * sizeof(struct node *));

  long change_x;
  long change_y;
  long changes_multiplied;
  long ranges_multiplied;

  for (size_t i = 0; i < n_nodes; i++) {

    change_x = (long)(*(nodes+i))->x - (long)(*(nodes+urgosa_loc_id))->x;
    change_y = (long)nodes[i]->y - (long)nodes[urgosa_loc_id]->y;
    changes_multiplied = (long)change_x*change_x + (long)change_y*change_y;
    ranges_multiplied = (long)p.initial_range * (long)p.initial_range;
    if (changes_multiplied <= ranges_multiplied) {
      dfs(&p, &a, 0, *(nodes+i));
    }
  }


  print_best(&p, &a);
  



cleanup:

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
