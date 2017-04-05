//#include <stdio.h>
//#include <Windows.h>
//
//char s[100];
//int count;
//HANDLE handler[4];
//HANDLE hl[4];
//DWORD WINAPI function(LPVOID pm)
//{
//	int tmp = (int)pm;
//	char c = 'A' + tmp;
//	while (true)
//	{
//		WaitForSingleObject(handler[tmp], INFINITE);
//		if (count >= 100)
//		{
//			break;
//		}
//		printf("%d curThread:%c  count = %d\n", tmp, c, count);
//		s[count] = (char)pm;
//		count++;
//		Sleep(100);
//		SetEvent(handler[(tmp+1)%4]);
//	}    
//	return 0;
//}
//
//int main()
//{
//	for (int i = 0; i < 4; i++)
//	{
//		handler[i] = CreateEvent(NULL, false, false, NULL);
//		hl[i] = CreateThread(NULL, 0, function, (void *)i, 0, NULL);
//	}    
//	printf("start\n");
//	SetEvent(handler[0]);
//	WaitForMultipleObjects(4, handler, true, INFINITE);
//	printf("done!\n");
//	return 0;
//}