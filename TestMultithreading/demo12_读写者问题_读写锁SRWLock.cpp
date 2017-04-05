#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;
//ֻ��Ҫʹ��һ����д���Ϳ���ʵ������Ҫ�Ĺ���
SRWLOCK g_srwlock;
const int READER_NUM = 5;//���߸���

//�����̺߳���
unsigned int _stdcall ReaderThreadFun(PVOID pM)
{
	cout << "���Ϊ" << GetCurrentThreadId() <<"�Ķ��߽���ȴ���..."<<endl;

	//���������ȡ�ļ�
	AcquireSRWLockShared(&g_srwlock);
	
	//��ȡ�ļ�
	cout <<  "���Ϊ" << GetCurrentThreadId() << "��ʼ��ȡ�ļ�"<<endl;
	Sleep(rand()%100);
	cout <<  "���Ϊ" << GetCurrentThreadId() << "������ȡ�ļ�"<<endl;

	//���߽�����ȡ�ļ�
	ReleaseSRWLockShared(&g_srwlock);
	return 0;
}

//д���̺߳���
unsigned int _stdcall WriterThreadFun(PVOID pM)
{
	cout << "    д�߳̽���ȴ���..."<<endl;
	
	//д������д�ļ�
	AcquireSRWLockExclusive(&g_srwlock);

	//д�ļ�
	cout << "    д�߳̿�ʼд�ļ�..."<<endl;
	//Sleep(rand()%100);
	Sleep(5000);
	cout << "    д�߳̽���д�ļ�..."<<endl;

	ReleaseSRWLockExclusive(&g_srwlock);
	return 0;
}


int main()
{
	//��ʼ����д��
	InitializeSRWLock(&g_srwlock);

	int i;
	HANDLE hThread[READER_NUM + 1];
	
	//���������������߳�
	for(i = 1; i<3; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	Sleep(2000);

	//����д����
	hThread[0] = (HANDLE)_beginthreadex(NULL,0,WriterThreadFun,NULL,0,NULL);
		
	//����������������߳�
	for (; i<=READER_NUM; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	WaitForMultipleObjects(READER_NUM+1,hThread,TRUE,INFINITE);
	
	//�ر����е��߳�handle
	for(i=0; i<READER_NUM+1; i++)
		CloseHandle(hThread[i]);
	
	system("pause");
	return 0;
}