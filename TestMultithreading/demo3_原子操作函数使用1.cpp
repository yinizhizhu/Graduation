#include <iostream>
#include <windows.h>
#include <process.h>

volatile long g_nLoginCount;//��½����
const int THREAD_NUM = 10;//�����߳���

unsigned int _stdcall ThreadFun(void* pPM) {
	Sleep(100);
	g_nLoginCount++;
	Sleep(50);
	return 0;
}

int main() {
	g_nLoginCount = 0;
	
	HANDLE handle[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL,0,ThreadFun,NULL,0,NULL);

	WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
	printf("��%d���û���½���¼�����%d\n",THREAD_NUM,g_nLoginCount);
	system("pause");
	return 0;
}
