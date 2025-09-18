#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
  char *name;
  int x, y;
  int cur_PP, max_PP;
  int adj_size;
  bool visited;
  struct node **adj_list;
}node;

struct params {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
}params;

struct answer {
  int best_path_length;
  int current_path_length;
  struct node **best;
  struct node **current;
  int best_healing;
};

void scan_and_print_input(struct params *p,char **argv){
  sscanf(argv[1],"%d",&p->initial_range);
  sscanf(argv[2],"%d",&p->jump_range);
  sscanf(argv[3],"%d",&p->num_jumps);
  sscanf(argv[4],"%d",&p->initial_power);
  sscanf(argv[5],"%lf",&p->power_reduction);

  printf("initial range: %d\n",p->initial_range);
  printf("jump range: %d\n",p->jump_range);
  printf("num_jumps: %d\n",p->num_jumps);
  printf("initial_power: %d\n",p->initial_power);
  printf("power_reduction: %f\n",p->power_reduction);


}

void print_linked_list(node *input_node) {
    node *curr = input_node;
    while (curr != NULL) {
        printf("name is: %s\n", curr->name);
        curr = (curr->adj_list != NULL) ? *(curr->adj_list) : NULL;
    }
    printf("end...\n");
}


struct node **create_nodes_array(size_t *n) {
  
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

  //printf("x distance is %d\n",x_distance);
  //printf("y distance is %d\n",y_distance);

  if ((x_distance * x_distance) + (y_distance * y_distance) <= jump_range * jump_range)
    return_value = true;
  else
    return_value = false;


  return return_value;
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
  
  if (n->visited)
    return;


  n->visited = true;
  a->current[a->current_path_length++] = n;
  if (a->current_path_length >= p->num_jumps) {
    check_best(p, a);
    a->current_path_length-=1;
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



node *create_node(struct node *node_input) {
  printf("node input name is: %s\n",node_input->name);
    node *new_node = malloc(sizeof(node*));
    new_node->x = node_input->x;
    new_node->y = node_input->y;
    new_node->cur_PP = node_input->cur_PP;
    new_node->max_PP = node_input->max_PP;
    new_node->name = malloc(50*sizeof(char));
    new_node->name = node_input->name;
    new_node->adj_list = NULL; // initially no link
    return new_node;
}

void append(node *input_node,node *master_list) {
    node *curr = master_list;

    while (curr->adj_list != NULL) {
        curr = *(curr->adj_list);
    }

    node *new_node = create_node(input_node);

    curr->adj_list = malloc(sizeof(node *));
    *(curr->adj_list) = new_node;
}



int main(int argc, char *argv[]) {
  if(argc < 6) {
    fprintf(stderr, "usage: %s initial_range jump_range num_jumps initial_power power_reduction < input_file\n", argv[0]);
    return 0;
  }

  struct params *p = malloc(sizeof(params));
  int node_count = 0;
  int i;
  struct answer a = {0};
 
  // section 1: read in the print parameters
  scan_and_print_input(p,argv);


  // section 3: set up nodes to read in input and then create a linked list
  struct node *list = malloc(sizeof(node*));
  list->name = malloc(50*sizeof(char));

  char *line = NULL;
  size_t len = 0;
  ssize_t read; // getline() usage

  struct node *current = NULL;

  while ((read = getline(&line, &len, stdin)) != -1) {
    current = malloc(sizeof(node));
    current->name = malloc(50*sizeof(char));
    sscanf(line, "%d %d %d %d %s", &current->x, &current->y, &current->cur_PP, &current->max_PP, current->name);

    if(node_count == 0){
      list = current;
    }
    else{
      append(current,list);
    }
    node_count++;
  }


  print_linked_list(list);


  // section 4: fill linked list nodes into an array
  struct node **nodes_array = malloc(sizeof(node*) * node_count);
  int countdown = node_count;

  for(i=0;i<node_count;i++){
    nodes_array[i] = list;

    if(list->adj_list != NULL)
      list = *(list->adj_list);
    //printf("nodes_array[i] name is now: %s\n",nodes_array[i]->name);
  }



// section 5: populate adjacency list on array of nodes (populate graph?)
int x,t;
for(i=0;i<node_count;i++){
  struct node **indexes = malloc(node_count*sizeof(node));
  int node_found_count=0;
  for(x=0;x<node_count;x++){
    if(x!=i){
      if(adjacent(p->jump_range,nodes_array[i],nodes_array[x])){
        indexes[node_found_count++]=nodes_array[x];
      }
    }
  }
  printf("node count found is: %d\n",node_found_count);
  nodes_array[i]->adj_list = malloc(sizeof(node)*node_found_count);
  nodes_array[i]->adj_size = node_found_count;
  nodes_array[i]->adj_list = indexes;
  printf("pointer for nodes_array at %d is: %p\n",i,nodes_array[i]->adj_list);
}



//verify that the graph looks right lol
for(i=0;i<node_count;i++){
  printf("The player is: %s\n",nodes_array[i]->name);
  //printf("\tplayer x value is: %d\n",nodes_array[i]->x);
  //printf("\tplayer y value is: %d\n",nodes_array[i]->y);
  printf("nodes_array size is: %d\n",nodes_array[i]->adj_size);
  struct node **sublist = malloc(sizeof(node)*node_count);
  sublist=nodes_array[i]->adj_list;
  for(x=0;x<nodes_array[i]->adj_size;x++){
    
    printf("\tadjacent player is: %s\n",(*(sublist+x))->name);
  }

}


//set each node's visited to false and get urgosa
node *urgosa = malloc(sizeof(node));
for(i=0;i<node_count;i++){
  nodes_array[i]->visited=false;
  int result1 = strcmp("Urgosa_the_Healing_Shaman",nodes_array[i]->name);
  printf("strcmp(\"%s\", \"%s\") = %d\n", "Urgosa_the_Healing_Shaman", nodes_array[i]->name, result1);
  if(strcmp("Urgosa_the_Healing_Shaman",nodes_array[i]->name)==0){
    urgosa = nodes_array[i];
    printf("Urgosa is at node %d\n",i);
  }
}

a.best = malloc(sizeof(node));
a.current = malloc(sizeof(node));

for (int i = 0; i < urgosa->adj_size; i++) {
        node *neigh_node = urgosa->adj_list[i];
        //dfs(neigh_node, &answer, max_depth);
        dfs(p,a,6,neigh_node);
}




cleanup:

  return 0;
}


/*
for(int i=0;i<node_count;i++){
  printf("address of %d: %p\n",i,&nodes_array[i]);
  printf("array at %d: %s\n", i, nodes_array[i]->name);
  printf("array_x at %d: %d\n",i,nodes_array[i]->x);
  printf("array_y at %d: %d\n",i,nodes_array[i]->y);
  printf("adj_list at %d: %p\n",i,nodes_array[i]->adj_list);
} */

//bool adj = adjacent(p.jump_range,nodes_array[1],nodes_array[2]);

  //printf("-Xname: %s\n",nodes_array[i]->name);
  //for(t=0;t<node_found;t++)
  //printf("t-%s\n",temp_nodes[t]->name);


  /*printf("current adjlist name is: %s\n",curr->name);
printf("adj list values: \n");
struct node *temp1 = curr->adj_list[0];
struct node *temp2 = curr->adj_list[1];
printf("name 1: %s\n",temp1->name);
printf("name 2: %s\n",temp2->name);*/