#include "../include/bool_mat.h"
#include "../include/top_sort.h"

#include <fstream>

int main() {
    std::ifstream fg;
    fg.open("graph.txt");
    if (!fg) {
        std::cout << "File is not open" << std::endl;
        return -1;
    }

    size_t n;
    fg >> n;
    BoolMat graph(n, n);

    size_t to, from;
    while (!fg.eof()) {
        fg >> from >> to;
        std::cout << from << " " << to << std::endl;
        add_edge(graph, from, to);
    }
    fg.close();

    std::cout << "Graph: " << std::endl << graph << std::endl;

    auto res = new size_t[n];

    top_sort(graph, res);

    std::cout << "Result is: ";
    for (size_t i = 0; i < n; i++) {
        std::cout << res[i] << " ";
    }

    delete[] res;

    return 0;
}