#include <iostream>
#include <thread>
#include "xdata.h"
#include "test.h"
using namespace std;

void ThreadMain()
{
	cout<<"Thread Main"<<endl;
}

int main(int argc,char *argv[])
{
	thread th(ThreadMain);
	cout<<"test make "<<endl;
	th.join();
	XData d;
	return 0;
}
