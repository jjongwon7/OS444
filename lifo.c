// lifo.c
#include "hw4.h"
void LIFO(int refStr[], int pageFrameNum) {
    int frame[pageFrameNum]; // 페이지 프레임
    int frameElementNum = 0; // 프레임에 들어있는 요소 개수
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수
    // UI 
    printf("===== LIFO =====\nInput\t\t");
    fprintf(res, "===== LIFO =====\nInput\t\t");
    
    for(int i=0; i<pageFrameNum; i++)
    {
        printf("Frame%d\t\t",i+1);
        fprintf(res, "Frame%d\t\t", i+1);
    }
    printf("\n");
    fprintf(res, "\n");
    
    // 페이지 스트링 개수만큼 반복
    for(int i=0; i<STRING_SIZE; i++)
    {
        // hit인 경우
        if(isHit(frame, frameElementNum, refStr[i]))
        {
            hitNum++; // hit 개수 + 1
            printFrame(frame, pageFrameNum, frameElementNum, refStr[i], isPageFault);
        }
        // miss인 경우
        else
        {
            isPageFault = 1;
            pageFaultNum++; // page fault 개수 + 1
           // frame에 빈 공간이 있는 경우
            if(frameElementNum < pageFrameNum)
            {
                frame[frameElementNum++] = refStr[i]; // 페이지 프레임에 넣음
                printFrame(frame, pageFrameNum, frameElementNum, refStr[i], isPageFault);
            }
            // frame에 빈 공간이 없는 경우
            else
            {
                frame[frameElementNum-1] = refStr[i]; // 맨 마지막에 들어온 페이지 교체
                printFrame(frame, pageFrameNum, frameElementNum, refStr[i], isPageFault);
            }
            isPageFault = 0;
        }
    }
    printf("HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    fprintf(res, "HIT 개수: %d, HIT 비율: %.1f%%\n", hitNum, (double)hitNum / STRING_SIZE * 100);
    printf("PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
    fprintf(res, "PAGE FAULT 개수: %d, PAGE FAULT 비율: %.1f%%\n", pageFaultNum, (double)pageFaultNum / STRING_SIZE * 100);
}