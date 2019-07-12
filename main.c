// C realization of the Boyer-Moore algorithm
// Created by Nikita Petrin aka KnofL. 2019
// Licensed under Apache License Version 2.0 



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#define DEBUG 0


typedef struct BasicIntList
{
    int data;
    struct BasicIntList *next;
}BasicIntList;

size_t patternLength = 0;
size_t alphabethLength = 256;
size_t baseStringLength = 0;


struct BasicIntList *addToList(struct BasicIntList *list, int data)
{
    struct BasicIntList *newNode = malloc(sizeof(struct BasicIntList));
    newNode->next = NULL;
    newNode->data = data;
    if(list != NULL)
    {
        newNode->next = list;
    }

    return newNode;
}

void clearList(struct BasicIntList *list)
{
    struct BasicIntList *tmp = NULL;
    while(list != NULL)
    {
        tmp = list;
        list = list->next;
        tmp->next = NULL;
        free(tmp);
        tmp = NULL;
    }
}

int findFirstOccurance(char* baseString, char *substring, int *dArray)
{
    size_t str_len = strlen(baseString);
    for(int i = 0; i < str_len - patternLength;) 
    {
        #if DEBUG
            printf ("Current i is: %i; Current str is: %s\n", i, baseString + i);
        #endif
        int j = patternLength - 1;
        for(; j >= 0 && baseString[i + j] == substring[j]; j--);
        if(j < 0){            
            return i;
        }
        else {
            if(j == patternLength - 1)
            {                
                if(dArray[baseString[i + j]] != 0)
                    i += dArray[baseString[i + j]];
                else i += patternLength;
            }else
            {             
                if(dArray[baseString[i + patternLength - 1]] != 0)
                    i += dArray[baseString[i + patternLength - 1]];
                else i += patternLength;
            }
        }
    }

    return -1;
}

struct BasicIntList *findAllOccurances(char* baseString, char *substring, int *dArray)
{
    struct BasicIntList *occurancesList = NULL;
    int occurance = 0;
    int step = 0;
    do
    {
        occurance = findFirstOccurance(baseString + step, substring, dArray);
        if(occurance != -1)
        {
            step += occurance;
            occurancesList = addToList(occurancesList, step);
            step += patternLength;            
        }
    }while(occurance != -1 && step + patternLength < baseStringLength);    

    return occurancesList;
}

int *createDArray(char* baseString, char *substring)
{
    int *d = malloc(sizeof(int) * alphabethLength);
    memset(d, 0, sizeof(int) * alphabethLength);
    for(int i = patternLength - 2; i >= 0; i--)
    {
        if(d[substring[i]] == 0)
            d[substring[i]] = patternLength - i - 1;        
    }

    if(d[substring[patternLength - 1]] == 0)
            d[substring[patternLength - 1]] = patternLength;

    return d;
}

void testFunction(char* baseString, char *substring)
{
    patternLength = strlen(substring);
    baseStringLength = strlen(baseString);

    int *dArray = createDArray(baseString, substring);
    
    struct BasicIntList *occurancesList = findAllOccurances(baseString, substring, dArray);
    struct BasicIntList *tmp = occurancesList;
    if(occurancesList != NULL)
    {
        do
        {
            if(tmp->data != -1)
            {
                printf("Found occurance at %i\n", tmp->data);            
            }
            tmp = tmp->next;
        }while(tmp != NULL);
    }else printf("Found 0 occurances of %s in string: %s\n", substring, baseString);
    clearList(occurancesList);
    occurancesList = NULL;
    free(dArray);
}


int main(int argc, char **argv)
{
    char *baseString = NULL;
    char *substring = NULL;
    if(argc == 2)
    {
        substring = argv[1];
        baseString = "This is the test baseString. It is important to make tests. It's never enough to make one test. test testtest and, yep - tes";
    }
    else if(argc == 3)
    {
        substring = argv[1];
        baseString = argv[2];
    }
    else if(argc == 1)
    {
        substring = "test";
        baseString = "This is the test baseString. It is important to make tests. It's never enough to make one test. test testtest and, yep - tes";
        #if DEBUG
        printf("Initiated\n");
        #endif
    }
    else 
    {
        printf("Bad command line argument\nUsage: SubstringSearch <substring> <baseString>\n");
        return 0;
    }

    testFunction(baseString, substring);    

    return 0;
}