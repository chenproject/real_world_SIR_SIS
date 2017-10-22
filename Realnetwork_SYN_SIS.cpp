// Rand_Net.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <ctime>//ʱ�亯��  
const int MAX = 198;
int degree[MAX]={0};//�洢ÿ���ڵ�ĵ�ǰ�ȣ���������µıߣ�
int Q1[MAX]; //�洢��ǰ����Ⱦ�Ľ��,������ֵΪ��Ⱦ���ı��
int Q2[MAX];	//�洢�µı���Ⱦ�Ľ��
int state[MAX]={0};//�洢�ڵ��״̬��0-s��1-I��
int count1 = 0; //Q1�еĽڵ���
int count2 = 0; //Q2�еĽ����
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
void Init_Q()
{
	for(int i=0;i<MAX;i++)
	{
		Q1[i] = -1;
		Q2[i] = -1;
	}
	count1 = 0;
	count2 = 0;
}
void Init_State(int n)
{
	for(int i=0;i<n;i++)
		state[i] = 0;
}
void Read()
{
	edge *e;
	int point1,point2;
	FILE *fp;
	fp = fopen("E:\\��������\\DONE\\����2\\Jazz musicians(D)\\network.pairs","r");
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
int get_State(int n)
{
	int count = 0;
	for(int i=0;i<n;i++)
		if(state[i] == 1)
			count++;
	return count;
}

bool Find_Point1(int point)	//�жϵ�ǰ����Ƿ��Ѿ���ѡ��
{
	for(int i=0;i<count1;i++)
		if(Q1[i] == point)
			return false;
	return true;
}
bool Find_Point2(int point)	//�жϵ�ǰ����Ƿ��Ѿ���ѡ��
{
	for(int i=0;i<count2;i++)
		if(Q2[i] == point)
			return false;	//�Ѿ���ѡ��
	return true;
}
void Init_Infect(int n, float p)	//��ʼ����Ⱦ���(��ʼ��Ⱦ�����Ϊn*p)
{
	count1 = 0;	
	int point;
	for(int i=0;i<n*p;i++)
	{
		point = (rand()*rand())%n;
		if(Find_Point1(point))
		{
			Q1[i] = point;
			count1++;
			state[point] = 1;
		}
		else
			i--;
	}
}
bool Find_Q(int point)	//�ж��ھӽ���Ƿ���S̬
{
	for(int i=0;i<count1;i++)
	{
		if(Q1[i] == point )
			return false;	//��ǰ���ΪI̬
	}
	return true;
}
void Clear_Queue1(int n)	//�ָ���Ⱦ����
{
	for(int i=0;i<count1;i++)
	{
		state[Q1[i]] = 0;
	}
	for(int i=0;i<count1;i++)
	{
		Q1[i] = -1;
	}
	count1 = 0;
}
void Change()	//��Q2�е�Ԫ�ط���Q1��
{
	//cout<<"changecount2="<<count2<<endl;
	for(int i=0;i<count2;i++)
	{
		Q1[count1++] = Q2[i];
	}
	//cout<<get_State(MAX)<<"+"<<count2<<endl;
} 
void Clear_Queue2()	//�ָ���Ⱦ����
{
	for(int i=0;i<count2;i++)
	{
		Q2[i] = -1;
	}
	count2 = 0;
}
void Print_Queue() //��ӡ��Ⱦ����
{
	for(int i=0;i<count1;i++)
		cout<<Q1[i]<<' ';
}
void Print_Queue2() //��ӡ��Ⱦ����
{
	for(int i=0;i<count2;i++)
		cout<<Q2[i]<<' ';
}
void Infect_Point(int n, double p)		//����ͬ������,nΪ���еĽ�������������I̬�����ھӽ��
{
	edge *e;
	//e = (edge *)malloc(sizeof(edge));
	//ofstream out;
	//out.open("C:\\Users\\Administrator\\Desktop\\result.txt");
	double r;	//�������������
	//Print_Queue();
	for(int t=0;t<1000;t++)		//ѭ���ܴ���
	{
		for(int j=0;j<count1;j++)	//Q1[j]Ϊ��ǰI̬���
		{
			e = node[Q1[j]].next;
			//for(int k=0;k<degree[Q1[j]];k++)
			while(e)
			{
				if((state[e->point] == 0))
				{
					r = (double)rand()/RAND_MAX;//����0-1��������
					if(r<p)
					{
						Q2[count2++] = e->point;
						state[e->point] = 1;
					}
				}
				e = e->next;
			}
		}		
		Clear_Queue1(n);
		Change();
		Clear_Queue2();
	}
}

void _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(0));
	int count = 0;//��Ч�Ĵ���
	double k1 = 0.0,k2 = 0.0;
	double key = 0.0;//������ֵ
	ofstream out;
	out.open("C:\\Users\\Administrator\\Desktop\\Jazz musicians(D).txt");
	double count_p = 0.0,count1_p = 0.0;	
	double x = 0.0,y = 0.0;	//x������ʾ�׸��ԣ�y��ʾ�ɱ���
	double p;	//��Ⱦ����
	int count4 = 0;	//R̬�ڵ�
	int count5 = 0;
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
	key = (double)k1/k2;
	cout<<"������ֵ="<<key<<endl;
	out<<"������ֵ="<<key<<endl;
	for(p = 0.0;p<=1.0;p += 0.001)
	{
		count_p = 0;
		count1_p = 0;
		count4 = 0;
		for(int i=0;i<2000;i++)	//����10��ƽ��
		{
			Init_Q();
			Init_State(MAX);
			Init_Infect(MAX,0.01f);
			Infect_Point(MAX,p);
			count4 += count1;
			count_p += (double)count1/MAX;
			count1_p += (double)count1*count1/MAX/MAX;
			//cout<<i<<endl;
		}
		count_p /= 2000;
		count1_p /= 2000;
		count4 /= 2000;
		x = MAX*(double)(count1_p - (count_p)*(count_p)) / count_p;
		y = sqrt((double)(count1_p - (count_p)*(count_p))) / count_p;
		cout<<p<<"+"<<count4<<endl;
		cout<<y<<endl;
		cout<<"next"<<endl;
		out<<p<<" "<<x<<" "<<y<<" "<<count4<<endl;
	}
	system("pause");
}



