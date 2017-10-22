// Rand_Net.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <ctime>//时间函数  
#include <vector>
#include <algorithm>
const int MAX=23613;
int state[MAX];//存储节点的状态(0-s,1-I,2-R)
int degree[MAX] = {0};
int Q1[MAX]={-1};  //存储当前被感染的结点,其数组值为感染节点的标号
//int Q2[MAX][MAX];	//存储活跃边，行号表示感染节点
int Q3[MAX]={-1};	//存储R态的节点号
int activeside = 0;//统计活跃变的条数
int count1 = 0; //Q1中的感染节点数
int count2 = 0; //活跃边的数目
int count3 = 0;	//R态的节点数
int countqs = 0;//s态的节点数 
vector<int> activeedge;//存储活跃边（对应的S态节点号）
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
void Init_State(int n)
{
	for(int i=0;i<n;i++)
		state[i] = 0; 
}
bool find_pointA(int point1 ,int point2)
{
	edge *e;
	//e = (edge *)malloc(sizeof(edge));
	e = node[point1].next;
	while(e)
	{
		if(e->point == point2)
			return false;
		e = e->next;
	}
	return true;
}
void Read()
{
	edge *e;
	int point1,point2;
	FILE *fp;
	fp = fopen("E:\\无向网络\\DONE\\网络2\\Google+(D)\\network.pairs","r");
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
void remove_Point(int point)//删除某一条活跃边
{
	 std::vector<int>::iterator pos;

	 pos = find(activeedge.begin(),activeedge.end(), point);
	 if (pos != activeedge.end()) 
	 {
		 activeedge.erase(pos);
	 }
}
void remove_All_Point(int point)//删除某一条活跃边
{
	activeedge.erase(remove(activeedge.begin(),activeedge.end(),point),activeedge.end());
}


void Init_Infect(int n,double p)	//初始化感染结点(初始感染结点数为n*p)
{
	count1 = 0;	//初始化令Q1数量为0，方便main函数中再次循环运行，这里若不为0，则每次除第一次初始化外，每次初始化count1都有具体的值，导致死循环
	int point;
	for(int i=0;i<5;i++)
	{
		point = (rand()*rand())%n;
		if(state[point] == 0)
		{
			Q1[count1++] = point;
			state[point] = 1;
		}
		else
			i--;
	}
	//cout<<"count3="<<count3<<endl;
}
void Init_Side(int n)	//初始化活跃边
{
	edge *e;
	//e = (edge *)malloc(sizeof(edge));
	activeside = 0;
	for(int i=0;i<count1;i++)
	{
		e = node[Q1[i]].next;
		while (e)
		{
			if((state[e->point] == 0))
			{
				activeedge.push_back(e->point);
				activeside ++;
			}
			e = e->next;
		}
	}
		//cout<<"Init_Side"<<endl;
}
void Recover( int point,int n ) //恢复节点point
{
	edge *e;
	//e = (edge *)malloc(sizeof(edge));
	for(int i=0;i<count1;i++)	//从Q1中移除point
	{
		if(Q1[i] == point)
		{
			//cout<<"找到了恢复节点"<<endl;
			for(int k=i;k<count1-1;k++)
			{
				Q1[k] = Q1[k+1];
			}
			break;
		}
	}
	count1--;
	Q3[count3++] = point;	//将被感染过恢复的节点存入Q3
	state[point] = 2;
	e = node[point].next;
	while (e)
	{
		if ((state[e->point] == 0))
		{
			remove_Point(e->point);
			activeside --;
		}
		e = e->next;
	}
}
void Infect( int point,int n)	//感染节点point
{
	edge *e;
	//e = (edge *)malloc(sizeof(edge));
	Q1[count1++] = point;
	state[point] = 1;
	e = node[point].next;
	while (e)
	{
		if((state[e->point] == 1))
		{
			remove_Point(point);//从活跃边中删除被感染的S态节点
			activeside --;
		}
		if((state[e->point] == 0))
		{
			activeedge.push_back(e->point);//添加新的与point连接的S态节点
			activeside ++;
		}
		e = e->next;
	}
	//cout<<"感染完毕"<<count1<<endl;
}
void Clear_Queue1()	//每个λ之后重置Q1队列
{
	for(int i=0;i<count1;i++)
	{
		Q1[i] = -1;
	}
	count1 = 0;
}
/*void Clear_Queue2(int n)	//每个λ之后重置Q2队列
{
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			Q2[i][j] = 0;
		}
		count2 = 0;
}*/
void Clear_Queue3()	//每个λ之后重置Q3队列
{
		for(int i=0;i<count3;i++)
		{
			Q3[i] = -1;
		}
		count3 = 0;
}
void Asynchronous_Updating(double p,int n)	//进行异步更新
{
	double r;	//产生的随机概率
	int point;	//随机选取的节点(下标或节点号)
	double p1;	//恢复事件发生的概率
	while(count1 > 0)
	{
		p1 = (double)count1/(double)(count1+p*activeside);
		//cout<<"p1="<<p1<<endl;
		r = (double)rand()/RAND_MAX;//生成0-1间的随机数
		//cout<<activeside<<"+"<<activeedge.size()<<endl;
		if(r<p1)		//进行恢复节点
		{
			point = (rand()*rand())%count1;	//恢复节点的下标
			Recover(Q1[point],n);
		}	
		else 		//进行感染节点
		{
			if(activeedge.size() == 0)
				break;
			point = (rand()*rand())%activeedge.size();	//感染节点的值
			Infect(activeedge[point],n);
		}
		//cout<<count1<<endl;
	}
}

void _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(0));//设置随机数种子，使每次获取的随机序列不同
	ofstream out;
	out.open("C:\\Users\\Administrator\\Desktop\\Google+(D).txt");
	double k1 = 0.0,k2 = 0.0;
	double key = 0.0;//理论阈值
	double count_p = 0.0,count1_p = 0.0;	
	double x = 0.0,y = 0.0;	//x用来表示易感性，y表示可变性
	double p1=0.0;//感染概率
	int count4 = 0;	//R态节点
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
	cout<<"ED理论阈值="<<(double)k1/(k2-2.0*k1)<<endl;
	out<<"ED理论阈值="<<(double)k1/(k2-2.0*k1)<<endl;

	for(p1 = 0.0;p1<=1.0;p1 += 0.001)
	{
		count_p = 0;
		count1_p = 0;
		count4 = 0;
		for(int i=0;i<1000;i++)	//进行10次平均
		{
			count3 = 0;
			//cout<<"继续循环"<<n<<endl;
			Init_State(MAX);
			Init_Infect(MAX,0.001);
			Init_Side(MAX);
			Asynchronous_Updating(p1,MAX);
			count4 += count3;
			count_p += (double)count3/MAX;
			count1_p += (double)count3*count3/MAX/MAX;
			//out<<p1<<" "<<(float)count3/n<<endl;
			//out<<(float)count3/n<<endl;
			Clear_Queue1();
			//Clear_Queue2(n);
			Clear_Queue3();
			activeedge.clear();
			//cout<<i<<endl;
		}
		count_p /= 1000;
		count1_p /= 1000;
		count4 /= 1000;
		x = MAX*(double)(count1_p - (count_p)*(count_p)) / count_p;
		y = sqrt((double)(count1_p - (count_p)*(count_p))) / count_p;
		cout<<p1<<"+"<<count4<<endl;
		cout<<y<<endl;
		cout<<"next"<<endl;
		out<<p1<<" "<<x<<" "<<y<<" "<<(double)count4/MAX<<endl;
	}
	system("pause");
}

