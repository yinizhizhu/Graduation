#include <stdio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

int ans = 0;

DWORD WINAPI function(LPVOID pm) {
	int tmp = (int)pm;
	char c = 'a' + tmp;
	while (true) {
		if (ans >= 100) {
			break;
		}
		printf("%d curthread:%c count = %d\n", tmp, c, ans);
		ans++;
	}
	return 0;
}

int main() {
	HANDLE hl[4];
	for (int i = 0; i < 4; i++) {
		hl[i] = CreateThread(NULL, 0, function, (void*)i, 0, NULL);
	}
	system("pause");
	return 0;
}