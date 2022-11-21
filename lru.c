// lru.c
#include "hw4.h"

// Hit가 발생했을 때 해당 페이지 프레임 노드를 큐의 맨 뒤로 보내주는 함수
void updateHitNode(Queue *queue, int order) {
    Node *ptr;
    Node *tmp;
    ptr = queue->front;
    
    if(order == 1) // 맨 앞에 있는 노드인 경우
    {
        queue->front = ptr->next; // front를 두 번째 노드로 설정
        ptr->next = NULL; // 맨 앞에 있던 노드의 next를 NULL로 설정하고
        queue->rear->next = ptr; // rear의 next가 ptr을 가리키도록 함
        queue->rear = ptr; // rear를 ptr로 설정
    }
    else if(order == queue->count) // 맨 뒤에 있는 노드인 경우
        return ; // 처리할 필요 x
    else // 사이에 위치한 노드인 경우
    {
        for(int i=1; i<order; i++) // next가 목표 노드를 가리키고 있는 노드를 탐색
        {
            if(i == order-1) // 원하는 노드를 찾은 경우
                tmp = ptr; // tmp에 해당 노드 저장
            ptr = ptr->next;
        }
        // -현재 tmp next가 ptr을 가리키고 있는 상황-
        tmp->next = ptr->next; // tmp가 ptr이 가리키고 있는 노드를 가리키도록 함
        ptr->next = NULL; // ptr 노드의 next를 NULL로 설정하고
        queue->rear->next = ptr; // rear의 next가 ptr을 가리키도록 함
        queue->rear = ptr; // rear를 ptr로 설정
    }
}
void LRU(int refStr[], int pageFrameNum) {
    int order = 0;
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수
    Queue queue; // Queue 구조체 선언
    // UI 
    printf("===== LRU =====\nInput\t\t");
    fprintf(res, "===== LRU =====\nInput\t\t");
    
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
        // queue가 비어있는 경우
        if(isEmpty(&queue))
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            enqueue(&queue, refStr[i]); // 큐에 노드 삽입
            printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
            isPageFault = 0;
        }
        // hit인 경우
        else if((order = searchQueue(&queue, refStr[i])) != 0)
        {
            hitNum++;
            updateHitNode(&queue, order);
            printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
        }
        // miss인 경우
        else
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            if(queue.count < pageFrameNum)
            {
                enqueue(&queue, refStr[i]); // 큐의 맨 뒤에 노드 추가
                printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
            }
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