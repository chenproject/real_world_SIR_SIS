// Rand_Net.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <ctime>//ʱ�亯��  
const int MAX=1134890;
int state[MAX];
int degree[MAX] = {0};
int Q1[MAX]; //�洢��ǰ����Ⱦ�Ľ��,������ֵΪ��Ⱦ���ı��
int Q2[MAX];	//�洢�µı���Ⱦ�Ľ��
int Q3[MAX];	//�洢����Ⱦ���Ľڵ�
int count1 = 0; //Q1�еĽڵ���
int count2 = 0; //Q2�еĽڵ���
int count3 = 0; //Q3�еĽڵ���
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
	fp = fopen("E:\\��������\\DONE\\����3\\You1tube friendship(D)\\network.pairs","r");
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
			return false;
	return true;
}
void Init_Infect(int n,double p)	//��ʼ����Ⱦ���(��ʼ��Ⱦ�����Ϊn*p)
{
	count1 = 0;	//��ʼ����Q1����Ϊ0������main�������ٴγ�ʼ������������Ϊ0����ÿ�γ���һ�γ�ʼ���⣬ÿ�γ�ʼ��count1���о����ֵ��������ѭ��
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
bool Find_Q(int point)	//�ж��ھӽ���Ƿ���S̬
{
	for(int i=0;i<count1;i++)
	{
		if(Q1[i] == point )
			return false;	//��ǰ���ΪI̬
	}
	return true;
}
bool Find_Q3(int point)	//�ж��ھӽ���Ѿ�����Ⱦ��
{
	for(int i=0;i<count3;i++)
	{
		if(Q3[i] == point )
			return false;	//��ǰ����Ѿ�����Ⱦ
	}
	return true;
}
void Clear_Queue1()	//�ָ���Ⱦ����
{
	for(int i=0;i<count1;i++)
	{
		Q1[i] = -1;
	}
	count1 = 0;
}
void Change()	//��Q2�е�Ԫ�ط���Q1��
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
	double r;	//�������������
	edge *e;
	while(count1 > 0)
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
	double p1=0.0;//��Ⱦ����
	double k1 = 0.0,k2 = 0.0;
	double key = 0.0;//������ֵ
	int count4 = 0;	//R̬�ڵ�
	double count_p = 0.0,count1_p = 0.0;	
	double x = 0.0,y = 0.0;	//x������ʾ�׸��ԣ�y��ʾ�ɱ���
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
	for(p1 = 0.0;p1<=1.0;p1 += 0.001)
	{
		count_p = 0;
		count1_p = 0;
		count4 = 0;
		for(int i=0;i<3000;i++)	//����10��ƽ��
		{
			count3 = 0;		//ÿ���µĦ˳�ʼ�������б���Ⱦ���Ľڵ�����
			//cout<<"����ѭ��"<<n<<endl;
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

