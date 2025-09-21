/* Author: Jeff Hammond
  Class: COSC566
  Instructor: Dr. Haynes / Steven Dao
  Date: September 20th, 2025
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//This will be my node. This is the same one provided by the instructions
struct node {
  char *name;
  int x, y;
  int cur_PP, max_PP;
  int adj_size;
  bool visited;
  struct node **adj_list;
};

//These will be my input params--storing them in a struct helps me reference them later on
struct params {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
};

//This struct will be the answer struct--it will hold the answer that is returned as output and include
//the best path length, current path length, list of best nodes, and list of current.
struct answer {
  int best_path_length;
  int current_path_length;
  struct node **best;
  struct node **current;
  int best_healing;
};

//This function will create the nodes array from the linked list. We use size_t as input to handle
//the largest objects safely in memory.
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
    //I had to use %ms to read in the text or else I would get segfaults. 
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

    //For memory safety, I used a dynamic array concept to scale the list. Without this, I kept hitting segfaults. I probably
    //could have gotten the other method to work, but this one was the safest and passed the runner!
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

//This is my adjacent function--like the prototype given from the assignment, it takes the jumprange and two nodes.
//The distance formula calculates the delta of xs and ys, and then squares (a*a + b*b <= c*c)
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

//I use the setup_graph function to 
void setup_graph(int jump_range, size_t num_nodes, struct node **nodes) {

  //calculates adjecency sizes
  for (int i=0; i<num_nodes; i++) {
    nodes[i]->adj_size = 0;
  }

  //assigns adj_size based on adjacency calculation!
  for (int i=0; i<num_nodes; i++) {
    for (int j=i+1; j<num_nodes; j++) {
      if (adjacent(jump_range, *(nodes+i), *(nodes+j))) {
        nodes[i]->adj_size++;
        nodes[j]->adj_size++;
      }
    }
  }
  
  //Once I have the adj_size, I go through and allocate based on count
  for (int i=0; i<num_nodes; i++) {
    if (nodes[i]->adj_size > 0) {
      nodes[i]->adj_list = malloc(nodes[i]->adj_size * sizeof(struct node *));
    } 
    else {
      (*(nodes+i))->adj_list = NULL;
    }
    (*(nodes+i))->adj_size = 0;
  }

  //Similar to above, I go through and assign the nodes because I now have the sizes. Seems inefficient, but holy
  //segfault I couldn't get another way to work.
  for (int i=0;i<num_nodes;i++) {
    for (int j=i+1; j<num_nodes; j++) {
      if (adjacent(jump_range, nodes[i], nodes[j])) {
        nodes[i]->adj_list[nodes[i]->adj_size++] = nodes[j];
        nodes[j]->adj_list[nodes[j]->adj_size++] = nodes[i];
      }
    }
  }
}

//this is a simple print function for the adjacency values
void print_adjacency(size_t num_nodes, struct node **nodes) {
  for (int i=0; i<num_nodes; i++) {
    printf("%s\n", (*(nodes+i))->name);
    for (int j = 0; j < (*(nodes+i))->adj_size; j++) {
      printf("  %s\n", (*(nodes+i))->adj_list[j]->name);
    }
  }
}

int simulate_healing(struct params *p, struct answer *a) {
  
  int healing_total = 0;
  double power = (double)p->initial_power;

  //Performs the healing
  for (int i=0; i<a->current_path_length; i++) {
    struct node *temp = a->current[i];
    int potential = (int)(power);

    if (potential > 0) {
      int healing_need = temp->max_PP - temp->cur_PP;
      if (healing_need < 0)
        healing_need = 0;
      int healed;
      if (potential < healing_need)
        healed = potential;
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

//This function checks for the best healing. It gets the healing total from the paths and if
//the total is greater, it sets the new best healing value
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

//This is my depth-first search. The code below plays out like
void dfs(struct params *p, struct answer *a, int depth, struct node *n) {
  
  //"If visited, then return"--copy that
  if (n->visited)
    return;

  //Else it sets the visited to true, sets the node value, and increases the path length
  n->visited = true;
  a->current[a->current_path_length++] = n;

  //If the current path is greater than or equal to the number of jumps from the params input, We check the best
  //and decrease the path length and set visited to false before returning
  if (a->current_path_length >= p->num_jumps) {
    check_best(p, a);

    a->current_path_length--;
    n->visited = false;

    return;
  }

  //If we get past those, we set progressed to false, update the neighbor node, and if the neighbor hasn't
  //been visited, we set progressed to true and perform dfs
  bool progressed = false;
  for (int i=0; i< n->adj_size; i++) {

    struct node *neighbor = n->adj_list[i];

    if (!neighbor->visited) {
      progressed = true;
      dfs(p, a, depth+1, neighbor);
    }
  }

  //Simple check_best to make sure the adjustments are now true; without this we miss the final delta
  check_best(p, a);

  a->current_path_length--;
  n->visited = false;
}

int main(int argc, char **argv) {
  if (argc < 6) {
    fprintf(stderr, "usage: %s initial_range jump_range num_jumps initial_power power_reduction < input_file\n", argv[0]);
    return 0;
  }
  //I'm sure there is a better programatic way to do this, but....here we are.
  struct params p;
  p.initial_range = atoi(argv[1]);
  p.jump_range = atoi(argv[2]);
  p.num_jumps = atoi(argv[3]);
  p.initial_power = atoi(argv[4]);
  p.power_reduction = atof(argv[5]);


  size_t n_nodes = 0;
  struct node **nodes = create_nodes_array(&n_nodes);

  setup_graph(p.jump_range, n_nodes, nodes);

  //This almost could have been its own function, but code was pretty small.
  //We check for Urgosa and get Urgosa's node ID. We assume Urgosa will always be there (no input validation)
  int urgosa_loc_id;
  for (int i=0; i<n_nodes; i++) {
    if (strcmp((*(nodes+i))->name, "Urgosa_the_Healing_Shaman") == 0) {
      urgosa_loc_id = i;
      break;
    }
  }

  //Queue up the answer struct
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

    //If the urgosa and the node are within range, we start dfs
    if (changes_multiplied <= ranges_multiplied) {
      dfs(&p, &a, 0, *(nodes+i));
    }
  }

  print_best(&p, &a);

cleanup:
//clean up--free up what I've used.
free(a.best);
  free(a.current);
  for (size_t i = 0; i < n_nodes; ++i) {
    free((*(nodes+i))->name);
    free((*(nodes+i))->adj_list);
    free((*(nodes+i)));
  }
  free(nodes);

  return 0;
}
