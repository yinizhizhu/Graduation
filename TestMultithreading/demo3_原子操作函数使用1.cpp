#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

volatile long g_nLoginCount;//登陆次数
const int THREAD_NUM = 10;//启动线程数

unsigned int _stdcall ThreadFun(void* pPM) {
	Sleep(100);
	g_nLoginCount++;
	//cout << g_nLoginCount << endl;
	Sleep(50);
	return 0;
}

int main() {
	g_nLoginCount = 0;

	HANDLE handle[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, NULL, NULL);

	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
	printf("有%d个用户登陆后记录结果是%d\n", THREAD_NUM, g_nLoginCount);
	system("pause");
	return 0;
}
