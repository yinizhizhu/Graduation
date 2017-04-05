#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;
//只需要使用一个读写锁就可以实现所需要的功能
SRWLOCK g_srwlock;
const int READER_NUM = 5;//读者个数

//读者线程函数
unsigned int _stdcall ReaderThreadFun(PVOID pM)
{
	cout << "编号为" << GetCurrentThreadId() <<"的读者进入等待中..."<<endl;

	//读者申请读取文件
	AcquireSRWLockShared(&g_srwlock);
	
	//读取文件
	cout <<  "编号为" << GetCurrentThreadId() << "开始读取文件"<<endl;
	Sleep(rand()%100);
	cout <<  "编号为" << GetCurrentThreadId() << "结束读取文件"<<endl;

	//读者结束读取文件
	ReleaseSRWLockShared(&g_srwlock);
	return 0;
}

//写者线程函数
unsigned int _stdcall WriterThreadFun(PVOID pM)
{
	cout << "    写线程进入等待中..."<<endl;
	
	//写者申请写文件
	AcquireSRWLockExclusive(&g_srwlock);

	//写文件
	cout << "    写线程开始写文件..."<<endl;
	//Sleep(rand()%100);
	Sleep(5000);
	cout << "    写线程结束写文件..."<<endl;

	ReleaseSRWLockExclusive(&g_srwlock);
	return 0;
}


int main()
{
	//初始化读写锁
	InitializeSRWLock(&g_srwlock);

	int i;
	HANDLE hThread[READER_NUM + 1];
	
	//先启动两个读者线程
	for(i = 1; i<3; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	Sleep(2000);

	//启动写进程
	hThread[0] = (HANDLE)_beginthreadex(NULL,0,WriterThreadFun,NULL,0,NULL);
		
	//最后启动其他读者线程
	for (; i<=READER_NUM; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	WaitForMultipleObjects(READER_NUM+1,hThread,TRUE,INFINITE);
	
	//关闭所有的线程handle
	for(i=0; i<READER_NUM+1; i++)
		CloseHandle(hThread[i]);
	
	system("pause");
	return 0;
}