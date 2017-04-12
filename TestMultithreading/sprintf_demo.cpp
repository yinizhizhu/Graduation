#include <iostream>
#include <string>
using namespace std;

int main() {
	char s[20];
	string s1;
	for (int i = 0; i < 10; i++) {
		sprintf(s, "%d.jpg", i);
		s1 = s;
		cout << s1 << endl;
	}
	system("pause");
}