// Найденные ошибки:
// 1. В AddEdge - если обе вершины существуют, и связь уже есть, setEdge равнялся 1, хотя должен равняться 0. И наоборот.
// Исправление:
// - setEdge = (bool) trTmp;
// + setEdge = !((bool) trTmp);

#include <cstdio>

struct Trailer;
struct Leader {
    int key;        // Номер вершины
    int count;      // Число предшественников (число входящих дуг)
    Leader* next;   // Указатель на следующий элемент в списке вершин
    Trailer *tr;    // Указатель на список вершин, к которым ведут исходящие дуги
};

struct Trailer {
    Leader* lead;   // Элемент, к которому есть исходящая дуга
    Trailer* next;  // Следующий элемент
};

void AddEdge(Leader* &vertexes, int vertexA, int vertexB) {
    Leader *ptrA = nullptr;
    Leader *ptrB = nullptr;
    
    for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->next);
    for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->next);
    
    bool setEdge = true;
    if (ptrA && ptrB) { // Если существует A и B
        Trailer* trTmp = ptrA->tr;
        for (Trailer* trTmp = ptrA->tr; trTmp && trTmp->lead != ptrB; trTmp = trTmp->next);
        setEdge = !((bool) trTmp); // setEdge равен 0 когда из A есть связь в B
    } else {
        if (!ptrA) {
            ptrA = new Leader;
            ptrA->key = vertexA;
            ptrA->count = 0;
            ptrA->next = nullptr;
            ptrA->tr = nullptr;
            ptrA->next = vertexes;
            vertexes = ptrA;    // Добавляем в начало списка новую вершину, если её не было
        }
        if (!ptrB) {
        ptrB = new Leader;
        ptrB->key = vertexB;
        ptrB->count = 0;
        ptrB->next = nullptr;
        ptrB->tr = nullptr;
        ptrB->next = vertexes;
        vertexes = ptrB;        // Добавляем в начало списка новую вершину, если её не было
        }
    }

    if (setEdge) {
        ptrB->count++;
        Trailer* elemTr = new Trailer;
        elemTr->lead = ptrB;
        elemTr->next = ptrA->tr;
        ptrA->tr = elemTr;
    }
}

void PrintGraph(Leader* vertexes) {
    if (vertexes == nullptr) {
        printf("Graph is Empty");
        return;
    }

    while (vertexes) {
        Trailer* trTmp = vertexes->tr;
        while (trTmp) {
            printf("(%d,%d)\n", vertexes->key, trTmp->lead->key);
            trTmp = trTmp->next;
        }
        vertexes = vertexes->next;
    }
}

void DeleteGraph(Leader* &vertexes) {
    while (vertexes) {
        while (vertexes->tr) {
            Trailer* tmp = vertexes->tr;
            vertexes->tr = vertexes->tr->next;
            delete tmp;
        }
        Leader* tmp = vertexes;
        vertexes = vertexes->next;
        delete tmp;
    }
}

/*
* ExcludeEdge - Функция удаления ребра из графа
* : Входные пареметры
* vertexes - список вершин графа
* (vertexA,vertexB) Ребро графа определяется парой вершин
* : Возвращаемое значение
* true - ребро удалено
* false - ребро не удалено
*
*/
bool ExcludeEdge(Leader* &vertexes, int vertexA, int vertexB) {
    Leader* ptrA = nullptr;
    Leader* ptrB = nullptr;

    for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->next);
    for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->next);

    if (ptrA && ptrA->tr && ptrB) {
        if (ptrA->tr->lead == ptrB) {
            Trailer* t = ptrA->tr;
            ptrA->tr = ptrA->tr->next;
            delete t;
            ptrB->count--;
        } else {
            Trailer* trTmp;
            for (trTmp = ptrA->tr; trTmp->next && trTmp->next->lead != ptrB; trTmp = trTmp->next);
            if (trTmp->next) {
                Trailer * t = trTmp->next;
                trTmp->next = trTmp->next->next;
                delete t;
                ptrB->count--;
            }
        }
        return true;
    }
    return false;
}

/*
* 1 2
* 1 3
* 1 5
* 4 5
* 2 4
*
*/
int main() {
    FILE* graphInFile = fopen("graph.dat","r");
    if (!graphInFile) {
        puts("File is not open");
        return -1;
    }
    Leader* graph = nullptr;
    int vertexA = 0;
    int vertexB = 0;

    while (fscanf(graphInFile,"%d%d",&vertexA,&vertexB) > 0) {
        printf("(%d,%d)\n",vertexA,vertexB);
        AddEdge(graph,vertexA,vertexB);
    }
    fclose(graphInFile);

    puts("Print Graph");
    PrintGraph(graph);

    puts("Excluding (1,5) (4,5) (5,3)");
    ExcludeEdge(graph,1,5);
    ExcludeEdge(graph,4,5);
    ExcludeEdge(graph,5,3);
    PrintGraph(graph);

    DeleteGraph(graph);
    PrintGraph(graph);
}