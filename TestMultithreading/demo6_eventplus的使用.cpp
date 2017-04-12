#include <iostream>
#include <windows.h>
#include <process.h>

HANDLE g_hThreadEvent;
//���߳�
unsigned int WINAPI FastThreadFun(void* pPM) {
	Sleep(10);//���������֤���̵߳��õȴ������Ĵ�����һ���������
	printf("%s ����\n", (PSTR)pPM);
	WaitForSingleObject(g_hThreadEvent, INFINITE);
	printf("%s �ȵ��¼������� ˳������\n", (PSTR)pPM);
	return 0;
}

//���߳�
unsigned int WINAPI SlowThreadFun(void* pPM) {
	Sleep(100);
	printf("%s ����\n", (PSTR)pPM);
	WaitForSingleObject(g_hThreadEvent, INFINITE);
	printf("%s �ȵ��¼������� ˳������\n", (PSTR)pPM);
	return 0;
}

int main() {
	printf(" ʹ��PlusEvent()����\n");

	BOOL bManualReset = TRUE;
	//�����¼� �ڶ��������ֶ���λTRUE���Զ���λFALSE
	g_hThreadEvent = CreateEvent(NULL, bManualReset, FALSE, NULL);
	if (bManualReset == TRUE)
		printf("��ǰʹ���ֶ���λ�¼�\n");
	else
		printf("��ǰʹ���Զ���λ�¼�\n");

	char szFastThreadName[5][30] = { "���߳�1000", "���߳�1001", "���߳�1002", "���߳�1003", "���߳�1004" };
	char szSlowThreadName[2][30] = { "���߳�196", "���߳�197" };

	int i = 0;
	for (i = 0; i < 5; i++)
		_beginthreadex(NULL, 0, FastThreadFun, szFastThreadName[i], 0, NULL);
	for (i = 0; i < 2; i++)
		_beginthreadex(NULL, 0, SlowThreadFun, szSlowThreadName[i], 0, NULL);
	Sleep(50);
	printf("�������̴߳���һ���¼����� - PlusEvent()\n");
	PulseEvent(g_hThreadEvent);
	Sleep(3000);
	printf("ʱ�䵽�����߳����н���\n");
	CloseHandle(g_hThreadEvent);
	system("pause");
	return 0;
}
//1.�����ֶ���λ�¼������������ڵȴ�״̬���̶߳���ɿɵ���״̬��
//2.�����Զ���λ�¼������������ڵȴ�״̬���߳�ֻ��һ����ɿɵ���״̬��