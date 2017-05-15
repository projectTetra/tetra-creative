#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <string>

namespace tetra
{
    /**
     * Load the shader source code from the asset directory.
     * By convention shader source code is located in the ASSET_ROOT/shaders directory.
     * @param name The name (not path) of the shader file.
     * @return The shader's source code as a string
     */
    std::string loadShaderSrc(const std::string& name);

    class FailedToLoadAsset : public std::exception
    {
    public:
        FailedToLoadAsset(const std::string& name,
                          const std::string& dir,
                          const std::string& reason);
        const char* what() const noexcept override;
    private:
        std::string msg;
    };
}

#endif
