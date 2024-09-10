#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Particle.h"
#include <vector>
#include <string>
#include "VectorOperations.h"

using namespace std;

class Environment{

private:
    vector<Particle> particles;    // Pointers to particles
    double gravity;                // Gravity
    double elasticity;             // Elasticity of collisions
    double width, height;

public:
    // Constructor
    Environment(double gravityValue, double elasticity, double width, double height);

    // Add Particle
    void addParticle(const Particle& particle);

    // Get Particles
    vector<Particle>& getParticles();

    // Check particle Collision, helper
    pair<bool, pair<double, double>>  areColliding(const Particle& particle1, const Particle& particle2) const;

    // Check wall collision, return wall in question
    pair<bool, char> isWallColliding(const Particle& particle) const;

    /* Returns indices of particles having wall collisions
    and Surface of collision, 's' for sides and 'u' for top/bottom */
    vector<pair<int, char>> getWallCollisions() const;

    // Returns indices of pairs of colliding objects
    vector<pair<pair<int, int>, pair<double, double>>> getParticleCollisions() const;

    // Updates velocities of particles colliding with walls
    void handleWallCollisions(vector<pair<int, char>>);

    // Updates velocities of colliding particles
    void handleParticleCollisions(vector<pair<pair<int,int>, pair<double, double>>>);

    //update positions
    void updateEnvironment();
};

#endif // ENVIRONEMNT_H