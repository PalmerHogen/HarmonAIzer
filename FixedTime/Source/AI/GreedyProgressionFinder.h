#pragma once

#include "ProgressionFinder.h"
#include "KeyDetector.h"
#include "../Parameters/UserParameters.h"
#include <unordered_map>

using namespace std;

class GreedyProgressionFinder : public ProgressionFinder, public KeyDetector
{
public:
    GreedyProgressionFinder(int memorySize = 4);
    ~GreedyProgressionFinder();
    Chord FindNextChord(Chord lastChord, unordered_map<Note, int> activeNotes);
private:
};
