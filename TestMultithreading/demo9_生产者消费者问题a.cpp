//问题描述：
//有一个生产者在生产产品，这些产品将提供给若干个消费者去消费，
//为了使生产者和消费者能并发执行，在两者之间设置一个具有多个缓冲区的缓冲池，
//生产者将它生产的产品放入一个缓冲区中，消费者可以从缓冲区中取走产品进行消
//费，显然生产者和消费者之间必须保持同步，即不允许消费者到一个空的缓冲区中
//取产品，也不允许生产者向一个已经放入产品的缓冲区中再次投放产品


//简单版本
//1生产者 1消费者 1缓冲区
#include <iostream>
#include <windows.h>
#include <process.h>

CRITICAL_SECTION g_cs;
HANDLE g_hEventFull, g_hEventEmpty;
int g_count;//全局资源
const int g_NUMBER = 10;

unsigned int _stdcall customThread(void* pPM) {
	volatile bool flag = true;
	while (flag) {
		WaitForSingleObject(g_hEventFull, INFINITE);
		EnterCriticalSection(&g_cs);
		if (g_count == g_NUMBER)
			flag = false;
		printf("消费者使用了资源%d\n", g_count);
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
		printf("生产者生产了资源%d", g_count);
		LeaveCriticalSection(&g_cs);
		SetEvent(g_hEventFull);
	}
	return 0;
}

int main() {
	InitializeCriticalSection(&g_cs);
	g_hEventFull = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hEventEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);

	//初始化全局资源
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

	return 0;
}
