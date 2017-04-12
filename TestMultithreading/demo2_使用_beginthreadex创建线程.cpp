//子线程报数,_beginthreadex的使用

//打印出来的结果和预想的完全不一样，导致的原因是由于没有线程同步
#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

int g_nCount;
//子线程函数
unsigned int _stdcall ThreadFun(PVOID) {
	g_nCount++;
	cout << "线程ID号为：" << GetCurrentThreadId() << "子线程报数" << g_nCount << endl;
	return 0;
}

//主函数，所谓主函数就是主线程执行的函数
int main() {
	cout << "子线程报数：" << endl;

	const int THREAD_NUM = 10;
	HANDLE handle[THREAD_NUM];

	g_nCount = 0;
	for (int i = 0; i < THREAD_NUM; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, NULL, NULL);
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
	return 0;
}