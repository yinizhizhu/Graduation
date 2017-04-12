//程序描述：
//	主线程启动10个子线程，并将表示子线程序号的变量地址作为参数，传递给子线程。子线程
//接收参数->sleep(50)->全局变量++ ->sleep(0) ->输出参数和全局变量
//要求：
//	子线程输出的线程序号不能重复
//	全局变量的输出必须递增

//搭建整个函数的整体框架
//可以发现在函数出现的结果比较混乱
#include <iostream>
#include <windows.h>
#include <process.h>

long g_nNum; //全局资源
unsigned int _stdcall Fun(void* pPM); //线程函数
const int THREAD_NUM = 10; //子线程数

int main() {
	g_nNum = 0;
	HANDLE handle[THREAD_NUM];
	
	int i = 0;
	while(i < THREAD_NUM) {
		handle[i] = (HANDLE)_beginthreadex(NULL,0,Fun,&i,0,NULL);
		i++;
	}
	WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
	system("pause");
	return 0;
}

unsigned int _stdcall Fun(void* pPM) {
	//由于创建线程需要一定的开销，所以新线程并不能第一时间执行到这
	int nThreadNum = *(int*)pPM;
	Sleep(50);
	g_nNum++;
	Sleep(0);
	printf("线程编号为%d  全局资源值为%d\n",nThreadNum,g_nNum);
	return 0;
}