// lfu.c
#include "hw4.h"

// 사용 빈도가 가장 적은 노드 삭제
void dequeueLFU(Queue *queue, int pageCount[], int pageFrameNum) {
    int min = 999; 
    int minPageOrder = 0;
    Node *ptr;
    Node *tmp;
    ptr = queue->front;

    // 사용 빈도가 가장 적은 노드 탐색
    for(int i=0; i<pageFrameNum; i++) // 큐 전체를 순회
    {
        if(pageCount[ptr->data] < min) // min보다 사용 빈도가 적은 페이지인 경우 값 갱신
        {
            min = pageCount[ptr->data];
            minPageOrder = i+1;
        }
        ptr = ptr->next;
    }

    // 탐색한 노드 삭제 (사용 빈도가 같은 경우 FIFO로 처리)
    if(minPageOrder == 1) // 맨 앞의 노드를 삭제하는 경우
    {
        dequeue(queue);
    }
    else if(minPageOrder == pageFrameNum) // 맨 뒤의 노드를 삭제하는 경우
    {
        ptr = queue->front;
        for(int i=1; i<pageFrameNum-1; i++) // next가 마지막 노드를 가리키고 있는 노드를 탐색
        {
            ptr = ptr->next;
        }
        tmp = ptr; // 마지막 노드를 가리키고 있는 노드를 tmp에 저장
        tmp->next = NULL;
        ptr = queue->rear; // ptr에 맨 뒤의 노드를 저장
        free(ptr); // 메모리 해제
        queue->rear = tmp; // rear를 tmp로 설정
    }
    else // 사이에 위치한 노드인 경우
    {
        ptr = queue->front;
        for(int i=1; i<minPageOrder; i++) // next가 목표 노드를 가리키고 있는 노드를 탐색
        {
            if(i == minPageOrder-1) // 원하는 노드를 찾은 경우
                tmp = ptr; // tmp에 해당 노드를 저장
            ptr = ptr->next;
        }
        tmp->next = ptr->next; // tmp가 ptr이 가리키고 있는 노드를 가리키도록 함
        free(ptr); // ptr 메모리 해제
    }
}
void LFU(int refStr[], int pageFrameNum) {
    int pageCount[PAGE_NUMBER+1] = {0}; // 페이지의 사용 횟수 저장할 배열
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수
    int order = 0;
    Queue queue; // Queue 구조체 선언
    // UI 
    printf("===== LFU =====\nInput\t\t");
    fprintf(res, "===== LFU =====\nInput\t\t");
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
                dequeueLFU(&queue, pageCount, pageFrameNum); // 사용 빈도가 가장 적은 노드 삭제
                enqueue(&queue, refStr[i]); // 맨 뒤에 노드 추가
                printQueue(&queue, pageFrameNum, refStr[i], isPageFault);
            }
            isPageFault = 0;
        }
        pageCount[refStr[i]]++; // 해당 페이지 사용 횟수 + 1
    }
    printf("HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    fprintf(res, "HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    printf("PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
    fprintf(res, "PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
}