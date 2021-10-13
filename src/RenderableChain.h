#ifndef RENDERABLECHAIN_H
#define RENDERABLECHAIN_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Texture.h"
#include "Chain.h"

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
};

#endif // RENDERABLECHAIN_H