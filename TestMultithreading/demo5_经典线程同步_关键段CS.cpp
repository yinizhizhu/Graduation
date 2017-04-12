//程序描述：
//	主线程启动10个子线程，并将表示子线程序号的变量地址作为参数，传递给子线程。子线程
//接收参数->sleep(50)->全局变量++ ->sleep(0) ->输出参数和全局变量
//要求：
//	子线程输出的线程序号不能重复
//	全局变量的输出必须递增

#include <iostream>
#include <windows.h>
#include <process.h>

long g_nNum; //全局资源
unsigned int _stdcall Fun(void* pPM); //线程函数
const int THREAD_NUM = 10; //子线程数

//关键段变量声明
CRITICAL_SECTION g_csThreadParameter, g_csThreadCode;

int main() {
	printf("经典线程同步 关键段\n");

	//关键段初始化
	InitializeCriticalSection(&g_csThreadParameter);
	InitializeCriticalSection(&g_csThreadCode);

	g_nNum = 0;
	HANDLE handle[THREAD_NUM];
	int i = 0;
	while (i < THREAD_NUM) {
		EnterCriticalSection(&g_csThreadParameter);//进入子线程序号关键区域
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
		i++;
	}
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
	DeleteCriticalSection(&g_csThreadCode);
	DeleteCriticalSection(&g_csThreadParameter);
	system("pause");
	return 0;
}

unsigned int _stdcall Fun(void* pPM) {
	int nThreadNum = *(int*)pPM;
	LeaveCriticalSection(&g_csThreadParameter);//离开子线程序号关键区域

	Sleep(50);

	EnterCriticalSection(&g_csThreadCode);//进入子线程互斥区域
	g_nNum++;
	Sleep(0);
	printf("线程编号为%d  全局资源值为%d\n", nThreadNum, g_nNum);
	LeaveCriticalSection(&g_csThreadCode);//离开子线程互斥区域
	return 0;
}

//因此可以将关键段比作旅馆的房卡，调用EnterCriticalSection()即申请房卡，
//得到房卡后自己当然是可以多次进出房间的，在你调用LeaveCriticalSection()交出房卡之前，别人自然是无法进入该房间

//所以不能保证同步，只能确定互斥