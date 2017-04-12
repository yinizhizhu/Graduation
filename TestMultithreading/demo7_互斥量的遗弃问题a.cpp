#include <iostream>
#include <conio.h>
#include <windows.h>
#include <process.h>

const char MUTEX_NAME[] = "MUTEX_MOREWINDOWS";

int main() {
	HANDLE hMutex = CreateMutex(NULL, TRUE, (LPCWSTR)MUTEX_NAME);
	printf("互斥量已经被创建，现在按任意键触发互斥量\n");
	getchar();
	//exit(0);
	ReleaseMutex(hMutex);
	printf("互斥量已经被触发\n");
	CloseHandle(hMutex);
	system("pause");
	return 0;
}