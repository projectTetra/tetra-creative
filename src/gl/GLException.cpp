#include <gl/GLException.hpp>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace tetra;

GLException::GLException(initializer_list<string> messages)
{
    errorMsg = accumulate(
        begin(messages), end(messages),
        string("GLException:"),
        [](string fullMsg, string curMsg)
        {
            return fullMsg + " " + curMsg;
        });
}

const char*
GLException::what() const noexcept
{
    return errorMsg.c_str();
}

