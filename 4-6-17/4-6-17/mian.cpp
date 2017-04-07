#include <iostream>
using namespace std;
void show_menu();
//Print the help menu, as shown above.
void show_title(int year);
//Print the title for that year.
void show_age
(int index);
//Print the age of the book for that index
char input;
int main() {

int number;
	cout << "press q to quit, h for menu, y for title, i for age" << endl;
	while (input != 'q'){ 
		
		cin >>input;

		if (input == 'h') {
			show_menu();
		}
		if (input == 'y') {
			cin >> number;
			show_title(number);
		}
		if (input == 'i') {
			cin >> number;
			show_age(number);
		}
	}
}
void show_menu() {

	cout << "show menu" << endl;

}

void show_title(int year) {

	cout << "show title" << endl;

}

void show_age(int index) {

	cout << "show age" << endl;  

}
