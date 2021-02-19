#include "permutation.h"

#include <iostream>
#include <fstream>

int main()
{
    std::ifstream graph_file("graph.txt");
    if (!graph_file.is_open())
    {
        std::cerr << "Couldn't open file!" << std::endl;
        return -1;
    }

    size_t n;
    graph_file >> n;

    auto graph = new int64_t*[n];
    for (size_t i = 0; i < n; i++)
    {
        graph[i] = new int64_t[n];
        for (size_t j = 0; j < n; j++)
        {
            if (graph_file.eof())
            {
                std::cerr << "Incorrect graph format." << std::endl;
                return -1;
            }
            graph_file >> graph[i][j];
        }
    }
    graph_file.close();

    size_t shortest_path = SIZE_MAX;
    Permutation perm(n - 1);
    do {
        size_t current_path = 0;
        size_t from = 0;
        size_t to;
        for (size_t i = 0; i < n - 1; i++)
        {
            to = perm[i];
            current_path += graph[from][to];
            from = to;
        }
        current_path += graph[from][0];
        perm.print();
        std::cout << " " << current_path << " " << std::endl;
        if (shortest_path > current_path)
        {
            shortest_path = current_path;
//            std::cout << "Current shortest found: " << shortest_path << std::endl;
//
//            std::cout << "0 "; perm.print();
//            std::cout << "0" << std::endl << std::endl;
        }
    } while (perm.next());
    std::cout << std::endl << "Shortest: " << shortest_path << std::endl;
    return 0;
}
