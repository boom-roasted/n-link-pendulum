#include "Grid.h"

Grid::Grid(
    const SDL_Rect& rect,
    const SDL_Point& origin,
    const SDL_Color& color,
    int spacingH,
    int spacingV,
    double scaleFactor)
    : rect_(rect)
    , origin_(origin)
    , color_(color)
    , spacingH_(spacingH)
    , spacingV_(spacingV)
    , scaleFactor_(scaleFactor)
{
}

SDL_Rect
Grid::rect()
{
    return rect_;
}
void
Grid::setRect(const SDL_Rect& rect)
{
    rect_ = rect;
}

void
Grid::setOrigin(const SDL_Point& origin)
{
    origin_ = origin;
}

void
Grid::setScaleFactor(double sf)
{
    scaleFactor_ = sf;
}

void
Grid::render(SDL_Renderer* renderer)
{
    // Line width isn't controllable without sdl2_gfx
    // Could instead can make a rectangle for thicker lines

    // Start on the far left side of the screen and find the next
    // available grid line. A grid line is available if it's pixel
    // distance to the origin is a multiple of the pixel spacing
    // in that direction.
    //
    // Pixel spacing is the number of pixels between each grid line
    // For example, if the scale factor is 1, then a spacing of 10
    // would be 10 pixels apart. However, if the scale factor is 2,
    // then a spacing of 10 would be 20 pixels apart.

    // Set drawing color
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);

    const int pixelSpacingH = scaleFactor_ * spacingH_;
    const int pixelSpacingV = scaleFactor_ * spacingV_;

    // Drawing bounds
    const int xMin = rect_.x;
    const int xMax = rect_.x + rect_.w;
    const int yMin = rect_.y;
    const int yMax = rect_.y + rect_.h;

    // Find first vertical grid line x coordinate
    int x = rect_.x;

    for (int xi = x; xi < xMax; xi++)
    {
        int distanceToOrigin = std::abs(xi - origin_.x);
        if (distanceToOrigin % pixelSpacingH == 0)
        {
            x = xi;
            break;
        }
    }

    // Draw all grid lines until we reach the edge of our rect's bounds
    while (x < xMax)
    {
        SDL_RenderDrawLine(renderer, x, yMin, x, yMax);
        x += pixelSpacingH;
    }

    // Do it all again for the horizontal lines
    int y = rect_.y;

    for (int yi = y; yi < yMax; yi++)
    {
        int distanceToOrigin = std::abs(yi - origin_.y);
        if (distanceToOrigin % pixelSpacingV == 0)
        {
            y = yi;
            break;
        }
    }

    while (y < yMax)
    {
        SDL_RenderDrawLine(renderer, xMin, y, xMax, y);
        y += pixelSpacingV;
    }
}

void
Grid::renderAxes(SDL_Renderer* renderer)
{
    // Set drawing color
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);

    int x1 = rect_.x;
    int x2 = rect_.x + rect_.w;

    // Horizontal axis
    SDL_RenderDrawLine(renderer, x1, origin_.y, x2, origin_.y);

    int y1 = rect_.y;
    int y2 = rect_.y + rect_.h;

    // Vertical axis
    SDL_RenderDrawLine(renderer, origin_.x, y1, origin_.x, y2);
}