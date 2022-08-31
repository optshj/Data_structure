#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_STACK_SIZE 100

typedef char element;
typedef struct StackNode{
    element data;
    struct StackNode *link;
}StackNode;

typedef struct{
    StackNode *top;
}LinkedStackType;

void init_stack(LinkedStackType *s){
    s->top = NULL; //top을 null로 설정
}

int is_empty(LinkedStackType *s){
    return (s->top == NULL); //top이 null이면 공백상태이면 1 반환 아니면 0 반환
}

int is_full(LinkedStackType *s){
    return 0; //메모리가 할당할 수 있는 한 스택의 크기의 제한이 없으므로 항상 0 반환
}

void push(LinkedStackType *s,element item){
    StackNode *temp = (StackNode *)malloc(sizeof(StackNode)); //temp 메모리 할당
    temp->data = item; // temp의 data값을 입력받은 item으로 지정
    temp->link = s->top; // temp의 link를 s의 top으로 지정
    s->top = temp; // s의 top을 temp로 지정
}

element pop(LinkedStackType *s){
    if(is_empty(s)){ //비어있는데 pop을 하면 오류 출력
        fprintf(stderr,"스택 공백 에러\n");
        exit(1);
    }
    else{
        StackNode *temp = s->top; //temp를 s의 top값으로 지정
        char data = temp->data; //data를 temp의 data 즉,s의 top의 data로 지정
        s->top = s->top->link; // s의 top을 s의 top의 link로 지정
        free(temp); //tmep 메모리 반환
        return data; //data반환
    }
}

element peek(LinkedStackType *s){
    if (is_empty(s)){ //비어있는데 peek을 하면 오류 출력
        fprintf(stderr,"스택 공백 에러\n");
        exit(1);
    }
    else{
        StackNode *temp = s->top; // 제일위에 있는 데이터를 temp로 지정
        char data = temp->data; //data를 temp의 data로 지정
        return data; // data를 반환
    }
}

//계산식에 우선순위 부여하는 함수(값이 높을수록 먼저 계산)
int prec(char op){
    switch (op){
    case '(' : case ')' : return 1; // 여는괄호 닫는괄호일때 우선순위 1로 지정
    case '+' : case '-' : return 2; // 더하기 빼기일때 우선순위 2로 지정
    case '*' : case '/' : return 3; // 곱하기 나누기일때 우선순위 3으로 지정
    default : return 0;
    }
}

//식에 소괄호가 제대로 되어있는지 판단하는 함수
int check_parenthesis(char exp[]){
    char ch ;
    int len = strlen(exp);
    int check = 0 ; //check라는 정수를 통해 괄호가 제대로 되어있는지 확인 여는괄호를 만나면 1을 더하고 닫는괄호를 만나면 1을 빼주는 방식으로 짝이 맞는지 확인
    for (int i = 0;i < len;i++){
        ch = exp[i];
        if (ch == '(') check ++;
        else if (ch == ')') check --;
        if (check < 0) break; // check가 음수가 되는순간 괄호의 짝이 안맞으므로 더이상 계산할 필요가 없음.
    }   
    if (check == 0) return 0; // check = 0 이면 괄호의 짝이 맞다는 것이므로 0 을 반환 아닌경우 1을 반환하여 메인함수에서 오류로 분류해서 처리함
    else return 1;

}

/*  식에 문자가 있거나 잘못된 식인지 판단하는 함수

    식에 문자가 있는지 (ex : 3+% -> 오류)
    부호가 두번이상 연속으로 나오는지 (ex : 3+*4 -> 오류)
    숫자 앞에 부호가 나오는지 (ex : +3+4 -> 오류, 3+(+4)-> 오류)
    .의 위치가 적절한지 (ex : 3.+4 -> 오류, 3.(4) -> 오류, 3.2.2 -> 오류, 3..2 -> 오류)
    괄호 앞에 문자가 있는지 (ex : 3.1(3.2) -> 오류)
    괄호 안에 숫자가 없는지 (ex : 3.1*() -> 오류)
    check1은 부호가 두번 연속 나오는지 체크
    check2는 괄호가 부적절한 위치에 있는지 체크
    check3은 괄호안에 수식이 있는지 체크
    check4는 .의 위치가 적절한지 체크
    default에서 2를 반환하게 되었다는것은 숫자나 부호나 괄호가 아니라는 것이므로 문자임. 이후 메인함수에서 오류로 분류해서 처리함
    그 외에는 1을 반환하여 올바르지 않은 식으로 간주하여 처리함
*/
int check_word(char exp[]){
    char ch;
    int check1 = 0,check2 = 0,check3 = 0,check4 = 0;
    int len = strlen(exp);
    for (int i = 0; i < len;i++){
        ch = exp[i];
        switch(ch){
            case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' : case '0' :
                if (check3 == 2) return 1; //닫는 괄호 직후 부호 없이 숫자가 먼저 나타나면 오류 반환
                check1 = 0;
                check2 = 1;
                check3 = 0;
                break;
            case '+' : case '-' : case '*' : case '/' :
                if (i == 0) return 1; // 첫번쨰에 부호가 나타나면 오류반환
                check1 ++ ;
                check2 = 0;
                check3 = 0;
                check4 = 0;
                if (check1 > 1) return 1; //부호나 점이 연속 두번 나타나면 오류반환(ex : 2+.3)
                break;
            case '.' :
                if (i == 0) return 1; //첫번째에 .이 나타나면 오류 반환
                check1 ++;
                check4 ++;
                if (check1 > 1) return 1; //부호나 점이 연속 두번 나타나면 오류반환(ex : 2+.3)
                else if (check3 == 2) return 1; //닫는 괄호 직후 부호없이 .이 먼저 나타나면 오류 반환
                else if (check4 > 1) return 1; // . 이 연속으로 두번 이상 나올경우 오류 반환
                break;
            case '(' :
                if (check2 == 1) return 1; //숫자 이후 여는괄호가 바로 나올경우 오류 반환
                check3 = 1;
                break;
            case ')' :
                if (check3 == 1) return 1; // 괄호를 연 직후 바로 닫을경우 오류 반환
                check3 = 2;
                break;
            case ' ':
                return 2;
                break;
            default : return 2; //숫자나 부호 괄호 . 이 아닌경우 오류 반환
        }
    }
    if (check1 != 0) return 1;  //중위식의 끝이 숫자가 아닐경우 오류 반환
    return 0;
}

//중위식을 후위식으로 변경하는 함수
char *infix_to_postfix(char exp[],char postfix[]){
    char ch,top_op;
    int i = 0,j = 0;
    LinkedStackType s;
    init_stack(&s);

    //중위식이 올바른지 확인
    if (check_parenthesis(exp)){
        printf("괄호 오류");
        exit(1);
    }
    if (check_word(exp) == 1){
        printf("올바르지 않은 식");
        exit(1);
    }
    else if (check_word(exp) == 2){
        printf("식에 문자가 있음");
        exit(1);
    }
    while(ch = exp[i++]){
        switch (ch) {
        case '+' : case '-' : case '*' : case '/' :
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))){ //스택이 비어 있지 않고 스택에 있는 부호가 현재 확인중인 부호보다 우선순위가 높으면
                postfix[j++] = ' ';
                postfix[j++] = pop(&s); //스택에 있는 부호를 문자열에 넣음
            }
            push(&s,ch); //스택에 부호를 넣음
            break;
        case '(' :
            push(&s,ch); //여는 괄호를 스택에 넣음 추후에 닫는괄호가 나올때 사용
            break;
        case ')' :
            top_op = pop(&s);
            while (top_op != '('){ //스택에서 닫는괄호가 나올떄 까지
				postfix[j++] = ' ';
                postfix[j++] = top_op;
                top_op = pop(&s);
            }
            break;
        default :
            if(j > 0) postfix[j++] = ' '; //배열에 가장 앞에 들어가는 숫자에서는 공백을 출력하지 않고 그 이후부터 공백 출력
            postfix[j++] = ch;
            while(isdigit(exp[i]) || exp[i] == '.') postfix[j++] = exp[i++]; //문자가 숫자거나 .이면 계속해서 추가 아닐경우 stop
            break;
        }   
    }
    while (!is_empty(&s)){ // 스택에 남아있는 부호를 다 꺼냄
        postfix[j++] = ' ';
        postfix[j++] = pop(&s);
    }
    postfix[j] = '\0'; //문자열에 끝을 표시
    return postfix; //후위식을 반환
}

//중위식을 전위식으로 변경하는 함수
//중위식을 뒤에서 부터 보는 것 이외에는 후위식 변환 함수와 차이점이 없음
char *infix_to_prefix(char exp[],char prefix[]){
    char ch,top_op;
    int i = strlen(exp)-1,j = 0; // 입력받은 중위식의 뒤에서 부터 보기 위해 i값을 strlen(exp) - 1로 지정
    LinkedStackType s;
    init_stack(&s);

    //중위식이 올바른지 확인
    if (check_parenthesis(exp)){
        printf("괄호 오류");
        exit(1);
    }
    if (check_word(exp) == 1){
        printf("올바르지 않은 식");
        exit(1);
    }
    else if (check_word(exp) == 2){
        printf("식에 문자가 있음");
        exit(1);
    }
    while(ch = exp[i--]){
        switch (ch){
        case '+' : case '-' : case '*' : case '/' :
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))){
                prefix[j++] = ' ';
                prefix[j++] = pop(&s);
            }
            push(&s,ch);
            break;
        case ')' :
            push(&s,ch);
            break;
        case '(' :
            top_op = pop(&s);
            while (top_op != ')'){
				prefix[j++] = ' ';
                prefix[j++] = top_op;
                top_op = pop(&s);
            }
            break;
        default :
            if(0 < j) prefix[j++] = ' '; //첫번째 숫자에서는 공백을 출력하지 않고 그 이후부터 공백 출력
            prefix[j++] = ch;
            while(isdigit(exp[i]) || exp[i] == '.') prefix[j++] = exp[i--]; //문자가 숫자거나 .이면 계속해서 추가 아닐경우 stop
            break;
        }
    }
    while (!is_empty(&s)){
        prefix[j++] = ' ';
        prefix[j++] = pop(&s);
    }
    prefix[j] = '\0';
    return prefix;
}
//후위식을 계산하는 함수
double eval(char exp[]){
    double op1, op2;
    char k[100];
    char ch;
    double li[100]; //숫자 배열을 통해 계산
    int j , str_len = 0, len = strlen(exp);
    for(int i = 0; i<len; i++){
        if (str_len < 0) { //오류처리 부분 (숫자 배열의 현재 인덱스가 음수이면 오류 출력)
            printf("수식이 올바르지 않습니다.");
            exit(1);
        }
        ch = exp[i];
        if (ch == ' ') continue; //메인함수에서 공백을 포함해서 받았으므로 공백은 건너뜀.
        else if(isdigit(ch)) {
            k[0] = ch;
            j = 1;
            while(isdigit(exp[i+1]) || exp[i+1] == '.') {//숫자가 아니거나 . 이 아닐때까지 반복하면서 문자열에 넣고 끝나면 문자열을 atof함수를 통해 숫자로 바꾸고 숫자 배열에 저장
                k[j++] = exp[++i];
            }
            k[j] = '\0';
            li[str_len++] = atof(k); //문자열 k를 숫자로 변경
        }
        else if (ch =='+' || ch == '-'|| ch == '*'|| ch == '/'){
            op2 = li[--str_len];
            op1 = li[--str_len];
            switch(ch){
            case '+' : li[str_len++] = op1 + op2; break; //더하기 연산
            case '-' : li[str_len++] = op1 - op2; break; //뺴기 연산
            case '*' : li[str_len++] = op1 * op2; break; //곱하기 연산
            case '/' : li[str_len++] = op1 / op2; break; //나누기 연산
            }
        }
        else { //오류 처리부분 ch가 공백 또는 숫자 또는 부호가 아닌경우 (= 문자인 경우) 오류 반환
            printf("식에 문자가 있습니다.");
            exit(1);
        }
    }
    return li[0]; //계산이 끝났을 경우 0번째에 저장되므로 0번쨰를 반환
}
int main(void){
    char infix[1000],postfix[1000],prefix[1000];

    printf("중위식을 입력하세요 : ");
    scanf("%s",infix);//중위식 입력 받음
    printf("전위식 : ");
    infix_to_prefix(infix,prefix);
    for (int i = strlen(prefix);i >= 0;i--) printf("%c",prefix[i]); //전위식 함수에서 뒤집혀서 나오므로 다시 뒤집어서 출력함
    printf("\n");
    printf("후위식 : ");
    infix_to_postfix(infix,postfix);
    printf("%s\n",postfix);
    printf("계산결과 : %lf",eval(postfix));
    return 0;
}