#include <tetra/behavior/Debug.hpp>

#include <iostream>

using namespace tetra;
using namespace std;

unique_ptr<Echo>
Echo::message(const string& msg)
{
    return unique_ptr<Echo>{new Echo{msg}};
}

Echo::Echo(const std::string& msg)
    : msg{msg}
{ }

void
Echo::run(double dt)
{
    cout << msg << endl;
    this->complete = true;
}
