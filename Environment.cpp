#include "Environment.h"
#include <cmath>
#include "VectorOperations.h"
#include <iostream>
constexpr double pi = 3.14159265358979323846;

Environment::Environment(double gravityValue, double elasticity, double width, double height)
    : gravity(gravityValue), elasticity(elasticity) , width(width), height(height){}

void Environment::addParticle(const Particle& particle) { particles.push_back(particle); }
vector<Particle>& Environment::getParticles(){return particles;}

pair<bool, pair<double, double>> Environment::areColliding(const Particle& particle1, const Particle& particle2) const {
    const double r1 = particle1.getRadius();
    const double r2 = particle2.getRadius();
    const pair<double, double> pos1 = particle1.getPos();
    const pair<double, double> pos2 = particle2.getPos();
    double x1 = pos1.first;
    double x2 = pos2.first;
    double y1 = pos1.second;
    double y2 = pos2.second;

    bool areColliding = pow(r1+r2, 2) >= pow(x1-x2, 2) + pow(y1-y2, 2);
    if (areColliding){
        pair<double, double> collision_point = {(x1*r2+x2*r1)/(r1+r2), (y1*r2+y2*r1)/(r1+r2)};
        return {true, collision_point};
    }
    return {false, {0,0}};
}

pair<bool, char> Environment::isWallColliding(const Particle& particle) const {
    pair<double, double> pos = particle.getPos();
    double x = pos.first;
    double y = pos.second;
    double r = particle.getRadius();

    if (x - r <= 1){return {true, 'l'};}                         //left
    else if (x + r >= width - 1){return {true, 'r'};}            //right
    else if (y - r <= 1){return {true, 't'};}                    //top
    else if (y + r >= height - 1){return {true, 's'};}           //sol
    else {return {false, 'n'};}
}

vector<pair<int, char>> Environment::getWallCollisions() const {
    vector<pair<int, char>> wallCollisions;
    for (int i = 0; i < particles.size(); ++i){
        pair<bool, char> result = isWallColliding(particles[i]);
        if (result.first){wallCollisions.push_back({i, result.second});}
    }
    return wallCollisions;
}

// Returns indices of pairs of colliding particles
vector<pair<pair<int, int>, pair<double, double>>> Environment::getParticleCollisions() const {
    vector<pair<pair<int, int>, pair<double, double>>> collidingPairs;
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = i+1; j <= particles.size(); ++j) {
            auto collision = areColliding(particles[i], particles[j]);
            if (collision.first) {
                collidingPairs.push_back({{i,j}, collision.second});
            }
        }
    }
    return collidingPairs;
}

void Environment::handleWallCollisions(vector<pair<int, char>> wallCollisions){
    for (auto pair : wallCollisions){
        Particle& p = particles[pair.first];
        std::pair<double, double> velocity = p.getVel();
        double vx = velocity.first;
        double vy = velocity.second;
        char wall = pair.second;

        uint8_t direction = ((vx>=0) << 1 | (vy>=0));        //00 = 0 -> x:-, y:, 01 = 1-> x:-, y:+, 10 = 2 -> x:+, y+-, 11=3 ->x:+, y:+
            switch (wall){
                case 'l':                                                //left
                    switch (direction){
                        case 0: p.cRotate(2*atan2(vx, vy)); break;
                        case 1: p.cRotate(2*atan2(vx, vy)); break;
                    }
                break;
                case 'r':                                                //right
                    switch (direction){
                        case 2: p.cRotate(2*atan2(vx, vy)); break;
                        case 3: p.cRotate(2*atan2(vx, vy)); break;
                    }
                break;
                case 't':
                    switch (direction){                                  //top
                        case 0: p.ccRotate(2*atan2(vy, vx)); break;
                        case 2: p.ccRotate(2*atan2(vy, vx)); break;
                    }
                break;
                case 's':
                    switch (direction){                                  //sol
                        case 1: p.ccRotate(2*atan2(vy, vx)); break;
                        case 3: p.ccRotate(2*atan2(vy, vx)); break;
                    }
                break;
            }
    }
}

// Updates velocities of colliding particles
void Environment::handleParticleCollisions(vector<pair<pair<int, int>, pair<double, double>>> collidingPairs) {
    for (const pair<pair<int, int>, pair<double, double>> & pair : collidingPairs) {
        Particle& p1 = particles[pair.first.first];
        Particle& p2 = particles[pair.first.second];

        std::pair<double, double> collision_point = pair.second;
        std::pair<double, double> N = normalize(collision_point - p1.getPos());  //unit vector normal to collision

        double p1_inertia_moment = p1.getMass()*pow(p1.getRadius(), 2)/2;
        double p2_inertia_moment = p2.getMass()*pow(p2.getRadius(), 2)/2;
        std::pair<double, double> v_rel = p2.getVel() - p1.getVel();

        double impulse = dotProduct((-1-elasticity)*v_rel, N)/((1/p1.getMass()) + (1/p2.getMass()) +
                         (pow(crossProduct(collision_point - p1.getPos(), N), 2)/p1_inertia_moment) +
                         (pow(crossProduct(collision_point - p2.getPos(), N), 2)/p2_inertia_moment));
        p1.setVel(p1.getVel() - (impulse/p1.getMass())*N);
        p2.setVel(p2.getVel() + (impulse/p2.getMass())*N);
    }
}


