#include "Math.hpp"

#include <math.h>

float Math::RadiansToDegrees(float radians) {
    return radians * 180.0f / PI;
}

float Math::DegreesToRadians(float degrees) {
    return degrees * PI / 180.0f;
}

bool Math::IsPowerOfTwo(int value) {
    return !(value == 0) && !(value & (value - 1));
}

unsigned int Math::NextPowerOfTwo(unsigned int value) {
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return ++value;
}

int Math::Abs(int value) {
    return value < 0 ? -value : value;
}

float Math::Abs(float value) {
    return value < 0 ? -value : value;
}

int Math::Max(int a, int b) {
    return (a > b) ? a : b;
}

float Math::Max(float a, float b) {
    return (a > b) ? a : b;
}

int Math::Min(int a, int b) {
    return (a < b) ? a : b;
}

float Math::Min(float a, float b) {
    return (a < b) ? a : b;
}

int Math::Clamp(int v, int a, int b) {
    return Max(Min(v, b), a);
}

float Math::Clamp(float v, float a, float b) {
    return Max(Min(v, b), a);
}

bool Math::AlmostEquals(float a, float b) {
    return Abs(a - b) < Epsilon;
}

int Math::IntegerSqrt(int v) {
    int l = 0;
    int m;
    int r = v + 1;

    while (l != r - 1) {
        m = (l + r) / 2;

        if (m * m <= v) {
            l = m;
        }
        else {
            r = m;
        }
    }

    return l;
}

float Math::Sqrt(float v) {
    return sqrtf(v);
}

float Math::Sin(float v) {
    return sinf(v);
}

float Math::Cos(float v) {
    return cosf(v);
}

float Math::Asin(float v) {
    return asinf(v);
}

float Math::Atan2(float a, float b) {
    return atan2f(a, b);
}

float Math::Pow(float a, float b) {
    return powf(a, b);
}

float Math::Log(float v) {
    return logf(v);
}

unsigned int Math::Align(unsigned int size, unsigned int align) {
    return (size + align - 1) & ~(align - 1);
}
