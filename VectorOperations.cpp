#include "VectorOperations.h"
#include <cmath>

//2D vectors
double dotProduct(const std::pair<double, double>& v1, const std::pair<double, double>& v2) {return v1.first*v2.first+v1.second*v2.second;}

double crossProduct(const std::pair<double, double>& v1, const std::pair<double, double>& v2) {return v1.first*v2.second - v1.second*v2.first;}

//vec1 - vec2
std::pair<double, double> operator-(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs) {
    return {lhs.first - rhs.first, lhs.second - rhs.second};
}
//vec1 + vec2
std::pair<double, double> operator+(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs) {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

//scalar*pair
std::pair<double, double> operator*(double scalar, const std::pair<double, double>& vec) {
    return {vec.first * scalar, vec.second * scalar};
}

std::pair<double, double> normalize(const std::pair<double, double>& vec) {
    double x = vec.first;
    double y = vec.second;
    double magnitude = sqrt(x * x + y * y);

    // Avoid division by zero
    if (magnitude == 0.0) {
        return {0.0, 0.0};
    }
    return {x / magnitude, y / magnitude};
}

double getMagnitude(const std::pair<double, double>& vec){ return sqrt(vec.first * vec.first + vec.second * vec.second); }