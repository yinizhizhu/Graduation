//#include <iostream>
//#include <windows.h>
//#include <process.h>
//
//volatile long g_nLoginCount;//��½����
//const DWORD THREAD_NUM = 100;//�����߳���
//
//unsigned int _stdcall ThreadFun(void* pPM)
//{
//	Sleep(100);
//	//g_nLoginCount++;//���µ�ԭ����Ҫ�����ڣ�++������ʱ����Ҫ���ڴ��е�ֵ���뵽�Ĵ����У�������ֶ��̣߳��ͻᵼ����һ�����̻�û����ɣ�����һ�����̾Ϳ�ʼ�ˣ�����������һ����ֵ
//	InterlockedIncrement(&g_nLoginCount);
//	Sleep(50);
//	return 0;
//}
//
//int main()
//{
//	printf("ԭ�Ӳ��� Interlockedϵ�к�����ʹ��\n");
//
//	//�ظ�20���Ա�۲���̷߳���ͬһ��Դʱ���µĳ�ͻ
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
//		printf("��%d���û���½���¼�����%d\n",THREAD_NUM,g_nLoginCount);
//	}
//	
//	system("pause");
//	return 0;
//}
