#include <iostream>
#include <stdio.h>
#include <string.h>
#define cofL 0.793
#define degtoT 450
#include <math.h>
#include <stdlib.h>
using namespace std;
char idex[1000][100];
double idpos[1000][2];
double driverpos[50000][2];
int onservice[50000],n,arrdriver[15000][2000];
int dur[32000][15000];
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
    int d=0, n_taken=0, total_order=0;
    long long int totalincome=0, potential_income=0, n_data=0;
    FILE *id=fopen("index.txt","r");
    FILE *dr=fopen("new_20181023_idle_driver_seoul7.txt", "r");
    FILE *order=fopen("calls_seoul_h3res7_20181023_min.txt","r");
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
            //i=i==n?i:rand()%n;
            if(i==n)
                continue;
            for(int j=0;j<b;j++)
            {
                if(rand()%1)continue;
                position(driverpos[d],i);
                d++;
            }
        }
    }
    fclose(id);
    fclose(dr);
    char start[100], dest[100];
    int duration, price;
    for(int t=0;t<1440;t++)
    {
        int c=0, ct, pr[15000], len[15000], dcnt[50000];
        double st[15000][2], de[15000][2];
        while(duration||(fscanf(order,"%s %s %d %d %d",&start,&dest,&ct,&duration,&price)!=-1&&ct==t))
        {
            int i;
            for(i=0;i<n;i++)
                if(strcmp(idex[i],start)==0)
                    break;
            //i=i==n?i:rand()%n;
            if(i==n||rand()%1)
            {
                duration=0;
                continue;
            }
            position(st[c],i);
            for(i=0;i<n;i++)
                if(strcmp(idex[i],dest)==0)
                    break;
            if(i==n)
            {
                duration=0;
                continue;
            }
            position(de[c],i);
            len[c]=duration;
            pr[c]=price;
            total_order++;
            potential_income+=price;
            c++;
            duration=0;
        }
        int d1=0;
        for(int i=0;i<d;i++)
            for(int j=0;j<c;j++)
                dur[d][c]=0;
        for(int i=0;i<d;i++)
            if(onservice[i]<=t){dcnt[d1]=i;d1++;}
        FILE *durout=fopen("dur.txt","w");
        fprintf(durout,"%d %d\n",d1,c);
        for(int i=0;i<d;i++)
        {
            int tmp[250]={};
            if(onservice[i]>t)
                continue;
            for(int j=0;j<c;j++)
            {
                dur[i][j]=1+(int)(sqrt((driverpos[i][0]-st[j][0])*(driverpos[i][0]-st[j][0])+(driverpos[i][1]-st[j][1])*(driverpos[i][1]-st[j][1])*cofL*cofL)*degtoT);
                tmp[dur[i][j]]++;
            }
            int s=0, p;
            for(p=1;s<5;p++)
                s+=tmp[p];
            for(int j=0;j<c;j++)
            {
                dur[i][j]=1+(int)(sqrt((driverpos[i][0]-st[j][0])*(driverpos[i][0]-st[j][0])+(driverpos[i][1]-st[j][1])*(driverpos[i][1]-st[j][1])*cofL*cofL)*degtoT);
                if(dur[i][j]<6)
                {
                    n_data++;
                    fprintf(durout,"%d\n",dur[i][j]);
                }
                else
                    fprintf(durout,"%d\n",249);
                if(dur[i][j]<=0)
                    printf("2");
            }
        }
        fclose(durout);
        system("./durprocess/bin/Release/durprocess");
        system("./maxflowopt/bin/Release/maxflowopt");
        //system("./flowopt/bin/Debug/flowopt");
        //system("./gotodemandalgorithm/bin/Debug/gotodemandalgorithm");
        //system("./fastestdriver/bin/Debug/fastestdriver");
        //system("./randomalgorithm/bin/Debug/randomalgorithm");
        FILE *movedata=fopen("move.txt","r");
        int mdata[50000], mint[10000], cnt[15000]={};
        for(int i=0;i<c;i++)
            mint[i]=100;
        while(fscanf(movedata,"%d %d",&a,&b)!=-1)
            mdata[dcnt[a]]=b+1;
        for(int i=0;i<d;i++)
        {
            if(onservice[i]>t)
            continue;
            if(mdata[i]>0&&dur[i][mdata[i]-1]==1)
            {
                driverpos[i][0]=st[mdata[i]-1][0];
                driverpos[i][1]=st[mdata[i]-1][1];
                arrdriver[mdata[i]-1][cnt[mdata[i]-1]]=i;
                cnt[mdata[i]-1]++;
                continue;
            }
            double tmp=sqrt((driverpos[i][0]-st[mdata[i]-1][0])*(driverpos[i][0]-st[mdata[i]-1][0])+(driverpos[i][1]-st[mdata[i]-1][1])*(driverpos[i][1]-st[mdata[i]-1][1])*cofL*cofL)*degtoT;
            driverpos[i][0]-=(driverpos[i][0]-st[mdata[i]-1][0])/tmp;
            driverpos[i][1]-=(driverpos[i][1]-st[mdata[i]-1][1])/tmp;
        }
        for(int i=0;i<c;i++)
        {
            if(cnt[i]==0)
                continue;
            a=rand()%cnt[i];
            driverpos[arrdriver[i][a]][0]=de[i][0];
            driverpos[arrdriver[i][a]][1]=de[i][1];
            onservice[arrdriver[i][a]]=t+len[i];
            n_taken++;
            totalincome+=pr[i];
        }
        fclose(movedata);
    }
    printf("1023%d %d %lf %lld %lld %lf %lld",total_order,n_taken, (double)n_taken/total_order, potential_income, totalincome, (double)totalincome/potential_income, n_data);
}
