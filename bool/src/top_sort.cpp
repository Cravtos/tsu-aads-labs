#include "../include/top_sort.h"

#include <vector>

void add_edge(BoolMat &bm, size_t from, size_t to) {
    bm[from].set_1(to);
}

void remove_edge(BoolMat &bm, size_t from, size_t to) {
    bm[from].set_0(to);
}

BoolVec check_cols(BoolMat &bm) {
    BoolVec res(bm.get_cols());
    for (size_t i = 0; i < bm.get_rows(); i++) {
        res |= bm[i];
    }

    return res;
}

void top_sort(BoolMat &graph, size_t* res) {
    BoolVec handled(graph.get_cols()); // обработанные вершины
    BoolVec has_trailers(graph.get_cols());
    BoolVec new_solutions(graph.get_cols());

    while (true) {
        has_trailers = check_cols(graph); // вершины, имеющие предшественников на тек. шаге
        new_solutions = ~has_trailers & ~handled;

        if (new_solutions.weight() == 0) {
            if (handled.weight() != handled.size())
            {
                printf("There is a loop in graph!\n");
            }
            break;
        }

        handled |= new_solutions; // добавляем обработанные вершины
        for (size_t i = 0; i < new_solutions.size(); i++) {
            if (new_solutions[i]) {
                *res = i;
                ++res;
                graph.empty_row(i); // удаляем соответсвущие строки
            }
        }
    }
}