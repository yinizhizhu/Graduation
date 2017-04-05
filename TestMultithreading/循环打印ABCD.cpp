//#include <iostream>
//#include <Windows.h>
//
//using namespace std;
//
//int count = 0;
//DWORD WINAPI function(LPVOID pm)
//{
//	int tmp = (int)pm;
//	char c = 'A' + tmp;
//	while(true)
//	{
//		if (count >= 100)
//		{
//			break;
//		}
//		printf("%d curThread:%c count = %d\n",tmp,c,count);
//		count++;
//	}
//	return 0;
//}
//
//int main()
//{
//	HANDLE hl[4];
//	for (int i = 0; i < 4; i++)
//	{
//		hl[i] = CreateThread(NULL,0,function,(void*)i,0,NULL);
//	}
//	system("pause");
//	return 0;
//}