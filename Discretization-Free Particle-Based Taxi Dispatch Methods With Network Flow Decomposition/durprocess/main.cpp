#include <stdio.h>
#include <assert.h>
#include <queue>
using namespace std;
    short dur[40000000]={}, go[6001000], went[6001000], g[6001], w[14001];
int c = 10, d = 10, stay[300][1501], ans, seqbo[60001][300];
short result[40000000];
void calc(int t,int d, int c, short* dur, short* go, short* went, short* result)
    {
for(int i=t;i<c;i+=10){
    bool visit[100000]={};
        priority_queue<pair<int, int> > q;
        q.push(make_pair(0,i+50000));
        //printf("%d\n",i);
        while(!q.empty())
        {
            pair<int, int> a=q.top();
            q.pop();
            if(visit[a.second])
                continue;
            //printf("%d\n",-a.first);
            visit[a.second]=1;;
            if(a.second>=50000)
            {

                for(int k=0;went[(a.second-50000)*d+k]&&k<d;k++)
                    if(dur[(went[(a.second-50000)*d+k]-1)*c+a.second-50000]-a.first<249)
                        q.push(make_pair(a.first-dur[(went[(a.second-50000)*d+k]-1)*c+a.second-50000],went[(a.second-50000)*d+k]-1));
                continue;
            }
            result[a.second*c+i]=-a.first-249;
            for(int j=0;go[a.second*c+j]&&j<c;j++)
                if(dur[a.second*c+go[a.second*c+j]-1]-a.first<249)
                    q.push(make_pair(a.first-dur[a.second*c+go[a.second*c+j]-1],go[a.second*c+j]-1+50000));
        }
        //printf("%d\n",i);
        }

    }
int main(int argc, char** argv)
{
 FILE* in = fopen("dur.txt", "r");
	fscanf(in, "%d %d", &d, &c);
	for (int i = 0; i < d; i++)
		for (int j = 0; j < c; j++)
		{
			fscanf(in, "%d", dur+i*c+j);
			if(dur[i*c+j]<249)
			{
                go[i*c+g[i]]=j+1;
                went[j*d+w[j]]=i+1;
                g[i]++;
                w[j]++;
            }
			if(dur[i*c+j]<=0)
                printf("%d %d\n",i,j);
			assert(dur[i*c+j] > 0);
			assert(dur[i*c+j] < 250);
		}
    fclose(in);
    for(int x=0;x<10;x++)
    {
        calc(x,d,c,dur,go,went,result);
    };
    FILE* out = fopen("dur.txt", "w");
    fprintf(out,"%d %d\n",d,c);
    for(int i=0;i<d;i++)
        for(int j=0;j<c;j++)
            fprintf(out,"%d\n",result[i*c+j]<0?result[i*c+j]+249:249);
	return 0;
}
