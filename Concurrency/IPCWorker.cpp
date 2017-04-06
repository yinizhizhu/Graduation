#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define COMMUNICATION_OBJECT_NAME TEXT("__FILE_MAPPING__")
#define SYNCHRONIZING_MUTEX_NAME TEXT( "__TEST_MUTEX__" )

typedef struct _tagCOMMUNICATIONOBJECT {
	HWND hWndClient;
	BOOL bExitLoop;
	LONG lSleepTimeout;
} COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

int main(int argc, char* argv[]) {
	HBRUSH hBrush = NULL;
	printf("%s\n", argv[0]);
	if (strcmp("blue", argv[0]) == 0)	hBrush = CreateSolidBrush(RGB(0, 0, 255));//blue
	else								hBrush = CreateSolidBrush(RGB(255, 0, 0));//red
	BOOL bContinueLoop = TRUE;
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, SYNCHRONIZING_MUTEX_NAME);
	HANDLE hMapping = OpenFileMapping(FILE_MAP_READ, FALSE, COMMUNICATION_OBJECT_NAME);
	if (hMapping) {
		while (bContinueLoop) {
			WaitForSingleObject(hMutex, INFINITE);
			PCOMMUNICATIONOBJECT pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping,
				FILE_MAP_READ, 0, 0, sizeof(COMMUNICATIONOBJECT));
			if (pCommObject) {
				LONG lWaitTimeout = pCommObject->lSleepTimeout;
				bContinueLoop = !pCommObject->bExitLoop;
				HWND hWnd = pCommObject->hWndClient;
				UnmapViewOfFile(pCommObject);
				HDC hDC = GetDC(hWnd);
				RECT rectClient = { 0 };
				if (GetClientRect(hWnd, &rectClient))
					FillRect(hDC, &rectClient, hBrush);
				ReleaseDC(hWnd, hDC);
				Sleep(lWaitTimeout);
			}
			ReleaseMutex(hMutex);
		}
	}
	CloseHandle(hMapping);
	CloseHandle(hMutex);
	DeleteObject(hBrush);
	return 0;
}