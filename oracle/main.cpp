#include <iostream>
#include <string.h>
#define cofL 0.793
#define degtoT 45
#include <math.h>
#include <stdlib.h>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
using namespace std;
char idex[1000][100];
double idpos[1000][2];
double driverpos[50000][2];
int onservice[50000],n;
int c[200], ct, pr[200][15000], len[200][15000], dcnt[200][50000];
double st[200][15000][2], de[200][15000][2];
void position(double* p, int i)
{
    double a,b;
    A:a=idpos[i][0]+(rand()%32768-16384)/819200.0;
    b=idpos[i][1]+(rand()%32768-16384)/819200.0;
    for(int j=0;j<n;j++)
        if((a-idpos[j][0])*(a-idpos[j][0])+(b-idpos[j][1])*(b-idpos[j][1])+0.000000001<(a-idpos[i][0])*(a-idpos[i][0])+(b-idpos[i][1])*(b-idpos[i][1]))
            goto A;
    p[0]=a;
    p[1]=b;
}
int main()
{
    IloEnv env;
    int d=0, n_taken=0, total_order=0;
    long long int totalincome=0, potential_income=0;
    FILE *id=fopen("index.txt","r");
    FILE *dr=fopen("new_20181023_idle_driver_seoul7.txt", "r");
    FILE *order=fopen("new_20181023_seoul7.txt","r");
    int a,b;
    char node[100];
    double p1,p2;
    while(fscanf(id,"%s %lf %lf",&node,&p1,&p2)!=-1)
    {
        strcpy(idex[n],node);
        idpos[n][0]=p1;
        idpos[n][1]=p2;
        n++;
    }
    while(fscanf(dr,"%d %s %d",&a,&node,&b)!=-1)
    {
        if(a==0)
        {
            int i;
            for(i=0;i<n;i++)
                if(strcmp(idex[i],node)==0)
                    break;
            if(i==n)
                continue;
            for(int j=0;j<b;j++)
            {
                if(rand()%64)continue;
                position(driverpos[d],i);
                d++;
            }
        }
    }
    fclose(id);
    fclose(dr);
    char start[100], dest[100];
    int duration, price;

    try{
    IloModel model(env);
    IloNumVarArray* x = new IloNumVarArray[144];
    IloNumVarArray** m = new IloNumVarArray*[144];
    IloNumVarArray*** m1 = new IloNumVarArray**[144];
    IloExpr obj(env);
    for(int t=0;t<144;t++)
    {
        while(duration||(fscanf(order,"%s %s %d %d %d",&start,&dest,&ct,&duration,&price)!=-1&&ct==t))
        {
            int i;
            for(i=0;i<n;i++)
                if(strcmp(idex[i],start)==0)
                    break;
            if(i==n||rand()%64)
            {
                duration=0;
                continue;
            }
            position(st[t][c[t]],i);
            for(i=0;i<n;i++)
                if(strcmp(idex[i],dest)==0)
                    break;
            if(i==n)
            {
                duration=0;
                continue;
            }
            position(de[t][c[t]],i);
            len[t][c[t]]=duration;
            pr[t][c[t]]=price;
            total_order++;
            potential_income+=price;
            c[t]++;
            duration=0;
        }
        x[t]=IloNumVarArray(env);
        m[t]=new IloNumVarArray[c[t]];
        m1[t]=new IloNumVarArray*[c[t]];
		for (int i = 0; i < c[t]; i++)
		{
			x[t].add(IloNumVar(env,0,1));
			m[t][i] = IloNumVarArray(env);
			m1[t][i]=new IloNumVarArray[144];
			for(int t1=t+1;t1<144;t1++)
                m1[t][i][t1]=IloNumVarArray(env);
            for(int t1=0;t1<t;t1++)
                for(int k=0;k<c[t1];k++)
                {
                    m1[t1][k][t].add(IloNumVar(env,0,1));
                    if(1+(int)(sqrt((de[t1][k][0]-st[t][i][0])*(de[t1][k][0]-st[t][i][0])+(de[t1][k][1]-st[t][i][1])*(de[t1][k][1]-st[t][i][1])*cofL*cofL)*degtoT)>t-t1)
                        model.add(m1[t1][k][t][i]==0);
                }
			for (int j = 0; j < d; j++)
				m[t][i].add(IloNumVar(env, 0, 1));
		}
		IloExprArray expr(env);
		for (int i = 0; i < c[t]; i++)
		{
			expr.add(IloExpr(env));
			for (int j = 0; j < d; j++)
			{
                if(1+(int)(sqrt((driverpos[j][0]-st[t][i][0])*(driverpos[j][0]-st[t][i][0])+(driverpos[j][1]-st[t][i][1])*(driverpos[j][1]-st[t][i][1])*cofL*cofL)*degtoT)>t)
                    model.add(m[t][i][j]==0);
				expr[i] += m[t][i][j];
				//obj -= m[i][j] * cost[j][i];
			}
			for(int t1=0;t1<t;t1++)
                for(int k=0;k<c[t1];k++)
                {
                    expr[i]+=m1[t1][k][t][i];
                }
            model.add(x[t][i] == expr[i]);
            obj += pr[t][i]*x[t][i];
		}
		expr.end();
	}
	for (int i = 0; i < d; i++)
		{
			IloExpr expr(env);
			for(int t=0;t<144;t++)
                for (int j = 0; j < c[t]; j++)
                    expr += m[t][j][i];
			model.add(expr <= 1);
			expr.end();
		}
	for(int t=0;t<144;t++)
        for(int i=0;i<c[t];i++)
        {
            IloExpr expr(env);
            for(int t1=t+1;t1<144;t1++)
                for(int k=0;k<c[t1];k++)
                    expr+=m1[t][i][t1][k];
            model.add(expr<=x[t][i]);
            expr.end();
        }
	model.add(IloMaximize(env, obj));
	obj.end();
    IloCplex cplex(model);
    //cplex.setParam((IloCplex::NumParam)CPX_PARAM_OBJDIF,0.1);
    if (!cplex.solve()) {
			env.error() << "Failed to optimize LP." << endl;
			throw(-1);
		}
    env.out() << "Solution status = " << cplex.getStatus() << endl;
    env.out() << "Solution value = " << cplex.getObjValue() << endl;
    totalincome = cplex.getObjValue();
    }
    catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}
    env.end();
    printf("%d %d %lf %lld %lld %lf",total_order,n_taken, (double)n_taken/total_order, potential_income, totalincome, (double)totalincome/potential_income);
}
