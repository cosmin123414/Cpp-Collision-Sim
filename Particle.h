#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>
using namespace std;

class Particle {

private:
    double mass;                    // Mass of the particle
    double radius;                  // Radius of the particle
    double  angular_vel;            // Angular velocity
    double orientation;             // Orientation
    pair<double, double> pos;       // Position of the particle in 2D space
    pair<double, double> vel;       // Velocity of the particle in 2D space
    pair<double, double> acc;       // Acceleration of particle in 2D space
    vector<int> color1;             // main color
    vector<int> color2;             // secondary color
    bool inert;                     // unaffected by other particles

public:
    // Constructor
    Particle(double m, double r, double angular_vel, double orientation, pair<double, double> pos, pair<double, double> vel,
             vector<int> color1, vector<int> color2, bool inert);

    // Getters and setters
    double getMass() const;
    void setMass(double m);

    double getRadius() const;
    void setRadius(double r);

    double getOrientation() const;
    void setOrientation(double new_orientation);

    double getAngularVelocity() const;
    void setAngularVelocity(double new_angular_velocity);

    pair<double, double> getPos() const;
    void setPos(pair<double, double>);

    pair<double, double> getVel() const;
    void setVel(pair<double, double>);

    void setAcc(pair<double, double>);

    vector<int> getColor1() const;
    vector<int> getColor2() const;
    void setColor1(vector<int> color);
    void setColor2(vector<int> color);

    bool isInert() const;

    // Manually move particles
    void translateX(int pixels);
    void translateY(int pixels);

    // Rotates velocity vector theta deg. clockwise
    void cRotate(double theta);

    // Rotates velocity vector theta deg. counter-clockwise
    void ccRotate(double theta);

    // Update state
    void updatePosition(double deltaTime);
    void updateOrientation(double deltaTime);
    void updateVelocity(double deltaTime);

    // Circular motion
    void updatePositionCircular(double time, double angularVel, double radius, pair<int, int> initial_pos);

    //Check collision
    bool areColliding(Particle p);

};



#endif // PARTICLE_H