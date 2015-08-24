#include <stdio.h>
#include <stdlib.h>
struct NumList
{
    int num;
    struct NumList* pNext;
};
int main(int argc, char* argv[])
{
    int i = 0;
    struct NumList* pStart = (struct NumList*)malloc(sizeof(struct NumList));
    struct NumList* pMid = pStart;
    
    // init list
    for(i = 0; i < 5; i++)
    {
        pMid->num = i*2;
        pMid->pNext = (struct NumList*)malloc(sizeof(struct NumList));
        pMid = pMid->pNext;
    }
    pMid->pNext = NULL;
    
    // handle list
    for(i = 0; i < 5; i++)
    {
        
    }
}
