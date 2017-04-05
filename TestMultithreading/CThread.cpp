//
////实验室项目
//#include <iostream>
//#include <Windows.h>
//
//using namespace std;
//
//HANDLE hSemaphore;
//HANDLE hMutex;
//HANDLE hThreadImageShow;
//HANDLE hThreadXfer;
//
//static int a = 0;
//
//
//
//
//DWORD WINAPI IMGShow(LPVOID lpParamter)
//{
//	WaitForSingleObject(hSemaphore, INFINITE);
//	WaitForSingleObject(hMutex, INFINITE);
//	cout << a<<endl;
//	ReleaseMutex(hMutex);
//	return 0;
//}
//
//DWORD WINAPI USBXfer(LPVOID lpParamter)
//{
//	a++;
//	WaitForSingleObject(hMutex, INFINITE);
//
//	a++;
//	ReleaseMutex(hMutex);
//	ReleaseSemaphore(hSemaphore, 1, NULL);
//	return 0;
//}
//
//void InitThread()
//{
//	hSemaphore = CreateSemaphore(NULL,0,90,NULL);
//	hMutex = CreateMutex(NULL, FALSE, NULL);
//
//	hThreadImageShow = CreateThread(NULL,0,IMGShow,NULL,0,NULL);
//	hThreadXfer = CreateThread(NULL,0,USBXfer,NULL,0,NULL);
//
//}
//int main()
//{
//	InitThread();
//	system("pause");
//}