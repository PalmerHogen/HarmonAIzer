#ifndef GREEDYPROGRESSIONFINDER
#define GREEDYPROGRESSIONFINDER

#include "ProgressionFinder.h"
#include "../Parameters/UserParameters.h"
#include <list>

using namespace std;

class GreedyProgressionFinder : public ProgressionFinder
{
public:
    GreedyProgressionFinder(int memorySize);
    ~GreedyProgressionFinder();
    void CatalogChord(Chord chord);
    Chord FindNextChord(list<Note> noteRestrictions);
private:
    int MemorySize;
    list<Chord> Memory;
};

#endif
