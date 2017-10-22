// Rand_Net.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <ctime>//时间函数  
const int MAX=1134890;
int state[MAX];
int degree[MAX] = {0};
int Q1[MAX]; //存储当前被感染的结点,其数组值为感染结点的标号
int Q2[MAX];	//存储新的被感染的结点
int Q3[MAX];	//存储被感染过的节点
int count1 = 0; //Q1中的节点数
int count2 = 0; //Q2中的节点数
int count3 = 0; //Q3中的节点数
typedef struct edge
{
	int point;
	struct edge *next;
}edge,*Edge;
typedef struct Node
{
	int point;//当前顶点
	edge *next;
};
Node node[MAX];

void Init_Net() //初始化网络
{
	for(int i=0;i<MAX;i++)
	{
		node[i].point = i;
		node[i].next = nullptr;
		//node[i].next = (edge *)malloc(sizeof(edge));
		//node[i].next->next = NULL;
	}
}
void Init_State()
{
	for(int i=0;i<MAX;i++)
		state[i] = 0;
}
void Init_Q()
{
	for (int i=0;i<MAX;i++)
	{
		Q1[i] = -1;
		Q2[i] = -1;
		Q3[i] = -1;
	}
	count1 = 0;
	count2 = 0;
	count3 = 0;
}
void Read()
{
	edge *e;
	int point1,point2;
	FILE *fp;
	fp = fopen("E:\\无向网络\\DONE\\网络3\\You1tube friendship(D)\\network.pairs","r");
	while(!feof(fp))
	{
		fscanf(fp,"%d  %d",&point1,&point2);
		if(feof(fp)) 
			break;
		e = (edge *)malloc(sizeof(edge));
		e->point = point2;
		e->next = node[point1].next;
		node[point1].next = e;
		degree[point1] += 1;
	}
	fclose(fp);
}
bool Find_Point1(int point)	//判断当前结点是否已经被选择
{
	for(int i=0;i<count1;i++)
		if(Q1[i] == point)
			return false;
	return true;
}
bool Find_Point2(int point)	//判断当前结点是否已经被选择
{
	for(int i=0;i<count2;i++)
		if(Q2[i] == point)
			return false;
	return true;
}
void Init_Infect(int n,double p)	//初始化感染结点(初始感染结点数为n*p)
{
	count1 = 0;	//初始化令Q1数量为0，方便main函数中再次初始化，这里若不为0，则每次除第一次初始化外，每次初始化count1都有具体的值，导致死循环
	int point;
	for(int i=0;i<n*p;i++)
	{
		point = (rand()*rand())%n;
		if(Find_Point1(point))
		{
			Q1[i] = point;
			count1++;
			Q3[count3++] = point;
			state[point] = 2;
		}
		else
			i--;
	}
}
bool Find_Q(int point)	//判断邻居结点是否是S态
{
	for(int i=0;i<count1;i++)
	{
		if(Q1[i] == point )
			return false;	//当前结点为I态
	}
	return true;
}
bool Find_Q3(int point)	//判断邻居结点已经被感染过
{
	for(int i=0;i<count3;i++)
	{
		if(Q3[i] == point )
			return false;	//当前结点已经被感染
	}
	return true;
}
void Clear_Queue1()	//恢复感染队列
{
	for(int i=0;i<count1;i++)
	{
		Q1[i] = -1;
	}
	count1 = 0;
}
void Change()	//将Q2中的元素放入Q1中
{
	for(int i=0;i<count2;i++)
	{
		Q1[count1++] = Q2[i];
	}
} 
int get_state0()
{
	int count = 0;
	for(int i=0;i<MAX;i++)
		if(state[i] == 0)
			count++;
	return count;
}
int get_state1()
{
	int count = 0;
	for(int i=0;i<MAX;i++)
		if(state[i] == 1)
			count++;
	return count;
}
int get_state2()
{
	int count = 0;
	for(int i=0;i<MAX;i++)
		if(state[i] == 2)
			count++;
	return count;
}
void Clear_Queue2()	//恢复感染队列
{
	for(int i=0;i<count2;i++)
	{
		Q2[i] = -1;
	}
	count2 = 0;
}
void Print_Queue() //打印感染队列
{
	for(int i=0;i<count1;i++)
		cout<<Q1[i]<<' ';
}
void Print_Queue2() //打印感染队列
{
	for(int i=0;i<count2;i++)
		cout<<Q2[i]<<' ';
}
void Infect_Point(int n, double p)		//进行同步更新,n为所有的结点数，方便查找I态结点的邻居结点
{
	double r;	//产生的随机概率
	edge *e;
	while(count1 > 0)
	{
		for(int j=0;j<count1;j++)	//Q1[j]为当前I态结点
		{
			e = node[Q1[j]].next;
			//for(int k=0;k<degree[Q1[j]];k++)
			while(e)
			{
				if((state[e->point] == 0))
				{
					r = (double)rand()/RAND_MAX;//生成0-1间的随机数
					if(r<p)
					{
						Q2[count2++] = e->point;
						Q3[count3++] = e->point;
						state[e->point] = 2;
					}
				}
				e = e->next;
			}
		}
		Clear_Queue1();
		Change();
		Clear_Queue2();	
	}
}

void _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(0));
	ofstream out;
	out.open("C:\\Users\\Administrator\\Desktop\\You1tube friendship(D).txt");
	double p1=0.0;//感染概率
	double k1 = 0.0,k2 = 0.0;
	double key = 0.0;//理论阈值
	int count4 = 0;	//R态节点
	double count_p = 0.0,count1_p = 0.0;	
	double x = 0.0,y = 0.0;	//x用来表示易感性，y表示可变性
	Init_Net();
	Read();
	cout<<"生成完毕"<<endl;
	for(int i=0;i<MAX;i++)
	{
		k1 += degree[i];
	}
	cout<<"平均度="<<(double)k1/MAX<<endl;
	for(int i=0;i<MAX;i++)
	{
		k2 += degree[i]*degree[i];
	}
	cout<<"二阶平均度="<<(double)k2/MAX<<endl;
	key = (double)k1/(k2-k1);
	cout<<"HMF理论阈值="<<key<<endl;
	out<<"HMF理论阈值="<<key<<endl;
	for(p1 = 0.0;p1<=1.0;p1 += 0.001)
	{
		count_p = 0;
		count1_p = 0;
		count4 = 0;
		for(int i=0;i<3000;i++)	//进行10次平均
		{
			count3 = 0;		//每次新的λ初始化网络中被感染过的节点总数
			//cout<<"继续循环"<<n<<endl;
			Init_Q();
			Init_State();
			Init_Infect(MAX,0.001);
			Infect_Point(MAX,p1);
			count4 += count3;
			count_p += (double)count3/MAX;
			count1_p += (double)count3*count3/MAX/MAX;
		}
		count_p /= 3000;
		count1_p /= 3000;
		count4 /= 3000;
		x = MAX*(double)(count1_p - (count_p)*(count_p)) / count_p;
		y = sqrt((double)(count1_p - (count_p)*(count_p))) / count_p;
		cout<<p1<<"+"<<count4<<endl;
		cout<<y<<endl;
		cout<<"next"<<endl;
		out<<p1<<" "<<x<<" "<<y<<" "<<(double)count4/MAX<<endl;
	}
	system("pause");
}

