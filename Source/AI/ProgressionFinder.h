#pragma once

#include "../MusicalStructure/Key.h"
#include "../MusicalStructure/Progression.h"

using namespace std;

class ProgressionFinder
{
public:
    ProgressionFinder();
    virtual ~ProgressionFinder();
    
    virtual void SetKey(Key key);
    virtual void UnsetKey();
protected:
    bool UsingKeySignature = false;
    Key PrimaryKey, RelativeKey, ParallelKey;
    
    unordered_set<Chord> AllChords;
    void GenerateAllChords();
};
