#include <iostream>
#include <Windows.h>
using namespace std;

//���̺߳���
DWORD WINAPI ThreadFun(LPVOID pM) {
	cout<<"���߳�ID��Ϊ:"<<GetCurrentThreadId()<<endl;
	cout<<"���߳����Hello World"<<endl;
	return 0;
}

int main() {
	 cout << "��򵥵Ĵ������߳�ʵ��" << endl;
	 HANDLE handle = CreateThread(NULL,0,ThreadFun,NULL,NULL,NULL);
	 WaitForSingleObject(handle,INFINITE);
	 system("pause");
	 return 0;
}
