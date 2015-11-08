#include <iostream>
#include <string>

void increment() {
	int v[] = {0,1,2,3,4,5,6,7,8,9};

	for( auto& x : v ) {
		++x;
	}
}

enum class ValueClass: unsigned int {
	Int,
	Bool,
	Float,
	Double,
	Char,
	Pointer
};

struct Data {
	int x, y;
};

union Any
{
	int invValue;
	float floatValue;
	Data dataValue;

	Any() :invValue(), floatValue() {

	}
};

int main(int argc, char* argv[]) {
	ValueClass vc = ValueClass::Double;

	auto v = 34;

	return 0;
}