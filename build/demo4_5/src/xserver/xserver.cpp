#include <iostream>
#include "xthread.h"
#include "xcom.h"
using namespace std;

/*
LD_LIBRARY_PATH=./; 
export LD_LIBRARY_PATH 
*/
class XTask:public XThread
{
public:
	XTask(int s):sec_(s){}
	void Main() override
	{
		cout<<"XTask main"<<endl;
		for(int i = 0; ; i++)
		{
			cout<<"test task "<<i<<endl;
			this_thread::sleep_for(chrono::seconds(sec_));
		}
	}
private:
	int sec_ = 3;	
};

int main(int argc,char *argv[])
{
	cout<<"XServer"<<endl;
	int sec = 1;
	if(argc>1)
		sec = atoi(argv[1]);
	XCom com;
	XTask task(sec);
	task.Start();
	task.Wait();
	return 0;
}