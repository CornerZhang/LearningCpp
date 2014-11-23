#include <iostream>

void increment() {
	int v[] = {0,1,2,3,4,5,6,7,8,9};

	for( auto& x : v ) {
		++x;
	}
}

int main(int argc, char* argv[]) {
	std::cout<<"Hello world!"<<std::endl;
	return 0;
}