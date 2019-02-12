#ifndef PROGRESSIONFINDER
#define PROGRESSIONFINDER

#include "../MusicalStructure/Key.h"

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

#endif
