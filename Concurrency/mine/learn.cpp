#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

#define SYNCHRONIZING_MUTEX_NAME		TEXT( "__MUTEX__" )
#define FUCKYOU

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
		if (ans > 20000000) break;//78
#else
		if (ans > 10000000) break;//102
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
	clock_t start = clock(), finish;
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
	finish = clock();
#ifndef FUCKYOU
	cout << "The two threads need time: ";
#else
	cout << "The four threads need time: ";
#endif
	cout << finish - start << endl;
	for (int i = 0; i < n; i++)
		cout << "tag[" << i << "] = " << tag[i] << endl;
	//CloseHandle(hMutex);
	cout << "End program." << endl;
	return 0;
}
