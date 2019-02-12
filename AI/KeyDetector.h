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
    int MemorySize;
    list<Chord> Memory;
    
    void QualifyKey();
};

#endif
