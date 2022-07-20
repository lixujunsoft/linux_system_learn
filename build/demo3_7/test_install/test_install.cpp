#include <iostream>
#include <thread>
using namespace std;

int main(int argc,char *argv[])
{
	int sec = 1;
	if(argc>1)
		sec=atoi(argv[1]);
	for(int i = 0;;i++)
	{
		cout<<"test install "<<i<<endl;
		this_thread::sleep_for(chrono::seconds(sec));
	}
	return 0;
}
