// Rand_Net.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <ctime>//ʱ�亯��  
#include <vector>
#include <algorithm>
const int MAX=23613;
int state[MAX];//�洢�ڵ��״̬(0-s,1-I,2-R)
int degree[MAX] = {0};
int Q1[MAX]={-1};  //�洢��ǰ����Ⱦ�Ľ��,������ֵΪ��Ⱦ�ڵ�ı��
//int Q2[MAX][MAX];	//�洢��Ծ�ߣ��кű�ʾ��Ⱦ�ڵ�
int Q3[MAX]={-1};	//�洢R̬�Ľڵ��
int activeside = 0;//ͳ�ƻ�Ծ�������
int count1 = 0; //Q1�еĸ�Ⱦ�ڵ���
int count2 = 0; //��Ծ�ߵ���Ŀ
int count3 = 0;	//R̬�Ľڵ���
int countqs = 0;//s̬�Ľڵ��� 
vector<int> activeedge;//�洢��Ծ�ߣ���Ӧ��S̬�ڵ�ţ�
typedef struct edge
{
	int point;
	struct edge *next;
}edge,*Edge;
typedef struct Node
{
	int point;//��ǰ����
	edge *next;
};
Node node[MAX];

void Init_Net() //��ʼ������
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
	fp = fopen("E:\\��������\\DONE\\����2\\Google+(D)\\network.pairs","r");
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
void remove_Point(int point)//ɾ��ĳһ����Ծ��
{
	 std::vector<int>::iterator pos;

	 pos = find(activeedge.begin(),activeedge.end(), point);
	 if (pos != activeedge.end()) 
	 {
		 activeedge.erase(pos);
	 }
}
void remove_All_Point(int point)//ɾ��ĳһ����Ծ��
{
	activeedge.erase(remove(activeedge.begin(),activeedge.end(),point),activeedge.end());
}


void Init_Infect(int n,double p)	//��ʼ����Ⱦ���(��ʼ��Ⱦ�����Ϊn*p)
{
	count1 = 0;	//��ʼ����Q1����Ϊ0������main�������ٴ�ѭ�����У���������Ϊ0����ÿ�γ���һ�γ�ʼ���⣬ÿ�γ�ʼ��count1���о����ֵ��������ѭ��
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
void Init_Side(int n)	//��ʼ����Ծ��
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
void Recover( int point,int n ) //�ָ��ڵ�point
{
	edge *e;
	//e = (edge *)malloc(sizeof(edge));
	for(int i=0;i<count1;i++)	//��Q1���Ƴ�point
	{
		if(Q1[i] == point)
		{
			//cout<<"�ҵ��˻ָ��ڵ�"<<endl;
			for(int k=i;k<count1-1;k++)
			{
				Q1[k] = Q1[k+1];
			}
			break;
		}
	}
	count1--;
	Q3[count3++] = point;	//������Ⱦ���ָ��Ľڵ����Q3
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
void Infect( int point,int n)	//��Ⱦ�ڵ�point
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
			remove_Point(point);//�ӻ�Ծ����ɾ������Ⱦ��S̬�ڵ�
			activeside --;
		}
		if((state[e->point] == 0))
		{
			activeedge.push_back(e->point);//����µ���point���ӵ�S̬�ڵ�
			activeside ++;
		}
		e = e->next;
	}
	//cout<<"��Ⱦ���"<<count1<<endl;
}
void Clear_Queue1()	//ÿ����֮������Q1����
{
	for(int i=0;i<count1;i++)
	{
		Q1[i] = -1;
	}
	count1 = 0;
}
/*void Clear_Queue2(int n)	//ÿ����֮������Q2����
{
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			Q2[i][j] = 0;
		}
		count2 = 0;
}*/
void Clear_Queue3()	//ÿ����֮������Q3����
{
		for(int i=0;i<count3;i++)
		{
			Q3[i] = -1;
		}
		count3 = 0;
}
void Asynchronous_Updating(double p,int n)	//�����첽����
{
	double r;	//�������������
	int point;	//���ѡȡ�Ľڵ�(�±��ڵ��)
	double p1;	//�ָ��¼������ĸ���
	while(count1 > 0)
	{
		p1 = (double)count1/(double)(count1+p*activeside);
		//cout<<"p1="<<p1<<endl;
		r = (double)rand()/RAND_MAX;//����0-1��������
		//cout<<activeside<<"+"<<activeedge.size()<<endl;
		if(r<p1)		//���лָ��ڵ�
		{
			point = (rand()*rand())%count1;	//�ָ��ڵ���±�
			Recover(Q1[point],n);
		}	
		else 		//���и�Ⱦ�ڵ�
		{
			if(activeedge.size() == 0)
				break;
			point = (rand()*rand())%activeedge.size();	//��Ⱦ�ڵ��ֵ
			Infect(activeedge[point],n);
		}
		//cout<<count1<<endl;
	}
}

void _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(0));//������������ӣ�ʹÿ�λ�ȡ��������в�ͬ
	ofstream out;
	out.open("C:\\Users\\Administrator\\Desktop\\Google+(D).txt");
	double k1 = 0.0,k2 = 0.0;
	double key = 0.0;//������ֵ
	double count_p = 0.0,count1_p = 0.0;	
	double x = 0.0,y = 0.0;	//x������ʾ�׸��ԣ�y��ʾ�ɱ���
	double p1=0.0;//��Ⱦ����
	int count4 = 0;	//R̬�ڵ�
	Init_Net();
	Read();
	cout<<"�������"<<endl;
	for(int i=0;i<MAX;i++)
	{
		k1 += degree[i];
	}
	cout<<"ƽ����="<<(double)k1/MAX<<endl;
	for(int i=0;i<MAX;i++)
	{
		k2 += degree[i]*degree[i];
	}
	cout<<"����ƽ����="<<(double)k2/MAX<<endl;
	key = (double)k1/(k2-k1);
	cout<<"HMF������ֵ="<<key<<endl;
	out<<"HMF������ֵ="<<key<<endl;
	cout<<"ED������ֵ="<<(double)k1/(k2-2.0*k1)<<endl;
	out<<"ED������ֵ="<<(double)k1/(k2-2.0*k1)<<endl;

	for(p1 = 0.0;p1<=1.0;p1 += 0.001)
	{
		count_p = 0;
		count1_p = 0;
		count4 = 0;
		for(int i=0;i<1000;i++)	//����10��ƽ��
		{
			count3 = 0;
			//cout<<"����ѭ��"<<n<<endl;
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

