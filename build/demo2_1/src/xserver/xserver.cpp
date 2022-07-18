#include <iostream>
#include "xthread.h"
using namespace std;

/*
LD_LIBRARY_PATH=./; 
export LD_LIBRARY_PATH 
*/
class XTask : public XThread
{
public:
	void Main() override
	{
		cout << "XTask main" << endl;
	}
};

int main(int argc,char *argv[])
{
	cout << "XServer" << endl;
	XTask task;
	task.Start();
	task.Wait();
	return 0;
}