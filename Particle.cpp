#include "Particle.h"
#include "VectorOperations.h"
#include <cmath>
#include <iostream>

// Constructor
Particle::Particle(double m, double r, double angular_vel, double orientation, pair<double, double> pos, pair<double, double> vel,
                   vector<int> color1, vector<int> color2, bool inert)
    : mass(m), radius(r), angular_vel(angular_vel), orientation(orientation), pos(pos), vel(vel), color1(color1), color2(color2), acc({0,0}), inert(inert) {}

// Getters
double Particle::getMass() const { return mass; }
double Particle::getRadius() const { return radius; }
pair<double, double> Particle::getPos() const { return pos; }
pair<double, double> Particle::getVel() const { return vel; }
vector<int> Particle::getColor1() const { return color1; }
vector<int> Particle::getColor2() const { return color2; }
double Particle::getOrientation() const { return orientation; }
double Particle::getAngularVelocity() const { return angular_vel; }
bool Particle::isInert() const { return inert; }

// Setters
void Particle::setMass(double m) { mass = m; }
void Particle::setRadius(double r) { radius = r; }
void Particle::setPos(pair<double, double> new_pos) { pos = new_pos; }
void Particle::setVel(pair<double, double> new_vel) { vel = new_vel; }
void Particle::setColor1(vector<int> color){ color1 = color; }
void Particle::setColor2(vector<int> color){ color2 = color; }
void Particle::setOrientation(double new_orientation){ orientation = new_orientation; }
void Particle::setAngularVelocity(double new_angular_vel){ angular_vel = new_angular_vel; }
void Particle::setAcc(pair<double, double> new_acc) { acc = new_acc; }

// Rotate velocity vectors
void Particle::ccRotate(double theta){
    double vx = vel.first;
    double vy = vel.second;
    double temp = vx;
    vel = {vx*cos(theta) + vy*sin(theta), vy = -temp*sin(theta) + vy*cos(theta)};
}
void Particle::cRotate(double theta){
    double vx = vel.first;
    double vy = vel.second;
    double temp = vx;
    vel = {temp*cos(theta) - vy*sin(theta), temp*sin(theta) + vy*cos(theta)};
}

// Update particle state
void Particle::updatePosition(double deltaTime) { pos = pos + deltaTime*vel; }
void Particle::updateOrientation(double deltaTime){ orientation = orientation + deltaTime*angular_vel; }
void Particle::updateVelocity(double deltaTime) { vel = vel + deltaTime*acc; }

// Manually induced circular motion
void Particle::updatePositionCircular(double time, double angularVel, double radius, pair<int, int> initial_pos){
    pos = {radius * cos(angularVel * time) + initial_pos.first, radius * sin(angularVel * time) + initial_pos.second};
}

//check collision
bool Particle::areColliding(Particle p){
    return pow(radius + p.getRadius(), 2) + 120 >= pow(pos.first - p.getPos().first, 2) + pow(pos.second - p.getPos().second, 2);
}