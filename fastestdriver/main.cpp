#include <iostream>
#include <stdlib.h>
using namespace std;
int dur[6001][1401], go[30001]={};
int main()
{
    FILE *in=fopen("dur.txt","r");
    FILE *out=fopen("move.txt","w");
    int a,b, ans=0;
    fscanf(in,"%d %d",&a,&b);
    for(int i=0;i<a;i++)
    {
        for(int j=0;j<b;j++)
            fscanf(in,"%d",dur[i]+j);
    }
    for(int j=0;j<b;j++)
    {
        int m=1000,n=rand()%a;
        for(int i=0;i<a;i++)
            if(!go[i]&&m>dur[i][j])
            {
                m=dur[i][j];
                n=i;
            }
        if(m==1)
            ans++;
        go[n]=j+1;
    }
    printf("%d %d\n",b,ans);
    for(int i=0;i<a;i++)
        fprintf(out,"%d %d\n",i,go[i]-1);
    return 0;
}
