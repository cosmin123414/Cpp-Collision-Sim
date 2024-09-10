#include "simulations.h"
#include <SDL2/SDL.h>

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void drawCircle(SDL_Renderer* renderer, Particle p) {

    double x = p.getPos().first;
    double y = p.getPos().second;
    double r = p.getRadius();
    double angle = p.getOrientation();
    vector<int> col1 = p.getColor1();
    vector<int> col2 = p.getColor2();

    // Draw the outer circle
    SDL_SetRenderDrawColor(renderer, col1[0], col1[1], col1[2], 255); // Outer circle color
    SDL_RenderFillCircle(renderer, x, y, r);

    // Draw the inner circle for orientation
    double small_x = x + r*0.6*cos(angle);
    double small_y = y + r*0.6*sin(angle);
    SDL_SetRenderDrawColor(renderer, col2[0], col2[1], col2[2], 255); // Inner circle color
    SDL_RenderFillCircle(renderer, small_x, small_y, r/5);
}

void synchrony(SDL_Renderer * renderer, int width, int height){

    //Setup environment
    Environment env = Environment(0, 1, width, height);
    env.addParticle(Particle(1, 20, 0.01, 0, {100, 400}, {0, 0}, {200, 19, 88}, {21, 104, 170}, true));

}
