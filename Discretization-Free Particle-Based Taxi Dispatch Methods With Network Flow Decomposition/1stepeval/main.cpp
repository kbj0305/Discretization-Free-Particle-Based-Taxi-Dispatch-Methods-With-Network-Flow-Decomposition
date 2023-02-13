#include <iostream>
using namespace std;
int dur[10000][5000], take[10000];
int main()
{
    FILE *in=fopen("dur.txt","r");
    FILE *in2=fopen("move.txt","r");
    int d,c;
    fscanf(in,"%d %d",&d,&c);
    for(int i=0;i<d;i++)
        for(int j=0;j<c;j++)
            fscanf(in,"%d",dur[i]+j);
    for(int i=0;i<d;i++)
    {
        int a,b;
        fscanf(in2,"%d %d",&a,&b);
        if(dur[a][b]==1)
            take[b]=1;
    }
    int ans=0;
    for(int i=0;i<c;i++)
        ans+=take[i];
    printf("%d",ans);
    return 0;
}
