#include <iostream>
#include <conio.h>
#include <windows.h>
#include <process.h>

const char MUTEX_NAME[] = "MUTEX_MOREWINDOWS";

int main() {
	HANDLE hMutex = CreateMutex(NULL, TRUE, (LPCWSTR)MUTEX_NAME);
	printf("�������Ѿ������������ڰ����������������\n");
	_getch();
	//exit(0);
	ReleaseMutex(hMutex);
	printf("�������Ѿ�������\n");
	CloseHandle(hMutex);
	system("pause");
	return 0;
}