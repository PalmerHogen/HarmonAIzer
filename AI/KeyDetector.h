#ifndef KEYDETECTOR
#define KEYDETECTOR

#include "../MusicalStructure/Key.h"
#include <list>

using namespace std;

class KeyDetector
{
public:
    KeyDetector(int memorySize = 4);
    ~KeyDetector();
    void CatalogChord(Chord chord);
    Key DetectKey();
protected:
    bool Updated;
    Key MostLikelyKey;
    unordered_set<Key> PossibleKeys;
    int MemorySize;
    list<Chord> Memory;
    
    void QualifyKeys();
    void DisqualifyKeys();
};

#endif
