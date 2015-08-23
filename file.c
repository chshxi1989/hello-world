#include <stdio.h>
#include <stdlib.h>
struct NumList
{
    int num;
    NumList* pNext;
};
int main(char** argv,int argc)
{
    int i = 0;
    NumList* pStart = malloc(struct NumList);
    Numlist* pMid = pStart;
    
    // init list
    for(i = 0; i < 5; i++)
    {
        pMid->num = i*2;
        pMid->pNext = malloc(struct NumList);
        pMid = pMid->pNext;
    }
    pMid->pNext = NULL;
    
    // handle list
    for(i = 0; i < 5; i++)
    {
        
    }
}
