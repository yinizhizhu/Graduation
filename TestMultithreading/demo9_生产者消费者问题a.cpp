//����������
//��һ����������������Ʒ����Щ��Ʒ���ṩ�����ɸ�������ȥ���ѣ�
//Ϊ��ʹ�����ߺ��������ܲ���ִ�У�������֮������һ�����ж���������Ļ���أ�
//�����߽��������Ĳ�Ʒ����һ���������У������߿��Դӻ�������ȡ�߲�Ʒ������
//�ѣ���Ȼ�����ߺ�������֮����뱣��ͬ�����������������ߵ�һ���յĻ�������
//ȡ��Ʒ��Ҳ��������������һ���Ѿ������Ʒ�Ļ��������ٴ�Ͷ�Ų�Ʒ


//�򵥰汾
//1������ 1������ 1������
#include <iostream>
#include <windows.h>
#include <process.h>

CRITICAL_SECTION g_cs;
HANDLE g_hEventFull, g_hEventEmpty;
int g_count;//ȫ����Դ
const int g_NUMBER = 10;

unsigned int _stdcall customThread(void* pPM) {
	volatile bool flag = true;
	while (flag) {
		WaitForSingleObject(g_hEventFull, INFINITE);
		EnterCriticalSection(&g_cs);
		if (g_count == g_NUMBER)
			flag = false;
		printf("������ʹ������Դ%d\n", g_count);
		LeaveCriticalSection(&g_cs);
		SetEvent(g_hEventEmpty);
		Sleep(10);
	}
	return 0;
}

unsigned int _stdcall produceThread(void* pPM) {

	for (int i = 0; i < g_NUMBER; i++) {
		WaitForSingleObject(g_hEventEmpty, INFINITE);
		EnterCriticalSection(&g_cs);
		g_count++;
		printf("��������������Դ%d", g_count);
		LeaveCriticalSection(&g_cs);
		SetEvent(g_hEventFull);
	}
	return 0;
}

int main() {
	InitializeCriticalSection(&g_cs);
	g_hEventFull = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hEventEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);

	//��ʼ��ȫ����Դ
	g_count = 0;

	HANDLE hthread[2];
	hthread[0] = (HANDLE)_beginthreadex(NULL, 0, customThread, NULL, 0, NULL);
	hthread[1] = (HANDLE)_beginthreadex(NULL, 0, produceThread, NULL, 0, NULL);

	WaitForMultipleObjects(2, hthread, TRUE, INFINITE);

	CloseHandle(hthread[0]);
	CloseHandle(hthread[1]);

	CloseHandle(g_hEventFull);
	CloseHandle(g_hEventEmpty);
	DeleteCriticalSection(&g_cs);

	system("pause");
}
