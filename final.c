#include<stdio.h>

#define Energy 100  //�����˵��������ֵ
#define RNUM  4//�������ܸ���
#define TNUM  5//��������
#define Vhigh 3 //���ٶ�����
#define Vlow 2  //���ٶ�����
#define a1  3   //������������ʱ�������ϵ��
#define a2  2 //������������ʱ�����ص�ϵ��
#define w1  0.3  //������۾���aʱ����һ��ָ����ٶξ����Ȩ��
#define w2  0.2  //������۾���aʱ���ڶ���ָ����ٶξ����Ȩ��
#define w3  0.5  //������۾���aʱ��������ָ�����������������Ȩ��
#define maxabs 20
typedef struct robot
{
	// int num; //�����˱��
	float E1;//�����˵�ǰ����ֵ
	float s1;  //��������ɵ�ǰִ�����������ʣ��·���и��ٶ�
	float s2; //��������ɵ�ǰִ�����������ʣ��·���е��ٶ�
	float d1;  //Ϊ��������ɵ�ǰ����֮��ȥ������̨��Ҫ��·���и��ٶ�
	float d2;  //Ϊ��������ɵ�ǰ����֮��ȥ������̨��Ҫ��·���е��ٶ�
			   //float Tr; //Ϊ�����˵�������ʼ�ص���Ҫ��ʱ��
}robot;

typedef struct task
{
	char label; //�����ı�ʶ��ǩ����N��Ϊ��ͨ���񣬡�C��Ϊ�������
	int n;      //�����ı��
	float x1[2]; //��ȡ��������ڵ�x1
	float x2[2];//�����յ����ڵ�x2
	float td1;  //ִ�������˶��켣�и��ٶε�·��td1
	float td2;  //ִ�������˶��켣�е��ٶε�·��td2
	float weigh; //��������weigh
	float Tmax; //������Ҫ��ʲôʱ�����������Tmax

}task;

int find(int s[], int len, double key);
void auction(int array[][TNUM]);
double max(double s[], int len);

int main()
{
	int i, j;
	robot r1[RNUM];
	task r2[TNUM];
	float Tr[RNUM][TNUM];  //ִ�������ʱ
	float Ec[RNUM][TNUM];  //����������
	int a[RNUM][TNUM];     //���۾���
	int H1, H2, H3;   //������۾��������ָ��
	int *from;
	printf("/**************Input the informaiton  of robots********/\n");
	for (i = 0; i < RNUM; i++)
	{
		printf("\nInput the informaiton  of the NO.%d robot\n", i + 1);
		printf("Input d1,d2,s1,s2 \n");
		scanf_s("%f %f %f %f", &r1[i].d1, &r1[i].d2, &r1[i].s1, &r1[i].s2);
		printf("Input the current energy \n");
		scanf_s("%f", &r1[i].E1);
	}

	printf("/*********Input the informaiton  of tasks************/\n");
	for (j = 0; j < TNUM; j++)
	{
		printf("\n Input the informaiton  of the NO.%d tasks\n", j + 1);
		printf("Input the location where task begins: \n");
		scanf_s("%f %f", &r2[j].x1[1], &r2[j].x1[2]);
		printf("the location where task is over:\n");
		scanf_s("%f %f", &r2[j].x2[1], &r2[j].x2[2]);
		printf("Input td1,td2\n");
		scanf_s("%f %f", &r2[j].td1, &r2[j].td2);
		printf("Input the weigh of goods:\n");
		scanf_s("%f", &r2[j].weigh);
		printf("Input the max waiting time\n");
		scanf_s("%f", &r2[j].Tmax);
	}
	printf("____________the calculating results______________\n");
	/*�������������ȴ�ʱ��֮�ڣ�������ܹ����ĵص㣬�Ҵ�ȥ������̨����������ܹ���ʣ��������������������10%ʱ��
	�����˿��Բ���Ͷ��*/

	for (i = 0; i<RNUM; i++)
		for (j = 0; j < TNUM; j++)
		{
			//���㵽����̨����ʱTr���������̺ķ�����ֵEc.
			Tr[i][j] = (r1[i].s1 + r1[i].d1) / Vhigh + (r1[i].s2 + r1[i].d2) / Vlow;
			Ec[i][j] = a1*(r1[i].s1 + r1[i].s2 + r1[i].d1 + r1[i].d2 + r2[j].td1) + r2[j].td2 + a2*r2[j].weigh;
			printf("The robot %d for task %d spends time:%f s,and costs energy %f\n", i + 1, j + 1, Tr[i][j], Ec[i][j]);

			if ((Tr[i][j] > r2[j].Tmax) || (r1[i].E1 - Ec[i][j] < (Energy*0.1)))
			{
				printf("The robot %d can't bid for the task.\n", i + 1);
				a[i][j] = -1;    //������ܾ��꣬��������Ϊ-1
			}
			else
			{
				printf("The robot %d can bid for the task.\n", i + 1);
				H1 = 100 / (r1[i].s1 + r1[i].d1 + r2[j].td1);
				H2 = 100 / (r1[i].s2 + r1[i].d2 + r2[j].td2);
				H3 = r1[i].E1;
				a[i][j] = w1*H1 + w2*H2 + w3*H3; //����Ͷ�꣬�������a
			}
		}
	//������۾���a
	printf("_____________The cost matrix a is showed below_____________\n");
	for (i = 0; i < RNUM; i++)
	{
		for (j = 0; j < TNUM; j++)
		{
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
	//���������������㷨
	auction(a);

	system("pause");
	return 0;
}

//find �������ҵ�������ֵΪkey��Ԫ�صĸ���
int find(int s[], int len, double key)
{
	int i, j = 0;
	for (i = 0; i < len; i++)
	{
		if (s[i] == key)
			j++;
	}

	return j;
}

double max(double s[], int len)
{
	int i;
	double result = -1e10;
	for (i = 0; i < len; i++)
	{
		if (s[i] >= result)
			result = s[i];
	}
	return result;
}


void auction(int array[][TNUM])
{
	double p[TNUM] = { 0 };				  //�±��Ӧ���ţ�ֵΪ��ļ۸�
	double q[RNUM] = { 0 };
	double lamda, delta;
	int h,k;
	int i, j, J;
	double eplison = 1 / (double)(TNUM + 1);  //��ʼ�������߶�
	int s1[RNUM];					  //s1�±��������ˣ���ӦֵΪ���������
	int s2[TNUM];				  //s2�±�������񣬶�Ӧ�����е�Ԫ��Ϊ��������Ļ�����
	for (i = 0; i < RNUM; i++) {
		s1[i] = -1;
	}
	for (i = 0; i < TNUM; i++) {
		s2[i] = -1;
	}
	
	double vi, wi, vij;			  //viΪ�������wiΪ�θ�����vijΪ������ĵ�ǰ����
	double vj, wj, pji;           //��������ʱ���õ���
	int s10 = find(s1, RNUM, -1);    //s10Ϊ�ҵ���û�н���������˵ĸ������������㣬��Ҫ���Ͻ���Ͷ��
									 //double Pj_idx;
	if (RNUM == TNUM)
	{
		while (s10)
		{
			//bidding phase

			double bid[TNUM] = { 0 };     //��ʼ������bid���顣
			int tmps1[RNUM] = { 0 };

			for (i = 0; i < RNUM; i++)
			{
				if (s1[i] == -1)  //δ���з�������Ļ�����iѡ��
				{
					vi = -3 * TNUM*maxabs;
					wi = -5 * TNUM*maxabs;

					//ÿһ�������˶�Ӧ�����񣬼����������vi�ʹδ�����wi
					for (j = 0; (array[i][j] != -1) && (j < TNUM); j++)
					{
						vij = array[i][j] - p[j];
						if (vij > wi)
							if (vij > vi)
							{
								J = j;
								wi = vi;
								vi = vij;
							}
							else
								wi = vij;
					}
					//vi�����㣬�ҵ���������󣬽���Ͷ��۵ļ���
					if (vi >= 0)
					{
						wi = wi > 0 ? wi : 0;

						tmps1[i] = J;
						bid[i] = p[J] + (double)vi - (double)wi + eplison;

					}
					//viС���㣬δ�ҵ�������󣬽��������Ϊ-10�����䲻��
					else if (vi < 0)
						s1[i] = -10;



				}
			}

			//assginment phase
			for (j = 0; j < TNUM; j++)
			{
				int temp0 = -1; //Ԫ�ؼ�¼Ͷ����
				double temp1 = -1;//��¼Ͷ��۸�
				int Ij = -1;
				int i_old = -1;
				//Ѱ�����Ͷ����Ij
				for (h = 0; h < RNUM; h++)
				{
					if (tmps1[h] == j)
					{
						if (bid[h] > temp1)
						{
							temp0 = h;
							temp1 = bid[h];   //�����ǰͶ��۸����ͬһͶ��������ˣ���¼
						}
					}
				}
				Ij = temp0;//Ij��¼Ͷ�����ߵ��Ǹ���

						   //���ԭ������j�з��䣬Ҫȥ������
				if (s2[j] >= 0)
				{
					i_old = s2[j];
					s1[i_old] = -1;
				}

				//���¶�Ӧ��ϵ���飬�Լ��۸�����
				if (Ij >= 0)
				{
					s2[j] = Ij;
					s1[Ij] = j;
					p[j] = temp1;
				}
			}

			s10 = find(s1, TNUM, -1);


		}
	}

	if (RNUM < TNUM)
	{
		while (s10)
		{
			//bidding phase

			double bid[TNUM] = { 0 };     //��ʼ������bid���顣
			int tmps1[RNUM];              //��ʱ��¼������Ͷ�����
			for (i = 1; i < RNUM; i++)
			{
				tmps1[i] = -1;
			}

			for (i = 0; i < RNUM; i++)
			{
				if (s1[i] == -1)  //δ���з�������Ļ�����iѡ��
				{
					vi = -3 * TNUM*maxabs;
					wi = -5 * TNUM*maxabs;

					//ÿһ�������˶�Ӧ�����񣬼����������vi�ʹδ�����wi
					for (j = 0; (array[i][j] != -1) && (j < TNUM); j++)
					{
						vij = array[i][j] - p[j];
						if (vij > wi)
							if (vij > vi)
							{
								J = j;
								wi = vi;
								vi = vij;
							}
							else
								wi = vij;
					}
					//vi�����㣬�ҵ���������󣬽���Ͷ��۵ļ���
					if (vi >= 0)
					{
						wi = wi > 0 ? wi : 0;

						tmps1[i] = J;
						bid[i] = p[J] + (double)vi - (double)wi + eplison;

					}
					//viС���㣬δ�ҵ�������󣬽��������Ϊ-10�����䲻��
					else if (vi < 0)
						s1[i] = -10;



				}
			}

			//assginment phase
			for (j = 0; j < TNUM; j++)
			{
				int temp0 = -1; //Ԫ�ؼ�¼Ͷ����
				double temp1 = -1;//��¼Ͷ��۸�
				int Ij = -1;
				int i_old = -1;
				//Ѱ�����Ͷ����Ij
				for (h = 0; h < RNUM; h++)
				{
					if (tmps1[h] == j)
					{
						if (bid[h] > temp1)
						{
							temp0 = h;
							temp1 = bid[h];   //�����ǰͶ��۸����ͬһͶ��������ˣ���¼
						}
					}
				}
				Ij = temp0;//Ij��¼Ͷ�����ߵ��Ǹ���

						   //���ԭ������j�з��䣬Ҫȥ������
				if (s2[j] >= 0)
				{
					i_old = s2[j];
					s1[i_old] = -1;
				}

				//���¶�Ӧ��ϵ���飬�Լ��۸�����
				if (Ij >= 0)
				{
					s2[j] = Ij;
					s1[Ij] = j;
					p[j] = temp1;
				}
			}

			s10 = find(s1, TNUM, -1);
		}

		for (i = 0; i < RNUM; i++)
			if (s1[i] != -1 && s1[i] != -10)
			{
				k = s1[i];
				q[i] = (double)array[i][k] - p[k];  //��q��¼�����˷��䵽������������
			}

		//��������
		//void reverse_auction()

		lamda = 5 * TNUM*max(p, TNUM);

		for (k = 0; k < TNUM; k++)
		{
			if ((s2[k] != -1) && (lamda>p[k]))
				lamda = p[k];
		}
		printf("the min price is %lf\n", lamda);
		//�ҵ�lamdaΪ�Ѿ�����������е���С�۸�


		int CS_cond = 1;
		for (k = 0; (k < TNUM) && (s2[k] == -1); k++)
		{
			if (p[k] > lamda)
				CS_cond = 0;
		}
		while (CS_cond == 0)
		{


			int Ij;
			int i_old;
			for (j = 0; j < TNUM; j++)
			{
				if ((s2[j] == -1) && (p[j] > lamda))
				{
					vj = -3 * TNUM*maxabs;
					wj = -5 * TNUM*maxabs;
					for (k = 0; (k < RNUM) && (array[k][j] != -1); k++)
					{
						pji = (double)array[k][j] - q[k];
						if (pji > wj)
							if (pji > vj)
							{
								Ij = k;
								wj = vj;
								vj = pji;
							}
							else
								wi = pji;
					}
					if (lamda >= (vj - eplison))
						p[j] = lamda;
					else if (lamda < (vj - eplison))
					{
						delta = (vj - lamda) >(vj - wj + eplison) ? (vj - wj + eplison) : (vj - lamda);
						p[j] = vj - delta;
						q[Ij] = q[Ij] + delta;
					}
					if (s2[j] > 0)
					{
						i_old = s2[j];
						s1[i_old] = -1;
					}
					s1[Ij] = j;
					s2[j] = Ij;

				}
			}

			lamda = 5 * TNUM*max(p, TNUM);

			for (k = 0; k < TNUM; k++)
			{
				if ((s2[k] != -1) && (lamda>p[k]))
					lamda = p[k];
			}

			CS_cond = 1;
			for (k = 0; k < TNUM; k++)
			{
				if ((s2[k] = -1) && p[k] > lamda)
					CS_cond = 0;
			}
		}
	}

	printf("************  s1 *************\n\n");
	for (i = 0; i<RNUM; i++)
		printf("%d\t", s1[i] + 1);
	printf("\n");
	//���ɷ������Ϊ1�Ŀ��Է���
	int X[RNUM][TNUM] = { 0 };
	printf("** The Allocation Matrix ***\n\n");

	for (i = 0; i < RNUM; i++)
	{
		if (s1[i] >= 0)
			X[i][(s1[i])] = 1;
	}
	for (i = 0; i < RNUM; i++)
	{
		for (j = 0; j <TNUM; j++)
		{
			printf("%d\t", X[i][j]);
		}
		printf("\n");
	}

	printf("**** The Total Cost ****\n\n");

	//������󻯵�������tcost
	int tcost = 0;
	for (i = 0; i < TNUM; i++)
		for (j = 0; j < RNUM; j++)
			if (X[i][j] == 1)
				tcost = tcost + array[i][j];
	printf("The total cost is %d", tcost);
}