////����������
////	���߳�����10�����̣߳�������ʾ���߳���ŵı�����ַ��Ϊ���������ݸ����̡߳����߳�
////���ղ���->sleep(50)->ȫ�ֱ���++ ->sleep(0) ->���������ȫ�ֱ���
////Ҫ��
////	���߳�������߳���Ų����ظ�
////	ȫ�ֱ���������������
//
//#include <iostream>
//#include <windows.h>
//#include <process.h>
//
//long g_nNum; //ȫ����Դ
//unsigned int _stdcall Fun(void* pPM); //�̺߳���
//const int THREAD_NUM = 10; //���߳���
//
////�ؼ��α�������
//CRITICAL_SECTION g_csThreadParameter, g_csThreadCode;
//
//int main()
//{
//	printf("�����߳�ͬ�� �ؼ���\n");
//
//	//�ؼ��γ�ʼ��
//	InitializeCriticalSection(&g_csThreadParameter);
//	InitializeCriticalSection(&g_csThreadCode);
//
//	g_nNum = 0;
//	HANDLE handle[THREAD_NUM];
//	int i = 0;
//	while(i < THREAD_NUM)
//	{
//		EnterCriticalSection(&g_csThreadParameter);//�������߳���Źؼ�����
//		handle[i] = (HANDLE)_beginthreadex(NULL,0,Fun,&i,0,NULL);
//		i++;
//	}
//	WaitForMultipleObjects(THREAD_NUM,handle,TRUE,INFINITE);
//	DeleteCriticalSection(&g_csThreadCode);
//	DeleteCriticalSection(&g_csThreadParameter);
//	system("pause");
//	return 0;
//}
//
//unsigned int _stdcall Fun(void* pPM)
//{
//	int nThreadNum = *(int*)pPM;
//	LeaveCriticalSection(&g_csThreadParameter);//�뿪���߳���Źؼ�����
//
//	Sleep(50);
//
//	EnterCriticalSection(&g_csThreadCode);//�������̻߳�������
//	g_nNum++;
//	Sleep(0);
//	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n",nThreadNum,g_nNum);
//	LeaveCriticalSection(&g_csThreadCode);//�뿪���̻߳�������
//	return 0;
//}
//
////��˿��Խ��ؼ��α����ùݵķ���������EnterCriticalSection()�����뷿����
////�õ��������Լ���Ȼ�ǿ��Զ�ν�������ģ��������LeaveCriticalSection()��������֮ǰ��������Ȼ���޷�����÷���
//
////���Բ��ܱ�֤ͬ����ֻ��ȷ������