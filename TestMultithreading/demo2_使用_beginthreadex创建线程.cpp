//���̱߳���,_beginthreadex��ʹ��

//��ӡ�����Ľ����Ԥ�����ȫ��һ�������µ�ԭ��������û���߳�ͬ��
#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

int g_nCount;
//���̺߳���
unsigned int _stdcall ThreadFun(PVOID) {
	g_nCount++;
	cout << "�߳�ID��Ϊ��" << GetCurrentThreadId() << "���̱߳���" << g_nCount << endl;
	return 0;
}

//����������ν�������������߳�ִ�еĺ���
int main() {
	cout << "���̱߳�����" << endl;

	const int THREAD_NUM = 10;
	HANDLE handle[THREAD_NUM];

	g_nCount = 0;
	for (int i = 0; i < THREAD_NUM; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, NULL, NULL);
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
	return 0;
}