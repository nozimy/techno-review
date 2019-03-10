/**
 * Студент группы АПО-13 Юнусов Нозимжон
 * */
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


// =============================== MAP

typedef struct dict_t_struct {
    char *key;
    bool value;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(void) {
    return calloc(1, sizeof(dict_t));
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
    d->key = malloc(strlen(key) + 1);
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

//int checkLineType(char *);

bool isExpressionValid(char *);

bool isAssignmentValid(char *);

int getPriority(int);

void freeIfNotNull(char *);

bool isLowLetter(char);

void removeRedundantSpaces(char *, bool);

bool isVarName(char *, int, int);

bool isTrueStr(char *, int);

bool isFalseStr(char *, int);

int findElemInArr(char *, char);


enum Operators {
    Or, Xor, And, Not, Brackets, ClosedBrackets, Identifier
};

typedef struct Node {
    enum Operators operator;
    bool value;
    struct Node *primaryChild;
    struct Node *secondaryChild;
} node_t;

bool resolveNode(node_t *);

void insertNode(node_t **rootNode, node_t *newNode);

int getOperatorType(char *line, int, int);

int main(void) {
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
int runParser() {

    // todo: Read lines to array
    char *line1 = NULL;
    size_t line1Length = 0;
    char *line2 = NULL;
    size_t line2Length = 0;
    char *line3 = NULL;
    size_t line3Length = 0;

    ssize_t charNum1 = getline(&line1, &line1Length, stdin);
//    printf("%s \n", line1);
//    printf("%c \n", line1[charNum1-2]);
//    printf("%zu \n", line1Length);
//    printf("%zd \n", charNum1);
    if (line1[charNum1-2] == ';') {
        ssize_t charNum2 = getline(&line2, &line2Length, stdin);
        if (line1[charNum2-2] == ';') {
            ssize_t charNum3 = getline(&line3, &line3Length, stdin); // todo: not used - remove warning
        }
    }



//    if (line1 == NULL || line2 == NULL || line3 == NULL
//        || charNum1 == -1 || charNum2 == -1 || charNum3 == -1) {
//        freeIfNotNull(line1);
//        freeIfNotNull(line2);
//        freeIfNotNull(line3);
//        return -1;
//    }


//    removeRedundantSpaces(line1, true);
//    removeRedundantSpaces(line2, true);
//    removeRedundantSpaces(line3, false);
//
//    if (!isAssignmentValid(line1)
//        || !isAssignmentValid(line2)
//        || !isExpressionValid(line3)) {
//
//        freeIfNotNull(line1);
//        freeIfNotNull(line2);
//        freeIfNotNull(line3);
//        return -1;
//    }

    bool isError = false;
    bool result = parseLogicalExpression(&isError, line1,
                                         line2, line3);

    if (isError) {
        freeIfNotNull(line1);
        freeIfNotNull(line2);
        freeIfNotNull(line3);
        return -1;
    }

    if (result == true) {
        printf("True");
    } else {
        printf("False");
    }

    freeIfNotNull(line1);
    freeIfNotNull(line2);
    freeIfNotNull(line3);

    return 0;
};

void addVar(dict_t **dict, char *assignLine){

    if (strlen(assignLine) <= 1){
        return;
    }
    int assignOperatorIndex = findElemInArr(assignLine, '=');


    char *var1 = NULL;
           var1 = calloc(1, (size_t) assignOperatorIndex + 1);
    strncpy(var1, assignLine, (size_t) assignOperatorIndex);
    bool val = false;
    if (isTrueStr(assignLine, assignOperatorIndex + 1)) {
        val = true;
    } else if (isFalseStr(assignLine, assignOperatorIndex + 1)) {
        val = false;
    }
    addItem(dict, var1, val);

    free(var1);

}

void freeNodeTree(node_t *rootNode){
    if (rootNode != NULL){
        freeNodeTree(rootNode->primaryChild);
        free(rootNode->secondaryChild);
        free(rootNode);
    }
}

void freeDictTree(dict_t *dict){
    if(dict != NULL){
        freeDictTree(dict->next);
        free(dict->key);
        free(dict);
    }
}

bool parseLogicalExpression(bool *isError,
                            char *line1,
                            char *line2,
                            char *line) {

//    *isError = true;

    dict_t **dict = dictAlloc();

//    removeRedundantSpaces(line1, false);
    if(isAssignmentValid(line1)){
        addVar(dict, line1);
    } else {
        removeRedundantSpaces(line1, false);
        if (isExpressionValid(line1)){
            line = line1;
        } else {
            *isError = true;
        }
    }

//    removeRedundantSpaces(line2, false);
    if(isAssignmentValid(line2)){
        addVar(dict, line2);
    } else {
        removeRedundantSpaces(line2, false);
        if (isExpressionValid(line2)){
            line = line2;
        } else {
            *isError = true;
        }
    }


    removeRedundantSpaces(line, false);
    if (!isExpressionValid(line)){
        *isError = true;
        return false;
    }

//    addVar(dict, line1);
//    addVar(dict, line2);

    node_t *rootNode = NULL;

    int i = 0;
    while (line[i] != '\0' && line[i] != '\n' && line[i] != ';') {
        int tokenStart = i;
        int tokenEnd = i;


        if (line[i] == '(') {
            node_t *newNode = NULL;
            newNode = calloc(1, sizeof(node_t));
            newNode->operator = Brackets;

            insertNode(&rootNode, newNode);

        } else if (line[i] == ')') {
            rootNode->operator = ClosedBrackets;
        } else if (line[i] != ' ') {
            tokenStart = i;
            tokenEnd = i;
            while (line[tokenEnd+1] != ' '
                   && line[tokenEnd+1] != '('
                   && line[tokenEnd+1] != ')'
                   && line[tokenEnd+1] != '\0'
                   && line[tokenEnd+1] != '\n'
                   && line[tokenEnd+1] != ';') {
                tokenEnd++;
            }

            node_t *newNode = NULL;
            newNode = calloc(1, sizeof(node_t));

            switch (getOperatorType(line, tokenStart, -1)) {
                case Not:
                    newNode->operator = Not;
                    break;
                case And:
                    newNode->operator = And;
                    break;
                case Or:
                    newNode->operator = Or;
                    break;
                case Xor:
                    newNode->operator = Xor;
                    break;
                case Identifier:
                    newNode->operator = Identifier;
                    int count = (tokenEnd - tokenStart) + 1;
                    char *varName = NULL;
                            varName = calloc(1, (size_t) count + 1);
                    strncpy(varName, line + tokenStart, (size_t) count);
                    if (isTrueStr(line, tokenStart)) {
                        newNode->value = true;
                    } else if(isFalseStr(line, tokenStart)) {
                        newNode->value = false;
                    } else {
                        newNode->value = getItem(*dict, varName);
                    }
                    free(varName);
                    break;
                default:
                    newNode->operator = Identifier;
                    if (isTrueStr(line, tokenStart)) {
                        newNode->value = true;
                    } else if(isFalseStr(line, tokenStart)) {
                        newNode->value = false;
                    }
                    break;

            }
            insertNode(&rootNode, newNode);

            i = tokenEnd;
        }

        i++;
    }

    bool result = resolveNode(rootNode);

    freeNodeTree(rootNode);

    freeDictTree(*dict);
    dictDealloc(dict);

    return result;
};

void insertNode(node_t **rootNode1, node_t *newNode) {
    node_t *rootNode = *rootNode1;
    node_t *node = NULL;
    node = rootNode;

    if (rootNode != NULL && rootNode->operator == Brackets) {
        node = rootNode->primaryChild;
    }


    if (!node) {
        node = newNode;
    } else if (newNode->operator == Identifier) {

        if (node->primaryChild == NULL) {
            node->primaryChild = newNode;
        } else {
            node->secondaryChild = newNode;
        }
    } else {
        if (node->operator == Identifier) {

            newNode->primaryChild = node;
            node = newNode;
        } else {
            if (getPriority(node->operator) > getPriority(newNode->operator)) {

                newNode->primaryChild = node;
                node = newNode;
            } else {

                if (node->primaryChild == NULL) {
                    node->primaryChild = newNode;
                } else {
                    node->secondaryChild = newNode;
                }
            }
        }

    }

    if (rootNode != NULL && rootNode->operator == Brackets) {
        rootNode->primaryChild = node;
    } else {
        rootNode = node;
    }

    *rootNode1 = rootNode;
}

bool resolveNode(node_t *node) {

    if (node == NULL){
        return false;
    }

    bool r, l = false;

    switch (node->operator) {
        case Identifier:
            return node->value;
        case Not:
            return !resolveNode(node->primaryChild);
        case And:
            r = resolveNode(node->secondaryChild);
            l = resolveNode(node->primaryChild);
            return r && l;
        case Or:
            r = resolveNode(node->secondaryChild);
            l = resolveNode(node->primaryChild);
            return r || l;
        case Xor:
            r = resolveNode(node->secondaryChild);
            l = resolveNode(node->primaryChild);
            return r ^ l;
        case Brackets:
        case ClosedBrackets:
            return resolveNode(node->primaryChild);
    }
    return false;
}

void freeIfNotNull(char *line) {
    if (line != NULL) free(line);
};

void removeRedundantSpaces(char *line, bool removeAll) {
    char *i = line;
    char *j = line;

    while (*j != 0) {
        *i = *j;
        j++;
        if (removeAll){
            if (*i != ' ') i++;
        } else {
            if (*i != ' ' || (*i == ' ' && *(i - 1) != ' ')) i++; // leave only one space between chars
        }
    }

    *i = 0;
}

//int checkLineType(char *line) {
//
//};

bool isExpressionValid(char *line) {

    size_t len = strlen(line);
    if(len <= 1){
        return true;
    }

    int bracketsCount = 0;
    int i = 0;
    int tokenStart = 0;
    int tokenEnd = 0;

    int prevTokenStartIndex = 0;
    int prevTokenEndIndex = 0;

    while (line[i] != '\0' && line[i] != '\n' && line[i] != ';') {
        tokenStart = i;
        tokenEnd = i;


        if (line[i] == '(') {
            bracketsCount++;

            if (i != 0 && prevTokenEndIndex !=0 && prevTokenStartIndex != 0) {
                int operType = getOperatorType(line, prevTokenStartIndex, -1);
                if(operType == -1 || operType == Identifier){
                    return false;
                }
            }

        } else if (line[i] == ')') {
            bracketsCount--;

            if (i != 0 && prevTokenEndIndex !=0 && prevTokenStartIndex != 0) {
                int operType = getOperatorType(line, prevTokenStartIndex, -1);
                if( operType != Identifier){
                    return false;
                }
            }

        } else if (line[i] != ' ') {
            tokenStart = i;
            tokenEnd = i;
            while (line[tokenEnd+1] != ' '
                   && line[tokenEnd+1] != '('
                   && line[tokenEnd+1] != ')'
                   && line[tokenEnd+1] != '\0'
                   && line[tokenEnd+1] != '\n'
                   && line[tokenEnd+1] != ';'
                   && line[tokenEnd+1] != '=') {
                tokenEnd++;
            }


            switch (getOperatorType(line, tokenStart, -1)) {
                case Not:
                    // проверить на наличие переменной или константы после not
                    if (i != 0 && prevTokenEndIndex !=0 && prevTokenStartIndex != 0) {
                        int operType = getOperatorType(line, prevTokenStartIndex, -1);
                        if(operType == Identifier
                        || prevTokenStartIndex != prevTokenEndIndex){ // если не ( и не )
                            return false;
                        }
                    }
                    break;
                case And:
                    // валидные примеры
                    // isdog and iscat
                    // (...) and (...)
                    // True and False
                    // isdog and True, True and iscat
                    // isdog and not iscat, isdog and not True
                    // Слева не может быть оператора
                    // справа может быть только опертор not
                case Or:
                case Xor:
                    if (i != 0 && prevTokenEndIndex !=0 && prevTokenStartIndex != 0) {
                        int operType = getOperatorType(line, prevTokenStartIndex, -1);
                        if(operType != Identifier){
                            return false;
                        }
                    }
                    break;
                case Identifier:
                    // and isdog and
                    // not isdog, not False, not isdog and
                    // isdog isdog isdog
                    //  с двух сторон не может быть переменной или константы
                    if (i != 0 && prevTokenEndIndex !=0 && prevTokenStartIndex != 0) {
                        int operType = getOperatorType(line, prevTokenStartIndex, -1);
                        if(operType == Identifier
                           || (operType == -1 && prevTokenStartIndex != prevTokenEndIndex)){ // если не ( и не )
                            return false;
                        }
                    }
                    break;
                default:
                    // если слово не является ни оператором или переменной или константой
                    return false;
                    break;

            }

            i = tokenEnd;
            prevTokenStartIndex = tokenStart;
            prevTokenEndIndex = tokenEnd;
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
int findElemInArr(char *line, char character) {
    int count = 0;
    int index = -1;
    for (int i = 0; line[i] != '\0'
                    && line[i] != '\n'
                    && line[i] != ';'; i++) {
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

bool isLowLetter(char character) {
    return (character >= 'a') && (character <= 'z');
}

bool compareStr(char *line, int firstIndex, char *str){
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++ ){
        if (line[firstIndex + i] != str[i]) {
            return false;
        }
    }
    return true;
}

bool isTrueStr(char *line, int firstIndex) {
    return compareStr(line, firstIndex, "True");
}


bool isFalseStr(char *line, int firstIndex) {
    return compareStr(line, firstIndex, "False");
}


bool isVarName(char *line, int firstIndex, int lastIndex) {
    for (int i = firstIndex; i <= lastIndex; i++) {
        if (isLowLetter(line[i]) == false) {
            return false;
        };
    }
    return true;
};

int getOperatorType(char *line, int firstIndex, int lastIndex){

    if (lastIndex != -1) {
        if (compareStr(line, lastIndex - 2, "not")){
            return Not;
        } else if (compareStr(line, lastIndex - 2, "and")){
            return And;
        } else if (compareStr(line, lastIndex - 1, "or")){
            return Or;
        } else if (compareStr(line, lastIndex - 2, "xor")){
            return Xor;
        }
    } else {
        if (compareStr(line, firstIndex, "not")){
            return Not;
        } else if (compareStr(line, firstIndex, "and")){
            return And;
        } else if (compareStr(line, firstIndex, "or")){
            return Or;
        } else if (compareStr(line, firstIndex, "xor")){
            return Xor;
        } else if (isVarName(line, firstIndex, lastIndex) || isTrueStr(line, firstIndex) || isFalseStr(line, firstIndex)){
            return Identifier;
        }
    }

    return -1;
}

bool isAssignmentValid(char *line) {

    size_t len = strlen(line);
    if(len <= 1){
        return true;
    }

    int assignOperatorIndex = findElemInArr(line, '=');

    if (assignOperatorIndex == -1) {
        return false;
    }

    if (isVarName(line, 0, assignOperatorIndex - 1) == false) {
        return false;
    }

    if (!(isTrueStr(line, assignOperatorIndex + 1)
          || isFalseStr(line, assignOperatorIndex + 1))) {

        return false;
    }

    return true;
}

/**
 * Get operator's priority
 * @param operator
 * @return Higher a returned number -> higher operator's priority
 */
int getPriority(int operator) {
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
        default:
            return -1;
            break;
    }
};