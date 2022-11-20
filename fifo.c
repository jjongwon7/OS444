// fifo.c
#include "hw4.h"

// 큐 초기화하는 함수
void initQueue(Queue *queue) { 
    queue->front = queue->rear = NULL; // front, rear NULL로 초기화
    queue->count = 0; // 큐 안의 노드 개수를 0으로 설정
}
// 큐가 비어있는지 확인하는 함수
int isEmpty(Queue *queue) { 
    return queue->count == 0; // 큐안의 노드 개수가 0이면 빈 상태
}
// 큐에 데이터를 삽입하는 함수
void enqueue(Queue *queue, int data) {
    Node *newNode = (Node *)malloc(sizeof(Node)); // 삽입할 노드 생성
    newNode->data = data;
    newNode->next = NULL; // FIFO 구조이므로 next는 NULL로 초기화
    if (isEmpty(queue)) // 큐가 비어있는 경우
    {
        queue->front = newNode; // front를 newNode로 설정
    }
    else // 큐가 비어있지 않는 경우
    {
        queue->rear->next = newNode; // rear의 next가 newNode를 가리키도록 함
    }
    queue->rear = newNode; // 맨 뒤를 newNode로 설정
    queue->count++; // 큐안의 노드 개수를 1 증가
 }
// 큐에 있는 데이터를 삭제하는 함수
void dequeue(Queue *queue) 
{
    Node *ptr;
    if (isEmpty(queue)) // 큐가 비어있는 경우
    {
        printf("큐가 비어있습니다.\n");
        return ;
    }
    ptr = queue->front; // 맨 앞에 있는 노드로 설정
    queue->front = ptr->next; // front를 다음 노드로 이동시키고
    free(ptr); // ptr 메모리 해제
    queue->count--; // 큐안의 노드 개수를 1 감소
}
// 큐 탐색
int searchQueue(Queue *queue, int data)
{
    int count = 0;
    Node *ptr;
    ptr = queue->front; // 맨 앞에 있는 노드로 설정
    while(1)
    {
        count++;
        if(ptr->data == data) // 탐색 성공 시 노드 순서 리턴
            return count;
        else if(ptr->next != NULL) // NULL이 아니면 ptr에 다음 노드 설정하고 다시 반복
        {
            ptr = ptr->next;
            continue;
        }
        else
            return 0; // 탐색 실패 시 0 리턴
    }
}
// 페이지 프레임 요소들 출력하는 함수
void printQueue(Queue *queue, int pageFrameNum, int page, int isPageFault) {
    Node *ptr;
    ptr = queue->front;

    printf("%d\t\t", page); // 현재 참조한 페이지 출력
    fprintf(res, "%d\t\t", page);

    while(1) // 맨 앞 노드부터 마지막 노드까지 출력
    {
        printf("%d\t\t", ptr->data);
        fprintf(res, "%d\t\t", ptr->data);
        if(ptr->next == NULL)
            break;
        ptr = ptr->next;
    }
    
    if(queue->count < pageFrameNum) // 페이지 프레임에 빈 공간이 있는 경우
        for(int i=0; i<pageFrameNum-queue->count; i++) // 빈 공간은 '-'로 표시
        {
            printf("-\t\t");
            fprintf(res, "-\t\t");
        } 
    if(isPageFault) // page fault라면 출력
    {
        printf("PAGE FAULT");
        fprintf(res, "PAGE FAULT");
    }    
    printf("\n");
    fprintf(res, "\n");
}
void FIFO(int refStr[], int pageFrameNum) {
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수
    Queue queue; // Queue 구조체 선언
    // UI 
    printf("===== FIFO =====\nInput\t\t");
    fprintf(res, "===== FIFO =====\nInput\t\t");
    
    for(int i=0; i<pageFrameNum; i++)
    {
        printf("Frame%d\t\t",i+1);
        fprintf(res, "Frame%d\t\t", i+1);
    }
    printf("\n");
    fprintf(res, "\n");
    
    initQueue(&queue); // 큐 초기화
    // 페이지 스트링 개수만큼 반복
    for(int i=0; i<STRING_SIZE; i++)
    {
        // 큐가 비어있는 경우
        if(isEmpty(&queue))
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            enqueue(&queue, refStr[i]); // 큐에 노드 삽입
            printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
            isPageFault = 0;
        }
        // hit인 경우
        else if(searchQueue(&queue, refStr[i]) != 0)
        {
            hitNum++; // hit 개수 + 1
            printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
        }
        // miss인 경우
        else
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            // frame에 빈 공간이 있는 경우
            if(queue.count < pageFrameNum)
            {
                enqueue(&queue, refStr[i]); // 큐의 맨 뒤에 노드 추가
                printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
            }
            // frame에 빈 공간이 없는 경우
            else
            {
                dequeue(&queue); // 맨 앞 노드를 삭제하고
                enqueue(&queue, refStr[i]); // 맨 뒤에 노드 추가
                printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
            }
            isPageFault = 0;
        }
    }
    printf("HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    fprintf(res, "HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    printf("PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
    fprintf(res, "PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
}