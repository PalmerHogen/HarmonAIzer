#ifndef GREEDYPROGRESSIONFINDER
#define GREEDYPROGRESSIONFINDER

#include "ProgressionFinder.h"
#include "KeyDetector.h"
#include "../Parameters/UserParameters.h"
#include <list>

using namespace std;

class GreedyProgressionFinder : public ProgressionFinder, public KeyDetector
{
public:
    GreedyProgressionFinder(int memorySize = 4);
    ~GreedyProgressionFinder();
    Chord FindNextChord(list<Note> noteRestrictions = {});
private:
};

#endif
