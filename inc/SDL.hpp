#ifndef SDL_HPP
#define SDL_HPP

namespace tetra
{
    class SDL
    {
    public:
        SDL();
        SDL(const SDL&) = delete;
        SDL(SDL&&);
        ~SDL();

    private:
        bool shouldQuit;
    };
}; /* namespace tetra */

#endif
