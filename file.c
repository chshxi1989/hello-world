#include <stdio.h>
#include <stdlib.h>
struct NumList
{
    int num;
    struct NumList* pNext;
};

// print list function
void printList(struct NumList* pStart)
{
    struct NumList* pMid = pStart;
    while(pMid != NULL)
    {
        printf("%d\n", pMid->num);
        pMid = pMid->pNext;
    }
}

int main(int argc, char* argv[])
{
   if(argc != 2)
    {
        printf("arguments error\n");
        printf("Usage:\n");
        printf("      file [num]\n");
    }
    int num = atoi(argv[1]);
    printf("argv[1]:%s, num:%d\n", argv[1], num);
    if(num <= 0)
    {
        printf("num error , must bigger than 0\n");
        return -1;
    }
    int i = 0;
    struct NumList* pStart = (struct NumList*)malloc(sizeof(struct NumList));
    struct NumList* pMid = pStart;
    struct NumList* pNext = NULL;
    // init list
    for(i = 0; i < num - 1; i++)
    {
        pMid->num = i*2;
        pMid->pNext = (struct NumList*)malloc(sizeof(struct NumList));
        pMid = pMid->pNext;
    }
    pMid->num = i*2;
    pMid->pNext = NULL;

    // print list
    printf("\nlist value:\n");
    printList(pStart);

    // invert list
    if(num == 1)
    {
        // not implement
    }
    else if(num == 2)
    {
        pMid = pStart->pNext;
        pStart->pNext = NULL;
        pMid->pNext = pStart;
        pStart = pMid;
    }
    else if(num > 2)
    {
        pMid = pStart->pNext;
        pNext = pMid->pNext;
        pStart->pNext = NULL;
        while(pNext->pNext != NULL)
        {
            pMid->pNext = pStart;
            pStart = pMid;
            pMid = pNext;
            pNext = pMid->pNext;
        }
        pMid->pNext = pStart;
        pNext->pNext = pMid;
        pStart = pNext;
    }

    // print List
    printf("\nList value after invert:\n");
    printList(pStart);

    // free list
    while(pStart->pNext != NULL)
    {
        pMid = pStart->pNext;
        free(pStart);
        pStart = pMid;
    }
    free(pStart);
    return 0; 
}
