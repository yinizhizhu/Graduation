//����������
//	���߳�����10�����̣߳�������ʾ���߳���ŵı�����ַ��Ϊ���������ݸ����̡߳����߳�
//���ղ���->sleep(50)->ȫ�ֱ���++ ->sleep(0) ->���������ȫ�ֱ���
//Ҫ��
//	���߳�������߳���Ų����ظ�
//	ȫ�ֱ���������������

//�����������������
//���Է����ں������ֵĽ���Ƚϻ���
#include <iostream>
#include <windows.h>
#include <process.h>

long g_nNum; //ȫ����Դ
unsigned int _stdcall Fun(void* pPM); //�̺߳���
const int THREAD_NUM = 10; //���߳���

int main() {
	g_nNum = 0;
	HANDLE handle[THREAD_NUM];
	
	int i = 0;
	while(i < THREAD_NUM) {
		handle[i] = (HANDLE)_beginthreadex(NULL,0,Fun,&i,0,NULL);
		i++;
	}
	WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
	system("pause");
	return 0;
}

unsigned int _stdcall Fun(void* pPM) {
	//���ڴ����߳���Ҫһ���Ŀ������������̲߳����ܵ�һʱ��ִ�е���
	int nThreadNum = *(int*)pPM;
	Sleep(50);
	g_nNum++;
	Sleep(0);
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n",nThreadNum,g_nNum);
	return 0;
}