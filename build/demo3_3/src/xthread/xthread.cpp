#include "xthread.h"
#include <iostream>
using namespace std;

void XThread::Start()
{
	cout<<"Start Thread"<<endl;
	th_ = std::thread(&XThread::Main,this);
}

void XThread::Wait()
{
	cout<<"begin Wait Thread"<<endl;
	th_.join();
	cout<<"end Wait Thread"<<endl;
}