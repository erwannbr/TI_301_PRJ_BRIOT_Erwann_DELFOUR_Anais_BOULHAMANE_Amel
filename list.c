//
// Created by anais on 20/10/2025.
//
t_std_list * CreateEmptyList () {
    t_std_list *list = (t_std_list*)malloc(sizeof(t_std_list));
    list.h = NULL;
    return list;
}