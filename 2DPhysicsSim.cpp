#include <SDL2/SDL.h>
#include "Environment.h"
#include "Particle.h"
#include <cmath>
#include <iostream>
#include <random>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TARGET_FPS = 200;
const int FRAME_DELAY = 1000 / TARGET_FPS;
constexpr double pi = 3.14159265358979323846;

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
    SDL_SetRenderDrawColor(renderer, col1[0], col1[1], col1[2], 255);
    SDL_RenderFillCircle(renderer, x, y, r);

    // Draw the inner circles for orientation
    SDL_SetRenderDrawColor(renderer, col2[0], col2[1], col2[2], 255);
    SDL_RenderFillCircle(renderer, x, y, r/4);
    SDL_RenderFillCircle(renderer, x + r*0.5*cos(angle), y + r*0.5*sin(angle), r/6);

    SDL_SetRenderDrawColor(renderer, col2[0], col2[1], col2[2], 255);
    SDL_RenderFillCircle(renderer, x - r*0.5*cos(angle), y - r*0.5*sin(angle), r/6);

    SDL_SetRenderDrawColor(renderer, col2[0], col2[1], col2[2], 255);
    SDL_RenderFillCircle(renderer, x + r*0.5*cos(angle + pi/2), y + r*0.5*sin(angle + pi/2), r/6);

    SDL_SetRenderDrawColor(renderer, col2[0], col2[1], col2[2], 255);
    SDL_RenderFillCircle(renderer, x + r*0.5*cos(angle - pi/2), y + r*0.5*sin(angle - pi/2), r/6);
}

void random(){

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Filled Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    Environment env = Environment(0, 1, SCREEN_WIDTH, SCREEN_HEIGHT);


    // Vector of colors (RGB)
    vector<vector<int>> colors = {{178,34,34}, {178,34,34}, {85,107,47}, {0,128,128}, {106,90,205}, {160,82,45}, {188,143,143},
                                  {112,128,144}, {220,220,220}};
    vector<pair<double, double>> positions = {{50, 50}, {150, 125}, {250, 50}, {350, 140}, {300, 300}, {410, 280}, {400, 500}, {200, 490}, {520, 510}};

    // 10 particles with random radius/mass/velocity
    for (int i=0; i<9; ++i){
        int radius = rand()%30 + 20;        // approx uniform [20, 60]
        int mass = radius/20;
        pair<double, double> velocity = {(rand()%20)/10.0 + 1, (rand()%20)/10 + 1};
        env.addParticle(Particle(mass, radius, 0, 0, positions[i], velocity, colors[i], colors[i], true));
    }


    bool running = true;
    while (running){
        Uint32 frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        //draw background
        SDL_SetRenderDrawColor(renderer, 219, 236, 244, 255);
        SDL_RenderClear(renderer);

        //Handle interactions
        env.handleWallCollisions(env.getWallCollisions());
        env.handleParticleCollisions(env.getParticleCollisions());

        for(auto & particle:env.getParticles()){
            particle.updatePosition(1);
            drawCircle(renderer, particle);}

        SDL_RenderPresent(renderer);

        // Frame duration management
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gasMixing() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Gas Mixing Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    Environment env = Environment(0, 1, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Colors for two types of gas
    vector<int> color1 = {178, 34, 34};  // Red
    vector<int> color2 = {0, 128, 128};  // Teal

    // Number of particles for each side
    int numParticles = 35;

    // Helper function to check if a position is valid
    auto isValidPosition = [&](pair<double, double> pos, int radius) {
        for (const auto& particle : env.getParticles()) {
            double dx = particle.getPos().first - pos.first;
            double dy = particle.getPos().second - pos.second;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < (particle.getRadius() + radius)) {
                return false;
            }
        }
        return true;
    };

    // Spawn particles on the left side with color1
    for (int i = 0; i < numParticles; ++i) {
        pair<double, double> pos;
        do {
            pos.first = rand() % (SCREEN_WIDTH / 2 - 2 * 15) + 15;
            pos.second = rand() % (SCREEN_HEIGHT - 2 * 15) + 15;
        } while (!isValidPosition(pos, 20));
        pair<double, double> velocity = {(rand() % 50) / 10.0 - 1, (rand() % 50) / 10.0 - 1};
        env.addParticle(Particle(1, 15, 0, 0, pos, velocity, color1, color1, true));
    }

    // Spawn particles on the right side with color2
    for (int i = 0; i < numParticles; ++i) {
        int radius = rand() % 20 + 10;        // approx uniform [10, 30]
        int mass = radius / 10;
        pair<double, double> pos;
        do {
            pos.first = rand() % (SCREEN_WIDTH / 2 - 2 * 15) + SCREEN_WIDTH / 2 + 15;
            pos.second = rand() % (SCREEN_HEIGHT - 2 * 15) + 15;
        } while (!isValidPosition(pos, 20));
        pair<double, double> velocity = {(rand() % 50) / 10.0 - 1, (rand() % 50) / 10.0 - 1};
        env.addParticle(Particle(1, 15, 0, 0, pos, velocity, color2, color2, true));
    }

    bool running = true;
    while (running) {
        Uint32 frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Draw background
        SDL_SetRenderDrawColor(renderer, 219, 236, 244, 255);
        SDL_RenderClear(renderer);

        // Handle interactions
        env.handleWallCollisions(env.getWallCollisions());
        env.handleParticleCollisions(env.getParticleCollisions());

        // Update and draw particles
        for (auto &particle : env.getParticles()) {
            particle.updatePosition(1);
            drawCircle(renderer, particle);
        }

        SDL_RenderPresent(renderer);

        // Frame duration management
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void synchrony(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Filled Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Environment env = Environment(0, 1, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Event event;
    bool running = true;
    int time = 0;
    int circ_motion_rad = 260;
    double rotation = -0.02;
    double angularVel = 0.01;
    double linearVel;

    // 6 equidistant (angle) unit vectors
    vector<pair<double, double>> directions = {{0, 1}, {-0.5, sqrt(3.0)/2.0}, {-sqrt(3.0)/2.0, 0.5}, {-1, 0},
                                               {-sqrt(3.0)/2, -0.5}, {-0.5, -sqrt(3.0)/2.0}};

    // Time for 1/12 of 2pi rotation
    double time_interval = pi/(6.0 * angularVel);

    // Linear particles velocity
    linearVel = (circ_motion_rad - 40)/(pi/(2*angularVel));

    // Circular motion particles
    env.addParticle(Particle(1, 20, rotation, 0, {400, 300}, {0,0}, {118, 114, 111}, {202, 222, 237}, true));
    env.addParticle(Particle(1, 20, rotation, 0, {400, 300}, {0,0}, {118, 114, 111}, {202, 222, 237}, true));

    while (running) {

        if (time <= pi/angularVel){
            for (int i = 0; i <= directions.size(); ++i){
                if (time == round(time_interval*i) + 5){
                    env.addParticle(Particle(1, 20, rotation, 0, {400, 300}, linearVel*directions[i], {159, 155, 152}, {202, 222, 237}, true));
                    env.getParticles().back().setOrientation(env.getParticles()[0].getOrientation());
                }
            }
        }

        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // Draw Background
        SDL_SetRenderDrawColor(renderer, 219, 236, 244, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 202, 222, 237, 255);
        SDL_RenderFillCircle(renderer, 400, 300, 280);


        // Update circular motion particles positions + draw
        env.getParticles()[0].updatePositionCircular(time, angularVel, circ_motion_rad, {400, 300});
        env.getParticles()[0].updateOrientation(1);
        env.getParticles()[1].updatePositionCircular(time, angularVel, (circ_motion_rad - 40)/2, {400, 300});
        env.getParticles()[1].updateOrientation(1);
        drawCircle(renderer, env.getParticles()[0]);
        drawCircle(renderer, env.getParticles()[1]);
        time += 1;

        // Other particles
        for (auto it = env.getParticles().begin() + 2; it != env.getParticles().end(); ++it){
            it->updatePosition(1);
            it->updateOrientation(1);
            if (it->areColliding(env.getParticles()[0])){ it->cRotate(pi); }
            drawCircle(renderer, *it);
        }

        SDL_RenderPresent(renderer);

        // Frame duration management
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    random();
    return 0;
}

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif
