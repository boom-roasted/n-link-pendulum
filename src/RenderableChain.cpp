#include "RenderableChain.h"

#include <filesystem>

RenderableChain::RenderableChain()
{
    chainOverTime_ = std::vector<Chain>();
    pinTexture_ = Texture();
    nodeTexture_ = Texture();
    currentChainIndex_ = 0;
}

RenderableChain::~RenderableChain() {}

bool
RenderableChain::loadFromFile(const std::string& p)
{
    bool success = true;

    if (!chainOverTime_.empty())
    {
        success = false;
    }
    else if (!std::filesystem::exists(p))
    {
        success = false;
    }
    else
    {
        chainOverTime_ = Chain::Deserialize(p);
    }

    return success;
}

void
RenderableChain::loadTextures(SDL_Renderer* renderer)
{
    pinTexture_.loadFromFile("dot.bmp", renderer);
    nodeTexture_.loadFromFile("dot.bmp", renderer);
    // TODO add asserts or something
}

Chain
RenderableChain::currentChain()
{
    return chainOverTime_[currentChainIndex_];
}

void
RenderableChain::increment(int by)
{
    const auto maxIndex = static_cast<int>(chainOverTime_.size());
    if (currentChainIndex_ + by < maxIndex)
        currentChainIndex_ += by;
    else
        currentChainIndex_ = 0; // Reset
}

void
RenderableChain::render(
    SDL_Renderer* renderer,
    double offsetX,
    double offsetY,
    double sf)
{
    if (chainOverTime_.empty())
        return;

    const auto& chain = currentChain();

    // Show the pin
    pinTexture_.render(
        chain.pin().x + offsetX, chain.pin().y + offsetY, renderer);

    // Show the nodes
    const auto nodes = chain.nodes();

    for (std::size_t i = 0; i < nodes.size(); i++)
    {
        const auto& node = nodes[i];

        nodeTexture_.render(
            node.state.x * sf + offsetX, node.state.y * sf + offsetY, renderer);
    }
}