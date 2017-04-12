//ʱ�䣺2016-03-23
//��ַ��http://blog.csdn.net/morewindows/article/details/7596034
//���ݣ���д�����⣬����д��д��ʱ���ܶ���д��֮�󣬶��߶����Զ�û������
//		ʵ�ֵĹ��̣�һ����������ߣ����������������߶���Ȼ����д���̣߳������ڿ��������������ߵ��߳�
//		�����������¼�������һ���޶����¼���һ��д���¼�


#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

const int READER_NUM = 5;

HANDLE g_hEventWriter,g_hEventNoReader;
volatile int g_nReaderCount;
CRITICAL_SECTION g_cs_reader_counter;

//�����߳�
unsigned int _stdcall ReaderThreadFun(PVOID pM) {
	cout << "���Ϊ" << GetCurrentThreadId() <<"�Ķ��߽���ȴ���..."<<endl;
	//�ȴ�д�����д�ļ�
	WaitForSingleObject(g_hEventWriter,INFINITE);

	//���Ӷ��߸���
	EnterCriticalSection(&g_cs_reader_counter);
	g_nReaderCount++;
	if (g_nReaderCount == 1) {
		ResetEvent(g_hEventNoReader);
	}
	LeaveCriticalSection(&g_cs_reader_counter);

	//��ȡ�ļ�
	cout <<  "���Ϊ" << GetCurrentThreadId() << "��ʼ��ȡ�ļ�"<<endl;
	Sleep(rand()%100);

	//�����Ķ������߸������ٿ�λ����
	cout <<  "���Ϊ" << GetCurrentThreadId() << "������ȡ�ļ�"<<endl;
	EnterCriticalSection(&g_cs_reader_counter);
	g_nReaderCount--;
	if (g_nReaderCount == 0) {
		SetEvent(g_hEventNoReader);
	}
	LeaveCriticalSection(&g_cs_reader_counter);
	return 0;
}

//д���߳�
unsigned int _stdcall WriterThreadFun(PVOID pM) {
	cout << "    д�߳̽���ȴ���..."<<endl;
	//�ȴ����ļ��Ķ���Ϊ��
	WaitForSingleObject(g_hEventNoReader,INFINITE);
	//���д������д�ļ�
	ResetEvent(g_hEventWriter);

	//д�ļ�
	cout << "    д�߳̿�ʼд�ļ�..."<<endl;
	//Sleep(rand()%100);
	Sleep(5000);
	cout << "    д�߳̽���д�ļ�..."<<endl;

	//���д�߽���д�ļ�
	SetEvent(g_hEventWriter);
	return 0;
}


int main() {
	//��ʼ���ؼ���
	InitializeCriticalSection(&g_cs_reader_counter);

	//��ʼ���¼�
	g_hEventWriter = CreateEvent(NULL,TRUE,TRUE,NULL);
	g_hEventNoReader = CreateEvent(NULL,FALSE,TRUE,NULL);
	g_nReaderCount = 0;

	int i;
	HANDLE hThread[READER_NUM + 1];

	//���������������߳�
	for(i = 1; i<3; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	//����д����
	hThread[0] = (HANDLE)_beginthreadex(NULL,0,WriterThreadFun,NULL,0,NULL);
	
	Sleep(500);
	//����������������߳�
	for (; i<=READER_NUM; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	WaitForMultipleObjects(READER_NUM+1,hThread,TRUE,INFINITE);

	//�ر����е��߳�handle
	for(i=0; i<READER_NUM+1; i++)
		CloseHandle(hThread[i]);

	//�ر��¼�handle�͹ؼ���
	CloseHandle(g_hEventNoReader);
	CloseHandle(g_hEventWriter);
	DeleteCriticalSection(&g_cs_reader_counter);
	system("pause");
	return 0;
}