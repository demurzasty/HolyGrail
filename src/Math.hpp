#pragma once 

struct Math {
    static constexpr float Epsilon = 0.00001f;

    static constexpr float PI = 3.1415926535897932384626433833f;

    static constexpr float TAU = 6.2831853071795864769252867666f;

    static float RadiansToDegrees(float radians);

    static float DegreesToRadians(float degrees);

    static bool IsPowerOfTwo(int x);

    static unsigned int NextPowerOfTwo(unsigned int v);

    static int Abs(int value);

    static float Abs(float value);

    static int Max(int a, int b);

    static float Max(float a, float b);

    static int Min(int a, int b);

    static float Min(float a, float b);

    static int Clamp(int v, int a, int b);

    static float Clamp(float v, float a, float b);

    static bool AlmostEquals(float a, float b);

    static int IntegerSqrt(int v);

    static float Sqrt(float v);

    static float Sin(float v);

    static float Cos(float v);

    static float Asin(float v);

    static float Atan2(float a, float b);

    static float Pow(float a, float b);

    static float Log(float v);

    static unsigned int Align(unsigned int size, unsigned int align);
};
