// vector_operations.h
#define VECTOR_OPERATIONS_H
#include <vector>

double dotProduct(const std::pair<double, double>& v1, const std::pair<double, double>& v2);
double crossProduct(const std::pair<double, double>& v1, const std::pair<double, double>& v2);

std::pair<double, double> operator+(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator-(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator*(double scalar, const std::pair<double, double>& vec);
std::pair<double, double> normalize(const std::pair<double, double>& vec);
double getMagnitude(const std::pair<double, double>& vec);


