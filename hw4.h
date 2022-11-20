// hw4.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PAGE_NUMBER 30 // 참조 페이지 스트링 범위 (1~30)
#define STRING_SIZE 500 // 페이지 스트링 개수 500으로 설정
#define MAX_LEN STRING_SIZE * 10 // 참조 페이지 스트링을 입력 받을 배열의 크기
extern FILE * res; // 결과를 저장할 파일 포인터

// === main ===
int* createPage();
char* createRW();
char* createPageStr();
void createPageStrFile();

// === OPTIMAL ===
int isHit(int frame[], int frameElementNum, int page);
void printFrame(int frame[], int pageFrameNum, int frameElementNum, int page, int isPageFault);
int findVictim(int frame[], int refStr[], int pageFrameNum, int cur);
void Optimal(int refStr[], int pageFrameNum);

// === FIFO ===
typedef struct Node {
    int data;
    struct Node *next;
}Node;

typedef struct Queue {
    Node *front;
    Node *rear;
    int count; // 큐 안의 노드 개수
}Queue;
void initQueue(Queue *queue);
int isEmpty(Queue *queue);
void enqueue(Queue *queue, int data);
void dequeue(Queue *queue);
int searchQueue(Queue *queue, int data);
void printQueue(Queue *queue, int pageFrameNum, int page, int isPageFault);
void FIFO(int refStr[], int pageFrameNum);

// === LIFO ===
void LIFO(int refStr[], int pageFrameNum);

// === LRU ===
void updateHitNode(Queue *queue, int order, int pageFrameNum);
void LRU(int refStr[], int pageFrameNum);

// === LFU ===
void dequeueLFU(Queue *queue, int pageCount[], int pageFrameNum);
void LFU(int refStr[], int pageFrameNum);

// === SC ===
typedef struct CNode{
    int data;
    int rBit;
    int wBit;
}CNode;

typedef struct CircularQueue{
    CNode* node;
    int front;
    int rear;
    int maxSize;
    int curSize;
} CircularQueue;
void initCircularQueue(CircularQueue *circularQueue, int pageFrameNum);
int isCircularQueueEmpty(CircularQueue* circularQueue);
void enqueueCQ(CircularQueue* circularQueue, int data);
int searchCQ(CircularQueue* circularQueue, int data);
void replacementCQ(CircularQueue* circularQueue, int data);
void printCQ(CircularQueue* circularQueue, int pageFrameNum, int page, int isPageFault);
void SC(int refStr[], int pageFrameNum);
// === ESC ===
void enqueueESC(CircularQueue* circularQueue, int data, char * refStrRW, int cur);
void replacementESC(CircularQueue* circularQueue, int data, char * refStrRW, int cur);
void printESC(CircularQueue* circularQueue, int pageFrameNum, int page, int isPageFault, char * refStrRW, int cur);
void ESC(int refStr[], char refStrRW[], int pageFrameNum);
