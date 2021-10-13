#ifndef RENDERABLECHAIN_H
#define RENDERABLECHAIN_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Chain.h"
#include "Texture.h"

class RenderableChain
{
public:
    RenderableChain();

    ~RenderableChain();

    bool loadFromFile(const std::string& p);

    void loadTextures(SDL_Renderer* renderer);

    Chain currentChain();

    void increment(int by);

    void
    render(SDL_Renderer* renderer, double offsetX, double offsetY, double sf);

private:
    std::vector<Chain> chainOverTime_;

    Texture pinTexture_;

    Texture nodeTexture_;

    int currentChainIndex_;

    struct Point
    {
        double x;
        double y;

        Point(double x, double y)
            : x(x)
            , y(y)
        {
        }
    };
};

#endif // RENDERABLECHAIN_H