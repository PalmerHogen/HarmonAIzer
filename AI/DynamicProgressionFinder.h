#ifndef DYNAMICPROGRESSIONFINDER
#define DYNAMICPROGRESSIONFINDER

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
    list<Chord> FindChordProgression(Chord startChord, Chord endChord, int numberOfChords);
    
private:
    tuple<list<Chord>, int> GetSubPath(Chord start, Chord finish, int pathLength);
    unordered_map<int, unordered_map<Chord, tuple<list<Chord>, int>>> Memo;
    
    void ClearPaths();
    void CatalogSubPath(int pathLength, Chord start, tuple<list<Chord>, int> entry);
    bool SubPathTraversed(int pathLength, Chord start);
};

#endif
