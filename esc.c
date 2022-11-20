// esc.c
#include "hw4.h"
// CircularQueue에 데이터를 삽입하는 함수
void enqueueESC(CircularQueue* circularQueue, int data, char * refStrRW, int cur){
    int index;
    if(isCircularQueueEmpty(circularQueue)) // 비어있는 경우
    {
        circularQueue->front = 0; // front 0으로 설정
        circularQueue->rear = 0; // rear 0으로 설정
        (circularQueue->node)[0].data = data; // data 저장
        if(refStrRW[cur] == 'R')
        {
            (circularQueue->node)[0].rBit = 1; // R bit = 1,
            (circularQueue->node)[0].wBit = 0; // W bit = 0으로 설정
        }
        if(refStrRW[cur] == 'W')
        {
            (circularQueue->node)[0].rBit = 1; // R bit = 1,
            (circularQueue->node)[0].wBit = 1; // W bit = 1로 설정
        }
    }
    else
    {
        index = (circularQueue->rear+1) % circularQueue->maxSize; // 모듈러 연산을 통하여 rear 다음 노드의 index 구하고
        circularQueue->rear = index; // 구한 index를 rear로 설정
        (circularQueue->node)[index].data = data; // 해당 노드에 data 저장
        if(refStrRW[cur] == 'R')
        {
            (circularQueue->node)[index].rBit = 1; // R bit = 1,
            (circularQueue->node)[index].wBit = 0; // W bit = 0으로 설정
        }
        if(refStrRW[cur] == 'W')
        {
            (circularQueue->node)[index].rBit = 1; // R bit = 1,
            (circularQueue->node)[index].wBit = 1; // W bit = 1로 설정
        }
    }
    circularQueue->curSize++; // 큐 안의 노드 개수를 1 증가
}
// victim 프레임을 찾아 새로운 페이지로 교체
void replacementESC(CircularQueue* circularQueue, int data, char * refStrRW, int cur){
    int index = circularQueue->front;
    int end = circularQueue->rear;
    // Class0: R=0, W=0
    // Class1: R=0, W=1
    // Class2: R=1, W=0
    // Class3: R=1, W=1
    // Class0 탐색
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<circularQueue->curSize; j++)
        {
            if((circularQueue->node)[index].rBit == 0 && (circularQueue->node)[index].wBit == 0) // R, W bit가 0인 경우
            {
                (circularQueue->node)[index].data = data; // 프레임 갱신
                if(refStrRW[cur] == 'R')
                {
                        (circularQueue->node)[index].rBit = 1; // R bit = 1,
                        (circularQueue->node)[index].wBit = 0; // W bit = 0으로 설정
                }
                if(refStrRW[cur] == 'W')
                {
                    (circularQueue->node)[index].rBit = 1; // R bit = 1,
                    (circularQueue->node)[index].wBit = 1; // W bit = 1로 설정
                }
                circularQueue->front = (index+1) % circularQueue->maxSize; // front를 다음 노드로 설정
                circularQueue->rear = (index); // rear를 현재 노드로 설정
                return ;
            }
            circularQueue->front = (index+1) % circularQueue->maxSize; // front를 다음 노드로 설정
            circularQueue->rear = (index); // rear를 현재 노드로 설정
            index = (index+1) % circularQueue->maxSize; // 모듈러 연산을 통하여 다음 노드의 index 저장
        }
        for(int j=0; j<circularQueue->curSize; j++)
        {
            if((circularQueue->node)[index].rBit == 0 && (circularQueue->node)[index].wBit == 1) // R, W bit가 0,1인 경우
            {
                (circularQueue->node)[index].data = data; // 프레임 갱신
                if(refStrRW[cur] == 'R')
                {
                        (circularQueue->node)[index].rBit = 1; // R bit = 1,
                        (circularQueue->node)[index].wBit = 0; // W bit = 0으로 설정
                }
                if(refStrRW[cur] == 'W')
                {
                    (circularQueue->node)[index].rBit = 1; // R bit = 1,
                    (circularQueue->node)[index].wBit = 1; // W bit = 1로 설정
                }
                circularQueue->front = (index+1) % circularQueue->maxSize; // front를 다음 노드로 설정
                circularQueue->rear = (index); // rear를 현재 노드로 설정
                return ;
            }
            (circularQueue->node)[index].rBit = 0; // R bit = 0으로 설정
            circularQueue->front = (index+1) % circularQueue->maxSize; // front를 다음 노드로 설정
            circularQueue->rear = (index); // rear를 현재 노드로 설정
            index = (index+1) % circularQueue->maxSize; // 모듈러 연산을 통하여 다음 노드의 index 저장
        }
    }
}
// 페이지 프레임 요소들 출력하는 함수
void printESC(CircularQueue* circularQueue, int pageFrameNum, int page, int isPageFault, char * refStrRW, int cur){
    int index;
    index = 0;
    printf("%d(%c)\t\t", page, refStrRW[cur]); // 현재 참조한 페이지 출력
    fprintf(res, "%d(%c)\t\t", page, refStrRW[cur]);
    while (index < circularQueue->curSize) { // 페이지 프레임 개수만큼 반복
        printf("%d(%d,%d)\t\t", (circularQueue->node)[index].data, (circularQueue->node)[index].rBit, (circularQueue->node)[index].wBit);
        fprintf(res, "%d(%d,%d)\t\t", (circularQueue->node)[index].data, (circularQueue->node)[index].rBit, (circularQueue->node)[index].wBit);
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
void ESC(int refStr[], char refStrRW[], int pageFrameNum) {
    CircularQueue circularQueue;
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수

    // UI 
    printf("===== ESC =====\nInput\t\t");
    fprintf(res, "===== ESC =====\nInput\t\t");
    
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
            enqueueESC(&circularQueue, refStr[i], refStrRW, i); // 큐에 노드 삽입
            printESC(&circularQueue, pageFrameNum, refStr[i], isPageFault, refStrRW, i);
            isPageFault = 0;
        }
        // hit인 경우
        else if(searchCQ(&circularQueue, refStr[i]))
        {
            hitNum++; // hit 개수 + 1
            printESC(&circularQueue, pageFrameNum, refStr[i], isPageFault, refStrRW, i);
        }
        // miss인 경우
        else
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
            // frame에 빈 공간이 있는 경우
            if(circularQueue.curSize < pageFrameNum)
            {
                enqueueESC(&circularQueue, refStr[i], refStrRW, i); // 큐의 맨 뒤에 노드 추가
                printESC(&circularQueue, pageFrameNum, refStr[i], isPageFault, refStrRW, i);
            }
            // frame에 빈 공간이 없는 경우
            else
            {
                replacementESC(&circularQueue, refStr[i], refStrRW, i); // 현재 포인터로부터 탐색하며 R bit가 0인 노드를 찾아 교체
                printESC(&circularQueue, pageFrameNum, refStr[i], isPageFault, refStrRW, i);
            }
            isPageFault = 0;
        }
    }
    printf("HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    fprintf(res, "HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    printf("PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
    fprintf(res, "PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
}