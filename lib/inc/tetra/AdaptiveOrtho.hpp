#ifndef ADAPTIVE_ORTHO_HPP
#define ADAPTIVE_ORTHO_HPP

#include <tetra/EventStream.hpp>
#include <sdl/SDLEvents.hpp>
#include <glm/mat4x4.hpp>

namespace tetra
{
    /**
     * This class represents an orthographic projection matrix which automatically
     * adapts to the screen dimensions.
     */
    class AdaptiveOrtho
    {
    public:
        AdaptiveOrtho(EventStream& stream, float scale = 1.0);
        AdaptiveOrtho(const AdaptiveOrtho&) = delete;
        AdaptiveOrtho(AdaptiveOrtho&& from) = default;

        void onWindowSize(const SDLWindowSize& event);

        const glm::mat4& value();

    private:
        EventStream::AutoRemoveListener resizeScreen;
        glm::mat4 projection;
        float scale;
    };
};

#endif
