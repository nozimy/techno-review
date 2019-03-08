/**
 * Студент группы АПО-13 Юнусов Нозимжон
 * */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// =============================== MAP

typedef struct dict_t_struct {
    char *key;
    bool value;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(void) {
    return malloc(sizeof(dict_t));
}

void dictDealloc(dict_t **dict) {
    free(dict);
}

bool getItem(dict_t *dict, char *key) {
    dict_t *ptr;
    for (ptr = dict; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr->value;
        }
    }

    return NULL;
}

void delItem(dict_t **dict, char *key) {
    dict_t *ptr, *prev;
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            if (ptr->next != NULL) {
                if (prev == NULL) {
                    *dict = ptr->next;
                } else {
                    prev->next = ptr->next;
                }
            } else if (prev != NULL) {
                prev->next = NULL;
            } else {
                *dict = NULL;
            }

            free(ptr->key);
            free(ptr);

            return;
        }
    }
}

void addItem(dict_t **dict, char *key, bool value) {
    delItem(dict, key); /* If we already have a item with this key, delete it. */
    dict_t *d = malloc(sizeof(struct dict_t_struct));
    d->key = malloc(strlen(key)+1);
    strcpy(d->key, key);
    d->value = value;
    d->next = *dict;
    *dict = d;
}
//===================================================


/**
 * Задача B-5. Парсер, логические выражения.
 *
 * Требуется написать программу, которая способна вычислять логическе выражения.
 * Допустимые выражения чувствительны к регистру и могут содержать:
 *      1) знаки операций 'and' - и, 'or' - или, 'not' - отрицание, 'xor' - сложение по модулю 2
 *      2) Скобки '(', ')'
 *      3) Логические значения 'True', 'False'
 *      4) Пустое выражение имеет значение "False"
 *
 * Также требуется реализовать возможность задания переменных, которые могут состоять только
 * из маленьких букв латинского алфавита (к примеру, 'x=True').
 * Объявление переменной имеет формат:
 *      <имя>=<True|False>; // допустимы пробелы
 *
 * Допускается несколько объявлений одной и той же переменной, учитывается последнее.
 * Все переменные, используемые в выражении, должны быть заданы заранее. Также запрещены
 * имена, совпадающие с ключевыми словами (and, or и т.д.).
 *
 * Необходимо учитывать приоритеты операций. Пробелы ничего не значат.
 *
 * Если выражение или объявление не удовлетворяют описанным правилам, требуется вывести в
 * стандартный поток вывода "[error]" (без кавычек).
 *
 * Examples:
 *      Input:
 *          iscat=True;
 *          isdog=False;
 *          (not iscat and isdog) or True
 *      Output:
 *          True
 *
 */

/**
 * Описание решения
 *
 */

int runParser();

bool parseLogicalExpression();

void removeSpaces(char*);

int checkLineType(char*);

bool isExpressionValid(char*);

bool isAssignmentValid(char*);

bool calcLogicalExpression(bool*, char*, int);

bool execCommand(bool, bool, int);

int getPriority(int);

void freeIfNotNull(char*);

bool isLowLetter(char);

bool isOperator(char*, int, int);

void removeRedundantSpaces(char*);

bool isVarName(char*, int, int);

bool isTrueStr(char*, int);
bool isFalseStr(char*,int);
int findElemInArr(char*, char);

bool isNotStr(char*, int);
bool isAndStr(char*, int);
bool isXorStr(char*, int);
bool isOrStr(char*, int);



enum Operators{ Or, Xor, And, Not, Brackets, ClosedBrackets};

typedef struct Node {
    bool isIdentifier;
    enum Operators operator;
    bool value;
    struct Node *primaryChild;
    struct Node *secondaryChild;
} node_t;

bool resolveNode(node_t *);
bool insertNode(node_t* rootNode, node_t * newNode);

int main(void)
{
    int errorCode = runParser();

    if (errorCode != 0) {
        printf("[error]");
    }

    return 0;
}


/**
 *  Парсер логических выражений:
 *      Читает данные со стандартного ввода
 *      Позволяет задать булевы переменные перед выражением
 * @return int errorCode
 */
int runParser(){

    // todo: Read lines to array
    char* line1 = NULL;
    size_t line1Length = 0;
    char* line2 = NULL;
    size_t line2Length = 0;
    char* line3 = NULL;
    size_t line3Length = 0;

    ssize_t charNum1 = getline(&line1, &line1Length, stdin);
    ssize_t charNum2 = getline(&line2, &line2Length, stdin);
    ssize_t charNum3 = getline(&line3, &line3Length, stdin);

    if (line1 == NULL || line2 == NULL || line3 == NULL
        || charNum1 == -1 || charNum2 == -1 || charNum3 == -1)  {
        freeIfNotNull(line1);
        freeIfNotNull(line2);
        freeIfNotNull(line3);
        return -1;
    }

    removeSpaces(line1);
    removeSpaces(line2);
    removeRedundantSpaces(line3);

    if (!isAssignmentValid(line1)
        || !isAssignmentValid(line2)
        || !isExpressionValid(line3)) {

        freeIfNotNull(line1);
        freeIfNotNull(line2);
        freeIfNotNull(line3);
        return -1;
    }

    bool isError = false;
    bool result = parseLogicalExpression(&isError, line1, (int)charNum1,
                                         line2, (int)charNum2,line3, (int)charNum3);

    if (isError) {
        freeIfNotNull(line1);
        freeIfNotNull(line2);
        freeIfNotNull(line3);
        return -1;
    }

    if (result == true){
        printf("True");
    } else {
        printf("False");
    }

    freeIfNotNull(line1);
    freeIfNotNull(line2);
    freeIfNotNull(line3);

    return 0;
};

bool parseLogicalExpression(bool* isError,
                            char* assignment1, int size1,
                            char* assignment2, int size2,
                            char* line, int size3){

    dict_t **dict = dictAlloc();

    int assignOperatorIndex = findElemInArr(assignment1, '=');
    char* var1 = malloc((size_t)assignOperatorIndex);
    strncpy(var1, assignment1, (size_t)assignOperatorIndex);
    bool val = false;
    if (isTrueStr(assignment1, assignOperatorIndex+1)) {
        val = true;
    } else if (isFalseStr(assignment1, assignOperatorIndex+1)) {
        val = false;
    }
    addItem(dict, var1, val);

    free(var1);

    assignOperatorIndex = findElemInArr(assignment2, '=');
    char* var2 = malloc((size_t)assignOperatorIndex);
    strncpy(var2, assignment2, (size_t)assignOperatorIndex);
    val = false;
    if (isTrueStr(assignment2, assignOperatorIndex+1)) {
        val = true;
    } else if (isFalseStr(assignment2, assignOperatorIndex+1)) {
        val = false;
    }
    addItem(dict, var2, val);

    free(var2);

//        printf("%s \n", var1);
//        printf("%d \n", getItem(*dict, var1));
//        printf("%s \n", var2);
//        printf("%d \n", getItem(*dict, var2));


    node_t * rootNode = NULL; // =  malloc(sizeof(node_t));
    node_t * prevNode = NULL;
    node_t * currentNode = NULL;
    int bracesCount = 0;

    int i = 0;
    while (line[i] != '\0' && line[i] !='\n' && line[i] != ';') {
        int tokenStart = 0;
        int tokenEnd=0;



        if (line[i] == '(') {
            node_t * newNode = malloc(sizeof(node_t));
            newNode->operator = Brackets;
            bracesCount++;

            if (rootNode == NULL) {
                rootNode = newNode;
                prevNode = rootNode;
            } else if (currentNode == NULL) {
                currentNode = newNode;
                prevNode->primaryChild = currentNode;
            } else {
                prevNode = currentNode;
                currentNode = newNode;
                prevNode->primaryChild = currentNode;
            }
        } else  if (line[i] == ')') {
            currentNode = rootNode;
            bracesCount--;
        } else if (line[i] != ' ') {
            tokenStart = i;
            tokenEnd = i;
            while (line[tokenEnd] != ' '
                   && line[tokenEnd] != '('
                   && line[tokenEnd] != ')'
                   && line[tokenEnd] != '\0'
                   && line[tokenEnd] != '\n'
                   && line[tokenEnd] != ';') {
                tokenEnd++;
            }

            node_t* newNode =  malloc(sizeof(node_t));

            if (isOperator(line, tokenStart, -1)) {
                newNode->isIdentifier = false;
                if (isNotStr(line, tokenStart)) {
                    newNode->operator = Not;

                    if (rootNode == NULL) {
                        rootNode = newNode;
                        prevNode = newNode;
                    } else if (currentNode == NULL) {
                        currentNode = newNode;
                        prevNode->primaryChild = currentNode;
                    } else {
                        prevNode = currentNode;
                        currentNode = newNode;
                        prevNode->primaryChild = currentNode;
                    }
                }
                else if (isAndStr(line, tokenStart)) {
                    newNode->operator = And;

                    if (getPriority(newNode->operator) < getPriority(currentNode->operator)) {
                        newNode->primaryChild = currentNode;
                        currentNode = newNode;
                        prevNode->primaryChild = currentNode;
                    } else {

                    }
                }
                else if (isXorStr(line, tokenStart)) {
                    newNode->operator = Xor;
                }
                else if (isOrStr(line, tokenStart)) {
                    newNode->operator = Or;

                    if (getPriority(newNode->operator) < getPriority(currentNode->operator)) {
                        newNode->primaryChild = currentNode;
                        currentNode = newNode;
                    } else {

                    }
                }
            } else {
                newNode->isIdentifier = true;
                int count = (tokenEnd - tokenStart) + 1;
                char* varName = malloc((size_t) count);
                strncpy(varName, line, (size_t) count);
                newNode->value = getItem(*dict, varName);
                free(varName);

                if (rootNode == NULL) {
                    rootNode = newNode;
                    prevNode = newNode;
                } else if (currentNode == NULL) {
                    rootNode->secondaryChild = newNode;
                } else {
                    currentNode->secondaryChild = newNode;
                }

            }

//                if (line[tokenEnd] != ')') {
//                    i = tokenEnd;
//                } else {
//                    i = tokenEnd-1;
//                }
        }

        i++;
    }

//        free(rootNode);
    free(prevNode);

    bool result = resolveNode(currentNode);

    printf("\n" );
    printf("result %d \n", result);
    printf("bracesCount %d \n", bracesCount);


    dictDealloc(dict);

    return result;
};

bool insertNode(node_t * rootNode, node_t * newNode){
    if (rootNode == NULL) {
        rootNode = newNode;
    } else if (rootNode->operator == Brackets) {

        node_t * node = rootNode->primaryChild; // todo: Правильно ли я делаю операции с указателями? не теряю ли данные. Нужно использовать указатель на указатель?

        if (node == NULL) {
            node = newNode;
        } else
        if (newNode->isIdentifier) {

            if (node->primaryChild == NULL){
                node->primaryChild = newNode;
            } else {
                node->secondaryChild = newNode;
            }
        } else {
            if (node->isIdentifier){

                newNode->primaryChild = node;
                node = newNode;
                newNode = NULL;
            } else {
                if(getPriority(node->operator) > getPriority(newNode->operator)){

                    newNode->primaryChild = node;
                    node = newNode;
                    newNode = NULL;
                } else {

                    if (node->primaryChild == NULL){
                        node->primaryChild = newNode;
                    } else {
                        node->secondaryChild = newNode;
                    }
                }
            }

        }
    } else {

        if (newNode->isIdentifier) {

            if (rootNode->primaryChild == NULL){
                rootNode->primaryChild = newNode;
            } else {
                rootNode->secondaryChild = newNode;
            }
        } else {
            if (rootNode->isIdentifier){

                newNode->primaryChild = rootNode;
                rootNode = newNode;
                newNode = NULL;
            } else {
                if(getPriority(rootNode->operator) > getPriority(newNode->operator)){

                    newNode->primaryChild = rootNode;
                    rootNode = newNode;
                    newNode = NULL;
                } else {

                    if (rootNode->primaryChild == NULL){
                        rootNode->primaryChild = newNode;
                    } else {
                        rootNode->secondaryChild = newNode;
                    }
                }
            }

        }
    }
}

bool resolveNode(node_t * node){

    printf("\n" );

    if (node->operator) {
        printf("operator %d\n", node->operator);
    }
    if (node->primaryChild) {
        printf("primaryChild operator %d\n", node->primaryChild->operator);
    }
    if (node->secondaryChild) {
        printf("secondaryChild operator %d\n", node->secondaryChild->operator);
    }
    printf("isIdentifier %d\n", node->isIdentifier);


    if (node->isIdentifier) {
        return node->value;
    }

    bool r, l = false;

    switch (node->operator) {
        case Not:
            printf("Not \n");
            return !resolveNode(node->secondaryChild);
        case And:
            printf("And \n");
            r = resolveNode(node->secondaryChild);
            l = resolveNode(node->primaryChild);
            return  r && l;
        case Or:
            printf("Or \n");
            r = resolveNode(node->secondaryChild);
            l = resolveNode(node->primaryChild);
            return r || l;
        case Xor:
            printf("Xor \n");
            r = resolveNode(node->secondaryChild);
            l = resolveNode(node->primaryChild);
            return r ^ l;
        case Brackets:
            printf("Brackets \n");
            return resolveNode(node->primaryChild);
    }
}

void freeIfNotNull(char* line){
    if (line != NULL) free(line);
};

void removeSpaces(char* line){
    char* i = line;
    char* j = line;

    while(*j != 0) {
        *i = *j++;
        if(*i != ' ') i++;
    }

    *i = 0;
};

void removeRedundantSpaces(char* line){
    char* i = line;
    char* j = line;

    while(*j != 0) {
        *i = *j;
        j++;
        if(*i != ' ' || (*i == ' ' && *(i-1) != ' ') ) i++;
    }

    *i = 0;

}

bool isVarName(char* line, int firstIndex, int lastIndex){
    for(int i = firstIndex; i <= lastIndex; i++){
        if(isLowLetter(line[i]) == false){
            return false;
        };
    }
    return true;
};

int checkLineType(char* line){

};

// todo: add chekcc rules: "a a a ", "and and and", "a and and", "and not and", "a a and a", "and a and a"
bool isExpressionValid(char* line){

    int bracketsCount = 0;

    int prevChar = 0;
    int nextChar = 0;

    int start, end = 0;

    int i = 0;
    while (line[i] != '\0' && line[i] !='\n' && line[i] != ';') {

        switch (line[i]) {
            case ' ':
                while (line[i+1] == ' ') {
                    i++;
                }
                break;
            case '(':
                if(i != 0){
                    prevChar = i-1;
                    if (line[prevChar] == ' ') {
                        prevChar--;
                    }
                    if (!isOperator(line, -1, prevChar)){
                        return false;
                    }
                }
                bracketsCount++;
                break;
            case ')':
                if (bracketsCount >= 1) {
                    bracketsCount--;
                } else {
                    return false;
                }
                prevChar = i-1;
                nextChar = i+1;
                if (line[prevChar] == ' ') {
                    prevChar--;
                }
                if (line[nextChar] == ' ') {
                    nextChar++;
                }
                if (!isOperator(line, nextChar, -1) || isOperator(line, -1, prevChar)) {
                    return false;
                }
                break;
            default:
                if (isLowLetter(line[i]) == false &&
                    line[i] != 'T' &&
                    line[i] != 'F') {
                    return false;
                } else {
                    start = i;
                    end = i;
                    while ((line[start-1] != ' '
                            && line[start-1] != '('
                            && line[start-1] != ')')
                           && start > 0) {
                        start--;
                    }
                    while (
                            (
                                    line[end+1] != ' '
                                    && line[end+1] != '('
                                    && line[end+1] != ')'
                            )
                            &&
                            (
                                    line[end+1] != '\0'
                                    && line[end+1] !='\n'
                                    && line[end+1] != ';'
                            )
                            ) {
                        end++;
                    }
                    if (!isVarName(line, start, end)
                        && !isTrueStr(line, start)
                        && !isFalseStr(line, start)) {
                        return false;
                    }
                }
                break;
        }

        i++;
    }

    if (bracketsCount != 0) {
        return false;
    }

    return true;
};

/**
 * Возвращает индекс элемента только если этот
 * элемент встречается ровно один раз в строке,
 * иначе возвращает -1
 * @param line
 * @param character
 * @return
 */
int findElemInArr(char* line, char character){
    int count = 0;
    int index = -1;
    for(int i =0; line[i] != '\0'
                  && line[i] !='\n'
                  && line[i] != ';'; i++){
        if (line[i] == character) {
            index = i;
            count++;
        };
    }
    if (count != 1) {
        return -1;
    }
    return index;
}

bool isLowLetter(char character){
    return (character >= 'a') && (character <= 'z');
}

bool isTrueStr(char* line, int firstIndex){
    if (line[firstIndex] == 'T' &&
        line[firstIndex+1] == 'r' &&
        line[firstIndex+2] == 'u' &&
        line[firstIndex+3] == 'e') {
        return true;
    }
    return false;
}

bool isFalseStr(char* line, int firstIndex){
    if (line[firstIndex] == 'F' &&
        line[firstIndex+1] == 'a' &&
        line[firstIndex+2] == 'l' &&
        line[firstIndex+3] == 's' &&
        line[firstIndex+4] == 'e') {
        return true;
    }
    return false;
}

bool isNotStr(char* line, int firstIndex){
    if (line[firstIndex] == 'n' &&
        line[firstIndex+1] == 'o' &&
        line[firstIndex+2] == 't') {
        return true;
    }
    return false;
}

bool isOrStr(char* line, int firstIndex){
    if (line[firstIndex] == 'o' &&
        line[firstIndex+1] == 'r') {
        return true;
    }
    return false;
}

bool isXorStr(char* line, int firstIndex){
    if (line[firstIndex] == 'x' &&
        line[firstIndex+1] == 'o' &&
        line[firstIndex+2] == 'r') {
        return true;
    }
    return false;
}

bool isAndStr(char* line, int firstIndex){
    if (line[firstIndex] == 'a' &&
        line[firstIndex+1] == 'n' &&
        line[firstIndex+2] == 'd') {
        return true;
    }
    return false;
}

bool isOperator(char* line, int firstIndex, int lastIndex){
    if (firstIndex != -1) {
        if (isNotStr(line, firstIndex)
            || isAndStr(line, firstIndex)
            || isOrStr(line, firstIndex)
            || isXorStr(line, firstIndex)) {
            return true;
        }
    } else if (lastIndex != -1) {
        if (isNotStr(line, lastIndex-2)
            || isAndStr(line, lastIndex-2)
            || isOrStr(line, lastIndex-1)
            || isXorStr(line, lastIndex-2)) {
            return true;
        }
    }


    return false;
}

bool isAssignmentValid(char* line){

    int assignOperatorIndex = findElemInArr(line, '=');

    if (assignOperatorIndex == -1) {
        return false;
    }

    if (isVarName(line, 0, assignOperatorIndex-1) == false) {
        return false;
    }

//    for(int i = 0; i < assignOperatorIndex; i++){
//        if(isLowLetter(line[i]) == false){
//            return false;
//        };
//    }

    if (!(isTrueStr(line, assignOperatorIndex+1)
          || isFalseStr(line, assignOperatorIndex+1))) {

        return false;
    }

    return true;
};;

bool execCommand(bool a, bool b, int operator){

};

/**
 * Get operator's priority
 * @param operator
 * @return Higher a returned number -> higher operator's priority
 */
int getPriority(int operator){
    switch (operator) {
        case Or:
            return 0;
        case And:
            return 1;
        case Xor:
            return 2;
        case Not:
            return 3;
        case Brackets:
        case ClosedBrackets:
            return 4;
        default:break;
    }
};

bool calcLogicalExpression(bool* isError, char* expression, int size){

};