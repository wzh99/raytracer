#pragma once

#include <glm/glm.hpp>
#include <vector>
using namespace glm;
using namespace std;

class Sampler {
public:
    Sampler (int nspl, int nset) : numSamples(nspl), numSets(nset) {}
    dvec2 sampleUnitSquare();
    dvec2 sampleUnitDisk();
    inline int getNumSamples() { return numSamples; }
    virtual ~Sampler() {}

protected:
    int numSamples;
    int numSets;
    vector<dvec2> samples;
    vector<dvec2> diskSamples;
    vector<int> shuffledIndices;
    unsigned long count;
    int jump;

    virtual void generateSamples() = 0;
    void setupShuffledIndices();
    void mapSamplesToUnitDisk();
};
