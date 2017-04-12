#include <windows.h>
#include <iostream>
#include <conio.h>

const char MUTEX_NAME[] = "MUTEX_MOREWINDOWS";

int main() {
	HANDLE hMutex = OpenMutex((LPSECURITY_ATTRIBUTES)MUTEX_ALL_ACCESS,TRUE,(LPCWSTR)MUTEX_NAME);
	if (hMutex == NULL) {
		printf("�򿪻�����ʧ��\n");
		return 0;
	}
	printf("�ȴ��С�����");

	DWORD dwResult = WaitForSingleObject(hMutex,20*1000);
	switch(dwResult) {
	case WAIT_ABANDONED:
		printf("ӵ�л������Ľ���������ֹ��\n");
		break;
	case WAIT_OBJECT_0:
		printf("�Ѿ��յ��ź�\n");
		break;
	case WAIT_TIMEOUT:
		printf("�ź�δ�ڹ涨��ʱ�����ʹ�\n");
		break;
	}
	CloseHandle(hMutex);
	return 0;
}