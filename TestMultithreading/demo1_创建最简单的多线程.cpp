#include <iostream>
#include <Windows.h>
using namespace std;

//子线程函数
DWORD WINAPI ThreadFun(LPVOID pM) {
	cout<<"子线程ID号为:"<<GetCurrentThreadId()<<endl;
	cout<<"子线程输出Hello World"<<endl;
	return 0;
}

int main() {
	 cout << "最简单的创建多线程实例" << endl;
	 HANDLE handle = CreateThread(NULL,0,ThreadFun,NULL,NULL,NULL);
	 WaitForSingleObject(handle,INFINITE);
	 system("pause");
	 return 0;
}
