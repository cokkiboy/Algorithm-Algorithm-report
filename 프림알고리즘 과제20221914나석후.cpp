#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// 그래프의 정점을 나타내는 구조체
struct Node {
    int vertex;
    int weight;
    struct Node* next;
};

// 그래프를 나타내는 구조체
struct Graph {
    int V;  // 정점의 수
    struct Node** adjList; // 인접 리스트 배열
};

// 최소 힙 (min heap) 구조체
struct MinHeapNode {
    int vertex; // 정점 
    int key;
};

struct MinHeap {
    int size;
    int capacity;
    int* pos;  // 정점의 위치를 저장하는 배열
    struct MinHeapNode** array;
};

// 새로운 그래프를 생성하는 함수
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->adjList = (struct Node**)malloc(V * sizeof(struct Node*));

    for (int i = 0; i < V; ++i) {
        graph->adjList[i] = NULL;
    }

    return graph;
}

// 새로운 노드를 생성하는 함수
struct Node* createNode(int vertex, int weight) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// 최소 힙 노드를 생성하는 함수
struct MinHeapNode* createMinHeapNode(int vertex, int key) {
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->vertex = vertex;
    minHeapNode->key = key;
    return minHeapNode;
}

// 최소 힙을 생성하는 함수
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// 두 노드를 스왑하는 함수
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// 최소 힙을 유지하도록 하는 함수
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key)
        smallest = right;

    if (smallest != idx) {
        struct MinHeapNode* smallestNode = minHeap->array[smallest];
        struct MinHeapNode* idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// 힙에서 최소 키 값을 추출하는 함수
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (minHeap->size == 0)
        return NULL;

    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];

    minHeap->array[0] = lastNode;

    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[lastNode->vertex] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// 힙에서 주어진 정점의 키 값을 감소시키는 함수
void decreaseKey(struct MinHeap* minHeap, int vertex, int key) {
    int i = minHeap->pos[vertex];

    minHeap->array[i]->key = key;

    while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key) {
        minHeap->pos[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

// 최소 신장 트리를 구축하는 함수
void primMST(struct Graph* graph) {
    int V = graph->V;
    int parent[V];
    int key[V];
    struct MinHeap* minHeap = createMinHeap(V);

    for (int v = 0; v < V; ++v) {
        parent[v] = -1;
        key[v] = INT_MAX;
        minHeap->array[v] = createMinHeapNode(v, key[v]);
        minHeap->pos[v] = v;
    }

    key[0] = 0;
    minHeap->array[0] = createMinHeapNode(0, key[0]);
    minHeap->pos[0] = 0;

    minHeap->size = V;

    while (minHeap->size > 0) {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->vertex;

        struct Node* temp = graph->adjList[u];
        while (temp != NULL) {
            int v = temp->vertex;
            int weight = temp->weight;

            if (minHeap->pos[v] < minHeap->size && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                decreaseKey(minHeap, v, key[v]);
            }
            temp = temp->next;
        }
    }

    // 최소 신장 트리 출력
    for (int i = 1; i < V; ++i) {
        printf("Edge: %d - %d\n", parent[i], i);
    }
}

// 그래프에 간선을 추가하는 함수
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct Node* newNode = createNode(dest, weight);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

int main() {
    int V = 9; // 정점의 수
    struct Graph* graph = createGraph(V);

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 2, 3, 7);
	addEdge(graph, 3, 5, 14);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 7, 8, 7);



    printf("결과값:\n");
    primMST(graph);
   
    
    return 0;
    
}

