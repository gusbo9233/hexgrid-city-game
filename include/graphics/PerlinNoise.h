#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

class PerlinNoise {
private:
    std::vector<int> p;

public:
    // Constructor with random seed
    PerlinNoise() : PerlinNoise(std::random_device{}()) {}

    // Constructor with specified seed
    PerlinNoise(unsigned int seed) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);
        p.insert(p.end(), p.begin(), p.end());
    }

    // 2D Perlin noise
    float noise(float x, float y) const {
        // Find unit cube that contains point
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;

        // Find relative x, y of point in cube
        x -= std::floor(x);
        y -= std::floor(y);

        // Compute fade curves for each of x, y
        float u = fade(x);
        float v = fade(y);

        // Hash coordinates of the 4 cube corners
        int A = p[X] + Y;
        int B = p[X + 1] + Y;

        // And add blended results from 4 corners of cube
        return lerp(v, 
                    lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)), 
                    lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1)));
    }

private:
    static float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float lerp(float t, float a, float b) {
        return a + t * (b - a);
    }

    static float grad(int hash, float x, float y) {
        // Convert low 4 bits of hash code into 12 gradient directions
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};

#endif // PERLIN_NOISE_H 