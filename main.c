#include <stdio.h>
#include "adjacency_list/adjacency_list.h"
#include "utils/utils.h"


int main() {
    t_adjacency_list new_adj_list = readGraph("../DATA/exemple1.txt");
    display_adjacency_list(&new_adj_list);
    checkIfMarkov(new_adj_list);
    return 0;
}
