#include <iostream>
#include "xcom.h"
#include "xthread.h"

extern void TestCpp();
extern "C" void TestC();
using namespace std;


class XTask:public XThread
{
public:
	void Main() override
	{
		cout<<"XTask main"<<endl;
	}
};

int main(int argc,char *argv[])
{
	XCom xcom;
	TestCpp();
	TestC();

	XTask task;
	task.Start();
	task.Wait();
	return 0;
}
