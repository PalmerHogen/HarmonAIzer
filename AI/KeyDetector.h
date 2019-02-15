#pragma once

#include "../MusicalStructure/Key.h"
#include "../MusicalStructure/Progression.h"
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
    Progression StoredProgression;
    
    void QualifyKey();
};
