#include <Assets.hpp>
#include <AssetRoot.h>

#include <fstream>
#include <sstream>

using namespace std;
using namespace tetra;

namespace
{
    /**
     * Compute the full path to the shader file using the ASSET_ROOT.
     * By convention shaders are located at ASSET_ROOT/shaders.
     */
    string fullShaderPath(const string& name)
    {
        static const string shaderRoot = string(ASSET_ROOT) + "/shaders/";

        // TODO:  use c++17 filesystem lib or boost filesystem
        return shaderRoot + name;
    }
}

string tetra::loadShaderSrc(const string& name)
{
    ifstream file(fullShaderPath(name));
    if (file.is_open())
    {
        auto source = ostringstream{};
        source << file.rdbuf();
        return source.str();
    }
    else
    {
        throw FailedToLoadAsset{ name
                               , fullShaderPath(name)
                               , "file could not be opened!"
                               };
    }
}

FailedToLoadAsset::FailedToLoadAsset(const string& name,
                                     const string& dir,
                                     const string& reason)
{
    stringstream ss;
    ss << "FailedToLoadAsset, " << name << ", from directory '"
       << dir << "' because " << reason;
    msg = ss.str();
}

const char*
FailedToLoadAsset::what() const noexcept
{
    return msg.c_str();
}

