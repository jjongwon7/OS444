// lfu.c
#include "hw4.h"
// CircularQueue 초기화하는 함수
void initCircularQueue(CircularQueue *circularQueue, int pageFrameNum){
    circularQueue->maxSize = pageFrameNum; // circularQueue의 maxSize를 페이지 프레임 개수로 설정
    circularQueue->curSize = 0; // circularQueue 안의 노드 개수를 0으로 설정
    circularQueue->node = (CNode *)malloc(sizeof(CNode) * pageFrameNum); // 페이지 프레임 개수만큼 node 생성
}
// CircularQueue가 비어있는지 확인하는 함수
int isCircularQueueEmpty(CircularQueue* circularQueue){
    return (circularQueue->curSize == 0); // 큐 안의 노드 개수가 0이라면 빈 상태
}
// CircularQueue에 데이터를 삽입하는 함수
void enqueueCQ(CircularQueue* circularQueue, int data){
    int index;
    if(isCircularQueueEmpty(circularQueue)) // 비어있는 경우
    {
        circularQueue->front = 0; // front 0으로 설정
        circularQueue->rear = 0; // rear 0으로 설정
        (circularQueue->node)[0].data = data; // data 저장
        (circularQueue->node)[0].rBit = 1; // 초기 R bit 1로 설정
    }
    else
    {
        index = (circularQueue->rear+1) % circularQueue->maxSize; // 모듈러 연산을 통하여 rear 다음 노드의 index 구하고
        circularQueue->rear = index; // 구한 index를 rear로 설정
        (circularQueue->node)[index].data = data; // 해당 노드에 data 저장
        (circularQueue->node)[index].rBit = 1; // 초기 R bit 1으로 설정
    }
    circularQueue->curSize++; // 큐 안의 노드 개수를 1 증가
}
// 큐 탐색
int searchCQ(CircularQueue* circularQueue, int data){
    int index; 
    index = circularQueue->front; // index를 front로 설정
    while (1)
    {
        if((circularQueue->node)[index].data == data) // 탐색 성공한 경우
        {
            (circularQueue->node)[index].rBit = 1; // R bit를 1로 갱신
            return 1; // 1 리턴
        }
        if(index == circularQueue->rear) // 탐색 실패한 경우
            break;
        index = (index+1) % circularQueue->maxSize; // 모듈러 연산을 통하여 다음 노드의 index 저장
    }
    return 0; // 실패 시 0 리턴
}
// victim 프레임을 찾아 새로운 페이지로 교체
void replacementCQ(CircularQueue* circularQueue, int data){
    int index;
    index = circularQueue->front; // index를 front로 설정
    // 큐를 순회하면서 R bit -> 1이면 0으로 바꾸고 리스트의 끝으로 이동
    // R bit -> 0이면 프레임 교체
    while (1) 
    {
        if((circularQueue->node)[index].rBit == 0) // R bit가 0인 노드를 찾은 경우
        {
            (circularQueue->node)[index].data = data; // 프레임 갱신
            (circularQueue->node)[index].rBit = 1; // 초기 R bit 1로 설정
            circularQueue->front = (index+1) % circularQueue->maxSize; // front를 다음 노드로 설정
            circularQueue->rear = (index); // rear를 현재 노드로 설정
            return ;
        }
        else // R bit가 1인 경우 0으로 바꾸고 리스트의 끝으로 이동시킴
        {
            (circularQueue->node)[index].rBit = 0;
            circularQueue->front = (index+1) % circularQueue->maxSize; // front를 다음 노드로 설정
            circularQueue->rear = index; // rear를 현재 노드로 설정
        }
        index = (index+1) % circularQueue->maxSize; // 모듈러 연산을 통하여 다음 노드의 index 저장
    }
}
// 페이지 프레임 요소들 출력하는 함수
void printCQ(CircularQueue* circularQueue, int pageFrameNum, int page, int isPageFault){
    int index;
    index = 0;
    printf("%d\t\t", page); // 현재 참조한 페이지 출력
    fprintf(res, "%d\t\t", page);
    while (index < circularQueue->curSize) { // 페이지 프레임 개수만큼 반복
        printf("%d(%d)\t\t", (circularQueue->node)[index].data, (circularQueue->node)[index].rBit);
        fprintf(res, "%d(%d)\t\t", (circularQueue->node)[index].data, (circularQueue->node)[index].rBit);
        index++;
    }
    if(circularQueue->curSize < pageFrameNum) // 페이지 프레임에 빈 공간이 있는 경우
        for(int i=0; i<pageFrameNum-circularQueue->curSize; i++) // 빈 공간은 '-'로 표시
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
void SC(int refStr[], int pageFrameNum) {
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수
    CircularQueue circularQueue;
    // UI 
    printf("===== SC =====\nInput\t\t");
    fprintf(res, "===== SC =====\nInput\t\t");
    
    for(int i=0; i<pageFrameNum; i++)
    {
        printf("Frame%d\t\t",i+1);
        fprintf(res, "Frame%d\t\t", i+1);
    }
    printf("\n");
    fprintf(res, "\n");

    initCircularQueue(&circularQueue, pageFrameNum); // 큐 초기화
    // 페이지 스트링 개수만큼 반복
    for(int i=0; i<STRING_SIZE; i++)
    {
        // queue가 비어있는 경우
        if(isCircularQueueEmpty(&circularQueue))
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            enqueueCQ(&circularQueue, refStr[i]); // 큐에 노드 삽입
            printCQ(&circularQueue, pageFrameNum, refStr[i], isPageFault);
            isPageFault = 0;
        }
        // hit인 경우
        else if(searchCQ(&circularQueue, refStr[i]))
        {
            hitNum++; // hit 개수 + 1
            printCQ(&circularQueue, pageFrameNum, refStr[i], isPageFault);
        }
        // miss인 경우
        else
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            // frame에 빈 공간이 있는 경우
            if(circularQueue.curSize < pageFrameNum)
            {
                enqueueCQ(&circularQueue, refStr[i]); // 큐의 맨 뒤에 노드 추가
                printCQ(&circularQueue, pageFrameNum, refStr[i], isPageFault);
            }
            // frame에 빈 공간이 없는 경우
            else
            {
                replacementCQ(&circularQueue, refStr[i]); // 현재 포인터로부터 탐색하며 R bit가 0인 노드를 찾아 교체
                printCQ(&circularQueue, pageFrameNum, refStr[i], isPageFault);
            }
            isPageFault = 0;
        }
    }
    printf("HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    fprintf(res, "HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    printf("PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
    fprintf(res, "PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
}