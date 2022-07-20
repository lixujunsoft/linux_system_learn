#include <iostream>

extern void TestCpp();
extern "C" void TestC();
using namespace std;

int main(int argc,char *argv[])
{
	TestCpp();
	TestC();
	return 0;
}
