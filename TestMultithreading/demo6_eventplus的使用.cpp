#include <iostream>
#include <windows.h>
#include <process.h>

HANDLE g_hThreadEvent;
//快线程
unsigned int _stdcall FastThreadFun(void* pPM) {
	Sleep(10);//用这个来保证各线程调用等待函数的次序有一定的随机性
	printf("%s 启动\n", (PSTR)pPM);
	WaitForSingleObject(g_hThreadEvent, INFINITE);
	printf("%s 等到事件被触发 顺利结束\n", (PSTR)pPM);
	return 0;
}

//慢线程
unsigned int _stdcall SlowThreadFun(void* pPM) {
	Sleep(100);
	printf("%s 启动\n", (PSTR)pPM);
	WaitForSingleObject(g_hThreadEvent, INFINITE);
	printf("%s 等到事件被触发 顺利结束\n", (PSTR)pPM);
	return 0;
}

int main() {
	printf(" 使用PlusEvent()函数\n");

	BOOL bManualReset = TRUE;
	//创建事件 第二个参数手动置位TRUE，自动置位FALSE
	g_hThreadEvent = CreateEvent(NULL, bManualReset, FALSE, NULL);
	if (bManualReset == TRUE)
		printf("当前使用手动置位事件\n");
	else
		printf("当前使用自动置位事件\n");

	char szFastThreadName[5][30] = { "快线程1000", "快线程1001", "快线程1002", "快线程1003", "快线程1004" };
	char szSlowThreadName[2][30] = { "慢线程196", "慢线程197" };

	int i = 0;
	for (i = 0; i < 5; i++)
		_beginthreadex(NULL, 0, FastThreadFun, szFastThreadName[i], 0, NULL);
	for (i = 0; i < 2; i++)
		_beginthreadex(NULL, 0, SlowThreadFun, szSlowThreadName[i], 0, NULL);

	Sleep(50);
	printf("现在主线程触发一个事件脉冲 - PlusEvent()\n");
	PulseEvent(g_hThreadEvent);

	Sleep(3000);
	printf("时间到，主线程运行结束\n");
	CloseHandle(g_hThreadEvent);
	system("pause");
	return 0;
}

//1.对于手动置位事件，所有正处于等待状态下线程都变成可调度状态。

//2.对于自动置位事件，所有正处于等待状态下线程只有一个变成可调度状态。