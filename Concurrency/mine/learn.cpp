#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

#define SYNCHRONIZING_MUTEX_NAME		TEXT( "__MUTEX__" )
//#define FUCKYOU

unsigned int WINAPI work(void* a) {
	unsigned int ans = *(unsigned int*)a;
	//HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, SYNCHRONIZING_MUTEX_NAME);
	//if (!hMutex) {
	//	cout << "Cannot open the mutex!\n";
	//}
	while (true) {
		//WaitForSingleObject(hMutex, INFINITE);
		ans++;
		*(unsigned int*)a = ans;
		//ReleaseMutex(hMutex);
		//cout << GetCurrentThreadId() << ": " << ans << "\n";
#ifndef FUCKYOU
		if (ans > 20000000) break;//86.505387ms
#else
		if (ans > 10000000) break;//49.876407ms
#endif
	}
	//CloseHandle(hMutex);
	return 0;
}

int main() {
	cout << "Interprocess communication demo." << endl;
	//HANDLE hMutex = CreateMutex(NULL, FALSE, SYNCHRONIZING_MUTEX_NAME);
	//if (!hMutex) {
	//	cout << "Cannot create mutex!" << endl << "Error:\t" << GetLastError() << endl;
	//	return 1;
	//}
	int n;
	LARGE_INTEGER freq;
	LARGE_INTEGER start_t, stop_t;
	QueryPerformanceFrequency(&freq);
	fprintf(stdout, "The frequency of your pc is %d.\n", freq.QuadPart);
	QueryPerformanceCounter(&start_t);
#ifndef FUCKYOU
	n = 2;
	HANDLE handle[2];
	unsigned int tag[2] = { 1, 1 };
	for (int i = 0; i < 2; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, work, (void*)&tag[i], NULL, NULL);
	WaitForMultipleObjects(2, handle, TRUE, INFINITE);
#else
	n = 4;
	HANDLE handle[4];
	unsigned int tag[4] = { 1, 1, 1, 1 };
	for (int i = 0; i < 4; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, work, (void*)&tag[i], NULL, NULL);
	WaitForMultipleObjects(4, handle, TRUE, INFINITE);
#endif
	QueryPerformanceCounter(&stop_t);
#ifndef FUCKYOU
	cout << "The two threads ";
#else
	cout << "The four threads ";
#endif
	fprintf(stdout, "executed time is %fms.\n", 1e3*(stop_t.QuadPart - start_t.QuadPart) / freq.QuadPart);
	for (int i = 0; i < n; i++)
		cout << "tag[" << i << "] = " << tag[i] << endl;
	//CloseHandle(hMutex);
	cout << "End program." << endl;
	return 0;
}
