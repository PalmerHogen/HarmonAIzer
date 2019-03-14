#pragma once

#include "ProgressionFinder.h"
#include "KeyDetector.h"
#include "../Parameters/UserParameters.h"

using namespace std;

class GreedyProgressionFinder : public ProgressionFinder, public KeyDetector
{
public:
    GreedyProgressionFinder(int memorySize = 4);
    ~GreedyProgressionFinder();
    Chord FindNextChord(unordered_set<Note> noteRestrictions = {});
private:
};
