//����������
//	���߳�����10�����̣߳�������ʾ���߳���ŵı�����ַ��Ϊ���������ݸ����̡߳����߳�
//���ղ���->sleep(50)->ȫ�ֱ���++ ->sleep(0) ->���������ȫ�ֱ���
//Ҫ��
//	���߳�������߳���Ų����ظ�
//	ȫ�ֱ���������������

#include <iostream>
#include <windows.h>
#include <process.h>

long g_nNum; //ȫ����Դ
unsigned int _stdcall Fun(void* pPM); //�̺߳���
const int THREAD_NUM = 10; //���߳���

//�ź����͹ؼ���
HANDLE g_hThreadSemaphore;
CRITICAL_SECTION g_csThreadCode;

int main() {
	printf("�����߳�ͬ�� �ź���semaphore\n");

	//��ʼ���¼��͹ؼ��� �Զ���λ �����޴����������¼�
	g_hThreadSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	InitializeCriticalSection(&g_csThreadCode);

	g_nNum = 0;
	HANDLE handle[THREAD_NUM];
	int i = 0;

	while (i < THREAD_NUM) {
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
		WaitForSingleObject(g_hThreadSemaphore, INFINITE);//�ȴ��¼�������
		i++;
	}
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	//�����¼��͹ؼ���
	CloseHandle(g_hThreadSemaphore);
	DeleteCriticalSection(&g_csThreadCode);

	system("pause");
	return 0;
}

unsigned int _stdcall Fun(void* pPM) {
	//���ڴ����߳���Ҫһ���Ŀ������������̲߳����ܵ�һʱ��ִ�е���
	int nThreadNum = *(int*)pPM;
	ReleaseSemaphore(g_hThreadSemaphore, 1, NULL);//�����¼�

	Sleep(50);

	EnterCriticalSection(&g_csThreadCode);
	g_nNum++;
	Sleep(0);
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);
	LeaveCriticalSection(&g_csThreadCode);
	return 0;
}

//
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
////�ź����͹ؼ���
//HANDLE g_hThreadSemaphore;
////CRITICAL_SECTION g_csThreadCode;
//
//int main() {
//	printf("�����߳�ͬ�� �ź���semaphore\n");
//
//	//��ʼ���¼��͹ؼ��� �Զ���λ �����޴����������¼�
//	g_hThreadSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
//	//	InitializeCriticalSection(&g_csThreadCode);
//
//	g_nNum = 0;
//	HANDLE handle[THREAD_NUM];
//	int i = 0;
//
//	while (i < THREAD_NUM) {
//		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
//		WaitForSingleObject(g_hThreadSemaphore, INFINITE);//�ȴ��¼�������
//		i++;
//	}
//	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
//
//	//�����¼��͹ؼ���
//	CloseHandle(g_hThreadSemaphore);
//	
//	//	DeleteCriticalSection(&g_csThreadCode);
//	system("pause");
//	return 0;
//}
//
//unsigned int _stdcall Fun(void* pPM) {
//	//���ڴ����߳���Ҫһ���Ŀ������������̲߳����ܵ�һʱ��ִ�е���
//	int nThreadNum = *(int*)pPM;
//	Sleep(50);
//	//	EnterCriticalSection(&g_csThreadCode);
//	g_nNum++;
//	Sleep(0);
//	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);
//	//	LeaveCriticalSection(&g_csThreadCode);
//	ReleaseSemaphore(g_hThreadSemaphore, 1, NULL);//�����¼�
//	return 0;
//}