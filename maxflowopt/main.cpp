#include <stdio.h>
#include <assert.h>
#include <set>
#include <stdlib.h>
short dur[3001][1401], seq[3001][1401], ride[300][1401];
bool move1[3001][1401],  visit[300][3201], use[3201];
int T = 250, ct, c = 10, d = 10, stay[300][15001], ans, seqbo[30001][300];
std::set<int> come[300][15001];
bool send(int a, int b)
{
	if (a == 0)
	{
		if (b == -1)
		{
			for (int i = 0; i < d; i++)
				if (!use[i]&&send(0, i))
				{
					use[i] = 1;
					return 1;
				}
			return 0;
		}
		visit[a][b] = 1;
		for(int j=0;j<seqbo[b][ct];j++)
			if ((!move1[b][seq[b][j]] && dur[b][seq[b][j]] <= ct) && (!visit[dur[b][seq[b][j]]][seq[b][j]] && send(dur[b][seq[b][j]], seq[b][j])))
			{
				move1[b][seq[b][j]] = 1;
				come[a][seq[b][j]].insert(b);
				visit[a][b] = 0;
				return 1;
			}
        visit[a][b]=0;
		return 0;
	}
	if (!ride[a][b])
	{
		ride[a][b]++;
		return 1;
	}
	visit[a][b] = 1;
	if (!visit[a + 1][b] && a < ct && send(a + 1, b))
	{
		stay[a][b]++;
		visit[a][b] = 0;
		return 1;
	}
	if (!visit[a - 1][b] && stay[a - 1][b] && send(a - 1, b))
	{
		stay[a - 1][b]--;
		visit[a][b] = 0;
		return 1;
	}
	for(std::set<int>::iterator i=come[a][b].begin();i!=come[a][b].end();i++)
		if(a==dur[*i][b]&&!visit[0][*i])
			if (move1[*i][b] && send(0, *i))
			{
                come[a][b].erase(*i);
				move1[*i][b] = 0;
				visit[a][b] = 0;
				return 1;
			}
    visit[a][b]=0;
	return 0;
}
int main(int argc, char** argv)
{
	FILE* in = fopen("dur.txt", "r");
	FILE* out = fopen("move.txt", "w");
	fscanf(in, "%d %d", &d, &c);
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < c; j++)
		{
			fscanf(in, "%d", dur[i] + j);
			assert(dur[i][j] > 0);
			assert(dur[i][j] < 250);
		}
		int tmp = 0;
		for (ct = 1; ct <= T; seqbo[i][ct++]=tmp)
			for (int j = 0; j < c; j++)
				if (dur[i][j] == ct)
				{
					seq[i][tmp] = j;
					tmp++;
				}
	}
	for (ct = 1; ct <= T; ct++)
		while (send(0, -1))ans+=(ct==1);
	for (int i = 0; i < d; i++)
    {
    int j;
        for (j = 0; j < c; j++)
			if (move1[i][j])
				goto A;
        j=rand()%c;
  A:      fprintf(out, "%d %d\n",i,j);
    }
    printf("%d %d\n",c,ans);
	return 0;
}
