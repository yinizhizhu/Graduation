//时间：2016-03-23
//网址：http://blog.csdn.net/morewindows/article/details/7596034
//内容：读写者问题，其中写者写的时候不能读，写完之后，读者都可以读没有限制
//		实现的过程：一共有五个读者，首先是让两个读者读，然后开启写者线程；后面在开启另外三个读者的线程
//		其中有两个事件，其中一个无读者事件、一个写完事件


#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

const int READER_NUM = 5;

HANDLE g_hEventWriter,g_hEventNoReader;
volatile int g_nReaderCount;
CRITICAL_SECTION g_cs_reader_counter;

//读者线程
unsigned int _stdcall ReaderThreadFun(PVOID pM) {
	cout << "编号为" << GetCurrentThreadId() <<"的读者进入等待中..."<<endl;
	//等待写者完成写文件
	WaitForSingleObject(g_hEventWriter,INFINITE);

	//增加读者个数
	EnterCriticalSection(&g_cs_reader_counter);
	g_nReaderCount++;
	if (g_nReaderCount == 1) {
		ResetEvent(g_hEventNoReader);
	}
	LeaveCriticalSection(&g_cs_reader_counter);

	//读取文件
	cout <<  "编号为" << GetCurrentThreadId() << "开始读取文件"<<endl;
	Sleep(rand()%100);

	//结束阅读，读者个数减少空位增加
	cout <<  "编号为" << GetCurrentThreadId() << "结束读取文件"<<endl;
	EnterCriticalSection(&g_cs_reader_counter);
	g_nReaderCount--;
	if (g_nReaderCount == 0) {
		SetEvent(g_hEventNoReader);
	}
	LeaveCriticalSection(&g_cs_reader_counter);
	return 0;
}

//写者线程
unsigned int _stdcall WriterThreadFun(PVOID pM) {
	cout << "    写线程进入等待中..."<<endl;
	//等待读文件的读者为零
	WaitForSingleObject(g_hEventNoReader,INFINITE);
	//标记写者正在写文件
	ResetEvent(g_hEventWriter);

	//写文件
	cout << "    写线程开始写文件..."<<endl;
	//Sleep(rand()%100);
	Sleep(5000);
	cout << "    写线程结束写文件..."<<endl;

	//标记写者结束写文件
	SetEvent(g_hEventWriter);
	return 0;
}


int main() {
	//初始化关键段
	InitializeCriticalSection(&g_cs_reader_counter);

	//初始化事件
	g_hEventWriter = CreateEvent(NULL,TRUE,TRUE,NULL);
	g_hEventNoReader = CreateEvent(NULL,FALSE,TRUE,NULL);
	g_nReaderCount = 0;

	int i;
	HANDLE hThread[READER_NUM + 1];

	//先启动两个读者线程
	for(i = 1; i<3; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	//启动写进程
	hThread[0] = (HANDLE)_beginthreadex(NULL,0,WriterThreadFun,NULL,0,NULL);
	
	Sleep(500);
	//最后启动其他读者线程
	for (; i<=READER_NUM; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ReaderThreadFun,NULL,0,NULL);
	WaitForMultipleObjects(READER_NUM+1,hThread,TRUE,INFINITE);

	//关闭所有的线程handle
	for(i=0; i<READER_NUM+1; i++)
		CloseHandle(hThread[i]);

	//关闭事件handle和关键段
	CloseHandle(g_hEventNoReader);
	CloseHandle(g_hEventWriter);
	DeleteCriticalSection(&g_cs_reader_counter);
	system("pause");
	return 0;
}