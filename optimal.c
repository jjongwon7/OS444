// optimal.c
#include "hw4.h"

// Hit인지 확인하여 hit -> 1, miss -> 0 리턴하는 함수
int isHit(int frame[], int frameElementNum, int page) {
    for(int i=0; i<frameElementNum; i++) // 페이지 프레임에 들어있는 요소 개수만큼 반복
    {
        if(frame[i] == page) // 페이지 프레임에 접근하고자 하는 페이지가 존재하는 경우(hit인 경우)
            return 1; // hit라면 1 리턴
    }
    return 0; // miss라면 0 리턴
}
// 페이지 프레임 요소들 출력하는 함수
void printFrame(int frame[], int pageFrameNum, int frameElementNum, int page, int isPageFault) {
    printf("%d\t\t", page); // 현재 참조한 페이지 출력
    fprintf(res, "%d\t\t", page);

    for(int i=0; i<frameElementNum; i++) // 현재 페이지 프레임에 들어있는 요소 개수만큼 반복
    {
        printf("%d\t\t",frame[i]);
        fprintf(res, "%d\t\t",frame[i]);
    }
    
    if(frameElementNum < pageFrameNum) // 페이지 프레임에 빈 공간이 있는 경우
        for(int i=0; i<pageFrameNum-frameElementNum; i++) // 빈 공간은 '-'로 표시
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
// 교체할 Victim 찾는 함수
int findVictim(int frame[], int refStr[], int pageFrameNum, int cur) {
    int maxLenFrame = 0; // 가장 오랫동안 쓰이지 않는 페이지 프레임 인덱스 값
    int maxLen = 0;
    int len = 0;
    for(int i=0; i<pageFrameNum; i++) // 페이지 프레임 개수만큼 반복
    {
        for(int j=cur; j<STRING_SIZE; j++) // 현재 참조하는 페이지부터 참조열의 끝까지 탐색
        {
            len++;
            if(frame[i] == refStr[j]) // 페이지 프레임에 들어있는 값을 찾은 경우 탐색 완료
                break;
        }
        if(maxLen < len) // 기존에 찾았던 것 보다 더 오랫동안 쓰이지 않는 경우 값 갱신
        {
            maxLen = len;
            maxLenFrame = i;
        }
        len = 0;
    }
    return maxLenFrame;
}
void Optimal(int refStr[], int pageFrameNum) {
    int frame[pageFrameNum]; // 페이지 프레임
    int frameElementNum = 0; // 프레임에 들어있는 요소 개수
    int victim = 0; // victim
    int isPageFault = 0; // page fault인지 확인
    int hitNum = 0; // hit 개수
    int pageFaultNum = 0; // page fault 개수
    // UI 
    printf("===== Optimal =====\nInput\t\t");
    fprintf(res, "===== Optimal =====\nInput\t\t");
    
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
                victim = findVictim(frame, refStr, pageFrameNum, i); // Victim을 찾고
                frame[victim] = refStr[i]; // 찾은 victim 교체
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