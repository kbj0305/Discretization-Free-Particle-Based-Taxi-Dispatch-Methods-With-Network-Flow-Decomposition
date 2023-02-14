#include <iostream>
#include <stdlib.h>
using namespace std;

int main()
{
    FILE *in=fopen("dur.txt","r");
    FILE *out=fopen("move.txt","w");
    int a,b;
    fscanf(in,"%d %d",&a,&b);
    for(int i=0;i<a;i++)
        fprintf(out,"%d %d\n",i,rand()%b);
    return 0;
}
