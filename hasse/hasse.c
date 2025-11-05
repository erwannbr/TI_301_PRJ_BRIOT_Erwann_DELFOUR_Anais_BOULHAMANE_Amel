#include <malloc.h>
#include "hasse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void links_init(t_link_array *a) {
    a->links = NULL;
    a->size = 0;
    a->length = 0;
}

void links_add(t_link_array *a, int start, int end) {
    for (int i = 0; i < a->size; ++i) {
        if (a->links[i].start == start && a->links[i].end == end) return;
    }
    if (a->size == a->length) {
        a->length = (a->length == 0) ? 8 : a->length * 2;
        a->links = (t_link*)realloc(a->links, a->length * sizeof *a->links);
    }
    a->links[a->size++] = (t_link){ start, end };
}

int *create_array_vertex_to_class(int vertex_count, const t_partition *P) {
    int *map = (int*)malloc(vertex_count * sizeof *map);
    if (!map) return NULL;
    for (int v = 0; v < vertex_count; ++v) map[v] = -1;

    for (int c = 0; c < P->nb_class; ++c) {
        const t_class *cls = P->classes[c];
        for (int k = 0; k < cls->nb_vertices; ++k) {
            int v_id_1based = cls->vertices[k]; // ex: 1..N
            int v_index = v_id_1based - 1;      // -> 0..N-1
            if (0 <= v_index && v_index < vertex_count)
                map[v_index] = c;
        }
    }
    return map;
}

void list_class_links(int vertex_count, Vertex *graph, int *vertex_to_class, t_link_array *class_links)
{
    for (int vertex = 0; vertex < vertex_count; vertex++) {
        int class_from = vertex_to_class[vertex];

        for (int n = 0; n < graph[vertex].neighbor_count; n++) {
            int neighbor = graph[vertex].neighbors[n];
            int class_to = vertex_to_class[neighbor];

            if (class_from != class_to) {
                links_add(class_links, class_from, class_to);
            }
        }
    }
}

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.start == link2.start)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.start == link2.end) && (link3.start == link1.end))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}
