#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#define MAX_VERTICES 600
#define TRANSFER_WEIGHT 1000
#define False 0
#define True 1

typedef struct GraphType{ // 시간 데이터 저장
    int n; //역 개수
    int weight[MAX_VERTICES][MAX_VERTICES]; //역 이동 시간
    int transfer[MAX_VERTICES][MAX_VERTICES]; //역 환승
} GraphType;

typedef struct Train_name_data{ // 역 데이터 저장
    int n; //총 역수 저장
    char train_code[MAX_VERTICES][30]; // train_code 저장
    char train_name[MAX_VERTICES][30]; // train_name 저장
}Train_name_data;

typedef struct Transfer_data{ // 환승 데이터 저장
    int n; //환승역 갯수
    char code[200][30]; //환승역 코드 저장
    int weight[200][150]; //환승 최대시간 저장
}Transfer_data;

int count_list[20]; // 노선별로 몇개의 역이 있는지 저장
int res = 1;
int transfer_list[MAX_VERTICES]; //환승 데이터 저장
int path[MAX_VERTICES]; //경로를 지나쳤는지 저장
int distance[MAX_VERTICES]; // 거리 데이터 저장
int found[MAX_VERTICES]; //해당 노선을 지나쳤는지 확인
char line_name_data[20][20] = {"1호선","1지선","2호선","2지선","3호선","4호선","5호선","5지선","6호선","7호선","8호선","9호선","분당선","인천1선","중앙선","경춘선","경의선","공항철도"};
int tranfer_time_list[MAX_VERTICES]; //환승했을때 소요시간을 저장하는 배열

GraphType line; //지하철 역 시간 정보 저장
Train_name_data train; //지하철 이름 정보 저장
Transfer_data transfer; //지하철 환승정보 저장

//역이름 데이터 읽기
void read_train_name_file(FILE* pFile){
    char *p;
    char str[1024];
    int v = 0;
    if(pFile != NULL)
        while(v!=553){
            fgets(str,1024,pFile);
            if (v == 0) {
                p = strtok(str,",");p = strtok(NULL,",");
                train.n = atoi(p); //총역수 저장
            }
            else {
                p = strtok(str,",");
                strcpy(train.train_code[v-1],p); //0번째는 train의 code 저장
                p = strtok(NULL,",");
                strcpy(train.train_name[v-1],p); //1번째는 taion의 name 저장
            }
            v++;
        }
    fclose(pFile);
}

//환승정보 데이터 읽기
void read_transfer_file(FILE* pFile){
    char *p;
    char str[1024];
    int h,v = 0;
    if (pFile != NULL)
        while(!feof(pFile)){
            fgets(str,1024,pFile);
            h = 0;
            if (v == 0){
                p = strtok(str,",");p = strtok(NULL,",");
                while(p != NULL){
                    transfer.n++;
                    p = strtok(NULL,",");
                }
            }
            else {
                p = strtok(str,",\n"); //첫 값은 역이름이므로 제외
                strcpy(transfer.code[v-1],p);
                p = strtok(NULL,",");
                while (p != NULL){
                    transfer.weight[v-1][h] = atoi(p); //weight에 0,0 부터 저장하기 위해 v-1을 해줌(why? 첫열에 있는 역이름을 제외하고 weight에 저장하기 위해서)
                    h++; //가로 길이 증가
                    p = strtok(NULL,",");
                }
            }
            v++; //세로 길이 증가
        }
    fclose(pFile);
}

//역데이터 읽기
void init_train(FILE *pFile){
    char *p;
    char str[1024];
    int h,v,init;
    v = init = line.n;
    if(pFile != NULL)
        while(!feof(pFile)){
            fgets(str,1024,pFile);
            h = init;
            p = strtok(str,",");p = strtok(NULL,",");
            if (v == init){
                while (p != NULL){
                    line.n++;
                    p = strtok(NULL,",\n");
                }
            }
            else { //첫줄을 제외한 나머지 값을 weight에 저장
                while (p != NULL){
                    line.weight[v-1][h++] = atoi(p); //weight를 이미 있는 곳 부터 저장하지 않기 위해 v-1을 해줌(why? 첫줄에 있는 역이름을 제외하고 weight에 저장하기 위해서)
                    p = strtok(NULL,",");
                }
            }
            v++; //세로 길이 증가
        }
    for (int i = init;i<line.n;i++) //빈공간은 9999로 채우기(자기 자신의 0빼고 나머지 0을 9999로 채워줌)
        for (int j = 0;j<init;j++){
            line.weight[i][j] = 9999;
            line.weight[j][i] = 9999;
        }
    count_list[res++] = line.n;
    fclose(pFile);
}

// 1~value 사이의 랜덤값을 반환
int random_value (int value){
    return rand()%value + 1; // 1~value 값 사이의 랜덤값을 반환
}

// 환승하는 경로인지 확인
int check_transfer(int i,int j){
    //환승역의 가중치가 환승이면 1반환 아니면 0 반환
    if (line.transfer[i][j] == TRANSFER_WEIGHT) return 1;
    return 0;
}

//현재 진행중인 호선이 몇호선인지 출력
void print_station_name(int idx){
    for (int i=0;i<19;i++)
        if (idx < count_list[i]) {
            printf("-><%s>",line_name_data[i-1]); //해당노선 출력
            break;
        }
}

int choose(int way){
    int i,min,minpos,min_transfer;
    min = INT_MAX;
    min_transfer = INT_MAX;
    minpos = -1;
    if (way == 1){ //최단 경로인 경우
        for (i = 0; i < line.n; i++) 
            if (distance[i] < min && !found[i]){
                min = distance[i];
                minpos = i;
            }
    }
    else{  // 최소 환승인 경우
        for (i = 0; i < line.n; i++){
            if (transfer_list[i] < min_transfer && !found[i]){ //최소환승이 가능한 경우로 먼저 탐색
                min_transfer = transfer_list[i];
                min = distance[i];
                minpos = i;
            }
            else if (transfer_list[i] == min_transfer && !found[i]) //최소환승이 같은경우 최단거리로 탐색
                if (distance[i] < min && !found[i]){
                    min_transfer = transfer_list[i];
                    min = distance[i];
                    minpos = i;
            }
        }
    }
    return minpos;
}

//다익스트라
void shortest_path(int start,int way){
    int i,u,w,random;
    for (i = 0; i<line.n;i++){ // 배열 초기화
        distance[i] = line.weight[start][i];
        transfer_list[i] = line.transfer[start][i];
        found[i] = False;
        path[i] = start;
    }
    found[start] = True;
    distance[start] = 0;
    transfer_list[start] = 0;

    if (way == 1){ // 최단 경로인 경우
        for (i = 0;i<line.n-1;i++){
            u = choose(way);
            found[u] = True;
            for (w=0;w<line.n;w++)
                if (!found[w]){
                    if (check_transfer(w,u)) { //환승역인 경우 환승시간내의 랜덤값을 넣음
                        random = random_value(line.weight[u][w]);
                        if ((distance[u] + random < distance[w])){
                            tranfer_time_list[u] = random; //환승시간저장배열에 환승시간을 저장
                            distance[w] = distance[u] + random;
                            path[w] = u; //path에 동선 저장
                        }
                    }
                    else { //환승이 아닌경우 원래 값을 넣음
                        if ((distance[u] + line.weight[u][w]) < distance[w]){
                            distance[w] = distance[u] + line.weight[u][w];
                            path[w] = u; //path에 동선 저장
                        }
                    }
                }
        }
    }
    else{ //최소 환승인 경우
        for (i = 0;i<line.n-1;i++){
            u = choose(way);
            found[u] = True;
            for (w = 0;w<line.n;w++)
                if(!found[w]){
                    if (check_transfer(w,u)) { //환승역인 경우 환승시간내의 랜덤값을 넣음
                        random = random_value(line.weight[u][w]);
                        if ((transfer_list[u] + line.transfer[u][w]) < transfer_list[w]){ //환승가중치로 먼저 계산한 뒤 환승이 같은경우 거리를 기준으로 계산
                            tranfer_time_list[u] = random; //환승시간저장배열에 환승시간을 저장
                            distance[w] = distance[u] + random;
                            transfer_list[w] = transfer_list[u] + line.transfer[u][w];
                            path[w] = u; //path에 동선 저장
                        }
                        else if ((transfer_list[u] + line.transfer[u][w]) == transfer_list[w]){
                            if ((distance[u] + random) < distance[w]){
                                tranfer_time_list[u] = random; //환승시간저장배열에 환승시간을 저장
                                distance[w] = distance[u] + random;
                                transfer_list[w] = transfer_list[u] + line.transfer[u][w];
                                path[w] = u; //path에 동선 저장
                            }
                        }
                    }
                    else { //환승이 아닌경우 원래 값을 넣음.
                        if ((transfer_list[u] + line.transfer[u][w]) < transfer_list[w]){ //환승가중치로 먼저 계산한 뒤 환승이 같은경우 거리를 기준으로 계산
                            distance[w] = distance[u] + line.weight[u][w];
                            transfer_list[w] = transfer_list[u] + line.transfer[u][w];
                            path[w] = u; //path에 동선 저장
                        }
                        else if ((transfer_list[u] + line.transfer[u][w]) == transfer_list[w]){
                            if ((distance[u] + line.weight[u][w]) < distance[w]){
                                distance[w] = distance[u] + line.weight[u][w];
                                transfer_list[w] = transfer_list[u] + line.transfer[u][w];
                                path[w] = u; //path에 동선 저장
                            }
                        }
                    }
                }
        }
    }
}
//역이름이 존재하는지 확인 있으면 1반환 없으면 0반환
int check_name(char name[]){
    for (int i=0;i<train.n;i++)
        if (strcmp(name,train.train_name[i]) == 0) return 1; //입력받은 이름이 역데이터에 존재하는 이름이면 1반환 아니면 0반환
    return 0;
}

// 지하철 code를 역이름의 index로 반환
int code_to_index(char code[]){
    for (int i=0;i<line.n;i++)
        if (strcmp(code,train.train_code[i]) == 0) return i;
}

// 지하철 이름을 역이름의 index로 반환
int name_to_index(char name[]){
    for (int i=0;i<train.n;i++)
        if (strcmp(name,train.train_name[i]) == 0) return i;
}

// 환승 횟수 배열 생성
void make_transfer_array(){
    int a,b;
    for (int i =0;i<line.n;i++)
        for (int j=0;j<line.n;j++) line.transfer[i][j] = line.weight[i][j]; //환승 가중치 배열 초기화

    for (int i=0;i<transfer.n;i++)
        for (int j=0;j<transfer.n;j++)
            if (transfer.weight[i][j] != 9999 && transfer.weight[i][j] != 0 ){ //환승을 하지 않는 역이 아니거나 자기자신이 아닌경우
                a = code_to_index(transfer.code[i]); //환승데이터의 인덱스를 원래데이터의 인덱스로 변환
                b = code_to_index(transfer.code[j]); //환승데이터의 인덱스를 원래데이터의 인덱스로 변환
                line.weight[a][b] = transfer.weight[i][j]; //원래 데이터에 환승데이터를 합침
                line.transfer[a][b] = TRANSFER_WEIGHT; //환승하는 경우
        }
}

//진행노선 출력
void print_path(int start,int end){
    int a,b,i = 0;
    int count_time = 0,count_transfer_time = 0;
    int li[553];
    li[i] = end;
    while (1){ //노선을 진행순서대로 저장
        li[++i] = path[end];
        end = path[end];
        if (end == start) break;
    }
    printf("<출발>\n");
    for (int j=i;j>=0;j--){
        if (strcmp(train.train_name[li[j]],train.train_name[li[j-1]]) == 0){
            if (j == i) { // start에서 환승을 하는 경우
                print_station_name(li[j-2]); // 무슨 노선인지 출력
                printf("%s",train.train_name[li[j]]); //역이름 출력
            }
            else if (j == 1) { // end에서 환승을 하는 경우
                print_station_name(li[j+1]); // 무슨 노선인지 출력
                count_time += line.weight[li[j+1]][li[j]]; //역 이동시간을 저장
                printf("%s",train.train_name[li[j]]); //역이름 출력
            }
            else {
                count_time += line.weight[li[j+1]][li[j]]; //환승역 까지 가는 시간 저장
                count_transfer_time += tranfer_time_list[li[j]]; //환승시간을 저장
                printf("-><환승 : 소요시간 %d 분> %s",tranfer_time_list[li[j]],train.train_name[li[j]]); //환승 역과 환승소요시간을 출력
            }
            i--; //환승역일 경우 정거장 수 1개 감소
            j--;
        }
        else {
            if (j!=i) count_time += line.weight[li[j+1]][li[j]]; //역 이동시간을 저장
            print_station_name(li[j]); //무슨 노선인지 출력
            printf("%s",train.train_name[li[j]]); //역이름 출력
        }
    }
    printf("<도착>\n");
    printf("\n소요시간 : %d (%d + 환승 소요시간 : %d) 분\n",count_time+count_transfer_time,count_time,count_transfer_time); //소요시간 출력
    printf("정거장 수 : %d 개\n",i+1); //정거장 수 출력
}

//입력받은 역이름이 올바른 형태인지 확인
int check(char start[],char end[]){
    if (!check_name(start) || !check_name(end)) { // 둘 중하나라도 존재하지 않는 역이름인 경우
        printf("존재하지 않는 역 이름입니다. 다시 입력해주세요. \n");
        return 1;
    }
    else if (strcmp(start,end) == 0) { // 두 역의 이름이 같은 경우
        printf("출발역과 도착역의 이름이 같습니다.\n");
        return 1;
    }
    else return 0;
}

int main(){
    char start[20],end[20];
    int way,start_idx,end_idx;
    srand(time(NULL));
    //역 파일 열기
    init_train(fopen("1호선.csv","r"));init_train(fopen("1지선.csv","r"));init_train(fopen("2호선.csv","r"));init_train(fopen("2지선.csv","r"));
    init_train(fopen("3호선.csv","r"));init_train(fopen("4호선.csv","r"));init_train(fopen("5호선.csv","r"));init_train(fopen("5지선.csv","r"));
    init_train(fopen("6호선.csv","r"));init_train(fopen("7호선.csv","r"));init_train(fopen("8호선.csv","r"));init_train(fopen("9호선.csv","r"));
    init_train(fopen("분당선.csv","r"));init_train(fopen("인천1선.csv","r"));init_train(fopen("중앙선.csv","r"));init_train(fopen("경춘선.csv","r"));
    init_train(fopen("경의선.csv","r"));init_train(fopen("공항철도.csv","r"));read_train_name_file(fopen("역이름.csv","r"));read_transfer_file(fopen("환승정보.csv","r"));
    //환승배열 생성
    make_transfer_array();

    while (1){
       printf("출발역을 입력해주세요 : "); //출발역 입력받음
       fgets(start,20,stdin);
       printf("도착역을 입력해주세요 : "); //도착역 입력받음
       fgets(end,20,stdin);
       if (!check(start,end)) break; //출발역과 도착역의 이름이 적절한지 확인
    }
    start_idx = name_to_index(start); //start 이름을 인덱스 값으로 변환
    end_idx = name_to_index(end); //end 이름을 인덱스 값으로 변환
    printf("방식? 1. 최단경로 2. 최소환승\n:"); //방식 입력받음
    scanf("%d",&way);
    if (way == 1){ //방식이 최단경로인 경우
       printf("\n최단경로\n");
       shortest_path(start_idx,way); //최단경로 기준으로 경로탐색
       print_path(start_idx,end_idx); //경로 출력
    }
    else if (way == 2){ //방식이 최소환승인 경우
        printf("\n최소환승\n");
        shortest_path(start_idx,way); //최소환승 기준으로 경로탐색
        print_path(start_idx,end_idx); //경로 출력
    }
    return 0;
}