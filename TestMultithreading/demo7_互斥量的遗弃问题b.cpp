#include <windows.h>
#include <iostream>
#include <conio.h>

const char MUTEX_NAME[] = "MUTEX_MOREWINDOWS";

int main() {
	HANDLE hMutex = OpenMutex((LPSECURITY_ATTRIBUTES)MUTEX_ALL_ACCESS,TRUE,(LPCWSTR)MUTEX_NAME);
	if (hMutex == NULL) {
		printf("打开互斥量失败\n");
		return 0;
	}
	printf("等待中、、、");

	DWORD dwResult = WaitForSingleObject(hMutex,20*1000);
	switch(dwResult) {
	case WAIT_ABANDONED:
		printf("拥有互斥量的进程意外终止了\n");
		break;
	case WAIT_OBJECT_0:
		printf("已经收到信号\n");
		break;
	case WAIT_TIMEOUT:
		printf("信号未在规定的时间内送达\n");
		break;
	}
	CloseHandle(hMutex);
	return 0;
}