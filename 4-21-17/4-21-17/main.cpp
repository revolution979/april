#include <stdio.h>
#include <iostream>
#include <ctime>
using namespace std;
int num;
void colorchange();

int main() {
srand(time(NULL));
	while (1) {
		
		colorchange();
		system("PAUSE");

	}

}
void colorchange() {

	int num = rand() % 100 + 1;

	if (num <= 25) {

		cout << "blue" << endl;
		//num;
			system("color 19");
		}
	else if (num >= 26 && num<= 50) {

		cout << "green" << endl;
		//num;
		system("color 2a");
	}
	else if (num >= 51 && num <= 100) {

		cout << "red" << endl;
		//num;
		system("color 4c");
	}
}