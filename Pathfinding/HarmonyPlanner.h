#ifndef HARMONYPLANNER
#define HARMONYPLANNER

#include "../MusicalStructure/Note.h"
#include "../MusicalStructure/Chord.h"
#include "../MusicalStructure/Key.h"
#include "../Parameters/UserParameters.h"
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <iostream>

using namespace std;

class HarmonyPlanner
{
public:
    HarmonyPlanner();
    ~HarmonyPlanner();
    list<Chord> GetChordProgression(Chord startChord, Chord endChord, int numberOfChords);
    
    void SetKey(Key key);
    void UnsetKey();
private:
    bool UsingKeySignature = false;
    Key PrimaryKey, RelativeKey, ParallelKey;
    
    tuple<list<Chord>, int> GetSubPath(Chord start, Chord finish, int pathLength);
    unordered_set<Chord> AllChords;
    unordered_map<int, unordered_map<Chord, tuple<list<Chord>, int>>> Memo;
    void GenerateAllChords();
    
    void ClearPaths();
    void CatalogSubPath(int pathLength, Chord start, tuple<list<Chord>, int> entry);
    bool SubPathTraversed(int pathLength, Chord start);
};

#endif
