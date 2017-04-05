//////程序描述：
//////	主线程启动10个子线程，并将表示子线程序号的变量地址作为参数，传递给子线程。子线程
//////接收参数->sleep(50)->全局变量++ ->sleep(0) ->输出参数和全局变量
//////要求：
//////	子线程输出的线程序号不能重复
//////	全局变量的输出必须递增
////
////#include <iostream>
////#include <windows.h>
////#include <process.h>
////
////long g_nNum; //全局资源
////unsigned int _stdcall Fun(void* pPM); //线程函数
////const int THREAD_NUM = 10; //子线程数
////
//////信号量和关键段
////HANDLE g_hThreadSemaphore;
////CRITICAL_SECTION g_csThreadCode;
////
////int main()
////{
////	printf("经典线程同步 信号量semaphore\n");
////
////	//初始化事件和关键段 自动置位 初试无触发的匿名事件
////	g_hThreadSemaphore = CreateSemaphore(NULL,0,1,NULL);
////	InitializeCriticalSection(&g_csThreadCode);
////
////	g_nNum = 0;
////	HANDLE handle[THREAD_NUM];
////	int i = 0;
////
////	while(i < THREAD_NUM)
////	{
////		handle[i] = (HANDLE)_beginthreadex(NULL,0,Fun,&i,0,NULL);
////		WaitForSingleObject(g_hThreadSemaphore,INFINITE);//等待事件被触发
////		i++;
////	}
////	WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
////
////	//销毁事件和关键段
////	CloseHandle(g_hThreadSemaphore);
////	DeleteCriticalSection(&g_csThreadCode);
////
////	system("pause");
////	return 0;
////}
////
////unsigned int _stdcall Fun(void* pPM)
////{
////	//由于创建线程需要一定的开销，所以新线程并不能第一时间执行到这
////	int nThreadNum = *(int*)pPM;
////	ReleaseSemaphore(g_hThreadSemaphore,1,NULL);//触发事件
////
////	Sleep(50);
////
////	EnterCriticalSection(&g_csThreadCode);
////	g_nNum++;
////	Sleep(0);
////	printf("线程编号为%d  全局资源值为%d\n",nThreadNum,g_nNum);
////	LeaveCriticalSection(&g_csThreadCode);
////	return 0;
////}
//
//
////程序描述：
////	主线程启动10个子线程，并将表示子线程序号的变量地址作为参数，传递给子线程。子线程
////接收参数->sleep(50)->全局变量++ ->sleep(0) ->输出参数和全局变量
////要求：
////	子线程输出的线程序号不能重复
////	全局变量的输出必须递增
//
//#include <iostream>
//#include <windows.h>
//#include <process.h>
//
//long g_nNum; //全局资源
//unsigned int _stdcall Fun(void* pPM); //线程函数
//const int THREAD_NUM = 10; //子线程数
//
////信号量和关键段
//HANDLE g_hThreadSemaphore;
////CRITICAL_SECTION g_csThreadCode;
//
//int main()
//{
//	printf("经典线程同步 信号量semaphore\n");
//
//	//初始化事件和关键段 自动置位 初试无触发的匿名事件
//	g_hThreadSemaphore = CreateSemaphore(NULL,0,1,NULL);
////	InitializeCriticalSection(&g_csThreadCode);
//
//	g_nNum = 0;
//	HANDLE handle[THREAD_NUM];
//	int i = 0;
//
//	while(i < THREAD_NUM)
//	{
//		handle[i] = (HANDLE)_beginthreadex(NULL,0,Fun,&i,0,NULL);
//		WaitForSingleObject(g_hThreadSemaphore,INFINITE);//等待事件被触发
//		i++;
//	}
//	WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
//
//	//销毁事件和关键段
//	CloseHandle(g_hThreadSemaphore);
////	DeleteCriticalSection(&g_csThreadCode);
//
//	system("pause");
//	return 0;
//}
//
//unsigned int _stdcall Fun(void* pPM)
//{
//	//由于创建线程需要一定的开销，所以新线程并不能第一时间执行到这
//	int nThreadNum = *(int*)pPM;
//
//
//	Sleep(50);
//
////	EnterCriticalSection(&g_csThreadCode);
//	g_nNum++;
//	Sleep(0);
//	printf("线程编号为%d  全局资源值为%d\n",nThreadNum,g_nNum);
////	LeaveCriticalSection(&g_csThreadCode);
//	ReleaseSemaphore(g_hThreadSemaphore,1,NULL);//触发事件
//	return 0;
//}