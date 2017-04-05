//#include <iostream>
//#include <windows.h>
//#include <process.h>
//
//volatile long g_nLoginCount;//登陆次数
//const DWORD THREAD_NUM = 100;//启动线程数
//
//unsigned int _stdcall ThreadFun(void* pPM)
//{
//	Sleep(100);
//	//g_nLoginCount++;//导致的原因主要是由于，++操作的时候，需要将内存中的值读入到寄存器中，如果出现多线程，就会导致上一个进程还没有完成，下面一个进程就开始了，出现了两个一样的值
//	InterlockedIncrement(&g_nLoginCount);
//	Sleep(50);
//	return 0;
//}
//
//int main()
//{
//	printf("原子操作 Interlocked系列函数的使用\n");
//
//	//重复20次以便观察多线程访问同一资源时导致的冲突
//	int num = 20;
//	while(num--)
//	{
//		g_nLoginCount = 0;
//
//		HANDLE handle[THREAD_NUM];
//		for (int i = 0; i < THREAD_NUM; i++)
//			handle[i] = (HANDLE)_beginthreadex(NULL,0,ThreadFun,NULL,0,NULL);
//
//		WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
//		printf("有%d个用户登陆后记录结果是%d\n",THREAD_NUM,g_nLoginCount);
//	}
//	
//	system("pause");
//	return 0;
//}
