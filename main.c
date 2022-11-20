// main.c
#include "hw4.h"
FILE * res; // 결과를 저장할 파일 포인터

int* createPage() // 랜덤하게 참조 페이지 번호 생성
{
    static int num[STRING_SIZE] = {0};
    for(int i=0; i<STRING_SIZE; i++)
        num[i] = (rand()%PAGE_NUMBER)+1; // 1-30 사이의 난수 생성
    return num;
}

char* createRW() // 랜덤하게 R / W 비트 생성
{
    static char rw[STRING_SIZE] = {0};
    for(int i=0; i<STRING_SIZE; i++)
    {
        switch(rand()%2) 
        {
            case 0:
                rw[i] = 'R';
                break;
            case 1:
                rw[i] = 'W';
        }
    }
    return rw;
}

char* createPageStr() // 랜덤하게 참조 페이지 스트링 생성
{
    static char str[MAX_LEN]; // 참조열을 저장할 배열
    int* num = createPage();
    char* rw = createRW();
    int idx=0;
    for(int cnt=0; cnt<STRING_SIZE; cnt++) // 페이지 스트링 개수만큼 반복하면서
    {
        sprintf(str+idx, "%d", num[cnt]); // 생성한 난수를 str에 저장
        while(num[cnt] != 0) // 생성한 난수의 자릿수 계산하여 index 조정
        {
            num[cnt] = num[cnt]/10;
            ++idx; // 자릿수만큼 index 더해줌
        }
        str[idx++] = ' '; // 입력한 난수 사이에 공백을 넣어 구분
        str[idx++] = rw[cnt]; // RW Bit 저장
        if(cnt != STRING_SIZE - 1)
            str[idx++] = ' '; // 입력한 난수 사이에 공백을 넣어 구분
    }
    return str;
}

void createPageStrFile() { // 랜덤하게 참조 페이지 스트링을 생성하여 파일에 저장
    FILE * fp = fopen("pagestring.txt","w");
    if(fp == NULL) // fopen 에러 처리
    {
        fprintf(stderr, "fopen error\n");
        exit(1);
    }
    else
    {
        fputs(createPageStr(), fp); // 파일에 랜덤한 참조열 작성
        fclose(fp);
    }
}

int main(int argc, const char * argv[]) {
    char * str; // 참조열 저장할 배열
    char strFile[MAX_LEN]; // 파일에 저장된 참조열 저장할 배열
    int refStr[STRING_SIZE] = {0}; // 참조열에 있는 페이지 번호 저장
    char refStrRW[STRING_SIZE] = {0}; // 참조열에 있는 페이지의 R/W Bit 저장
    int pageFrameNum = 0; // 페이지 프레임 개수
    int algorithmNum[3] = {0}; // 입력받은 알고리즘 시뮬레이터 번호 저장
    int cnt = 0; // 입력받은 알고리즘 시뮬레이터 개수 저장
    int isWrong = 0; // 잘못된 값이 들어온 경우
    int dataInputType = 0; // 데이터 입력 방식 저장
    char num[10]; // 알고리즘 시뮬레이터를 입력받을 배열
    char *ptr; // split시에 쓰일 포인터
    int isAll = 0; // All을 선택한 경우
    
    // UI
    while(1) // UI 메뉴 A 문항
    {
        printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오 (최대 3개)\n");
        printf("(1)Optimal (2)FIFO (3)LIFO (4)LRU (5)LFU (6)SC (7)ESC (8)All\n");
        printf("알고리즘 시뮬레이터 선택: ");
        scanf("%[^\n]s", num);
        ptr = strtok(num, " "); // 공백으로 입력받은 문자열 split
        while (ptr != NULL)
        {
            if(cnt < 3) // 최대 3개까지 입력 가능하도록 처리
            {
                algorithmNum[cnt++] = atoi(ptr);
                if(algorithmNum[cnt-1] == 8) // 사용자가 (8)ALL을 입력한 경우
                    isAll = 1;
                else if(algorithmNum[cnt-1] < 1 || algorithmNum[cnt-1] > 8) // 사용자가 1~8 범위 밖의 값을 입력한 경우 처리
                {
                    isWrong = 1;
                    printf("잘못된 값입니다. 1-8 이내의 정수만 입력하세요.\n\n");
                    break;
                }
            }
            else // 입력한 개수가 3을 초과한 경우 처리
            {
                printf("알고리즘은 최소 1개 ~ 최대 3개까지만 입력 가능합니다.\n\n");
                isWrong = 1;
                break;
            }
            ptr = strtok(NULL, " ");
        }
        if(isWrong == 1 || cnt == 0) // 잘못된 값을 입력했거나, 아예 입력하지 않은 경우 재입력 처리
            {
                while(getchar()!= '\n'); // 버퍼 비워줌
                memset(algorithmNum, 0, sizeof(algorithmNum));
                cnt = 0;
                isWrong = 0;
                continue;
            }
        else // 알고리즘 선택을 최소 1개에서 최대 3개까지 올바르게 입력한 경우
            break;
    }
    while(1) // UI 메뉴 B 문항
    {
        printf("B. 페이지 프레임 개수를 입력하시오. (3~10): ");
        scanf("%d", &pageFrameNum);
        if(pageFrameNum >= 3 && pageFrameNum <= 10) // 올바르게 입력한 경우
            break;
        else // 잘못된 값을 입력한 경우 재입력 처리
            printf("3~10 사이의 정수만 입력 가능합니다.\n\n");
        while(getchar()!= '\n'); // 버퍼 비워줌
    }
    while(1) // UI 메뉴 C 문항
    {
        printf("C. 데이터 입력 방식을 선택하시오. (1, 2): ");
        scanf("%d", &dataInputType);
        if(dataInputType == 1 || dataInputType == 2) // 올바르게 입력한 경우
        {
            switch(dataInputType)
            {
                case 1: // 랜덤하게 페이지 스트링 생성
                    str = createPageStr(); // str 배열에 참조열 저장
                    printf("===페이지 스트링===\n");
                    printf("%s\n", str); // 페이지 스트링 출력
                    break;
                case 2: // 참조열이 담긴 파일을 읽어 str 배열에 저장
                    str = strFile;
                    createPageStrFile();
                    FILE * fp = fopen("pagestring.txt","r");
                    if(fp == NULL) // fopen 예외처리
                    {
                        fprintf(stderr, "fopen error\n");
                        exit(1);
                    }
                    fgets(str, MAX_LEN, fp); // str 배열에 참조열 저장
                    fclose(fp);
            }
            break;
        }
        else
            printf("숫자 1 또는 2만 입력 가능합니다.\n\n");
        while(getchar()!= '\n'); // 버퍼 비워줌
    }

    res = fopen("result.txt","w+"); // 결과를 저장할 파일 오픈
    if(res == NULL) // fopen 예외처리
    {
        fprintf(stderr, "fopen error\n");
        exit(1);
    }

    // 참조열 split하여 각 배열에 값 저장
    int isPage = 1;
    int i = 0;
    int j = 0;
    ptr = strtok(str, " ");
    while (ptr != NULL) // 공백 기준으로 주어진 문자열 스플릿
    {
        if(isPage++ % 2)
            refStr[i++] = atoi(ptr); // refStr 배열에 정수 형태로 저장
        else
            refStrRW[j++] = *ptr;
        ptr = strtok(NULL, " ");
    }

    if(isAll) // All을 입력한 경우 모든 알고리즘 실행
    {
        Optimal(refStr, pageFrameNum);
        FIFO(refStr, pageFrameNum);
        LIFO(refStr, pageFrameNum);
        LRU(refStr, pageFrameNum);
        LFU(refStr, pageFrameNum);
        SC(refStr, pageFrameNum);
        ESC(refStr, refStrRW, pageFrameNum);
    }
    else // All이 아닌 1~3개의 알고리즘을 입력한 경우
    {
        int existOptimal = 0;
        for(int i=0; algorithmNum[i]!=0 && i<3 ; i++)
        {
            switch(algorithmNum[i])
            {
                case 1:
                    Optimal(refStr, pageFrameNum);
                    existOptimal = 1;
                    break;
                case 2:
                    FIFO(refStr, pageFrameNum);
                    break;
                case 3:
                    LIFO(refStr, pageFrameNum);
                    break;
                case 4:
                    LRU(refStr, pageFrameNum);
                    break;
                case 5:
                    LFU(refStr, pageFrameNum);
                    break;
                case 6:
                    SC(refStr, pageFrameNum);
                    break;
                case 7:
                    ESC(refStr, refStrRW, pageFrameNum);
            }
        }
        if(existOptimal == 0) // Optimal 알고리즘과 성능 비교하기 위해 Optimal 알고리즘 실행.
            Optimal(refStr, pageFrameNum);
    }
    fclose(res);
    return 0;
}
