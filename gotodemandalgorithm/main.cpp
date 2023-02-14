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
    {
        int m=200, dur[10000];
        for(int j=0;j<b;j++)
        {
            fscanf(in,"%d",dur+j);
            m=m>dur[j]?dur[j]:m;
        }
        int x;
        do
        x=rand()%b;
        while(dur[x]>m);

        fprintf(out,"%d %d\n",i,x);
    }
    return 0;
}
