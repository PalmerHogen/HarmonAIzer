#pragma once

#include "../Parameters/UserParameters.h"
#include "ProgressionFinder.h"
#include <list>
#include <unordered_map>
#include <tuple>

using namespace std;

class DynamicProgressionFinder : public ProgressionFinder
{
public:
    DynamicProgressionFinder();
    ~DynamicProgressionFinder();
    Progression FindChordProgression(Chord startChord, Chord endChord, int numberOfChords);
    
private:
    tuple<Progression, int> GetSubProgression(Chord start, Chord finish, int pathLength);
    unordered_map<int, unordered_map<Chord, tuple<Progression, int>>> SubProgressions;
    
    void CatalogSubProgression(int progressionLength, Chord start, tuple<Progression, int> entry);
    bool SubProgressionTraversed(int progressionLength, Chord start);
};
