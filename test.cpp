#include <iostream>
#include <sstream> // for stringstream
#include <string>
#include <cstdlib> //for rand() and srand()
#include <ctime> //for time()

using namespace std;

struct student{
	int worth;
}

void Show(student* s){
	cout << s->worth << endl;
}

int main(){
	student guy;
	guy[0].worth = 29;
	Show(&guy);
	
return 0;
}
