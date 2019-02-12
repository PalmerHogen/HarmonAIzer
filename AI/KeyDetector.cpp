#include "KeyDetector.h"
#include "../Parameters/UserParameters.h"
#include <unordered_map>

KeyDetector::KeyDetector(int memorySize)
{
    Updated = false;
    MemorySize = memorySize;
}

KeyDetector::~KeyDetector(){}

void KeyDetector::CatalogChord(Chord chord)
{
    Memory.push_back(chord);
    if(Memory.size() > MemorySize)
        Memory.pop_front();
    Updated = false;
}

Key KeyDetector::DetectKey()
{
    if(!Updated)
    {
        QualifyKey();
        Updated = true;
    }
    return MostLikelyKey;
}

void KeyDetector::QualifyKey()
{
    Key detectedKey;
    int maxWeight = 0;
    unordered_map<Key, int> KeyWeights;
    for(Chord chord : Memory)
    {
        for(int keyType = NaturalMinorKey; keyType <= MajorKey; keyType++)
        {
            KeyPattern pattern((KeyType) keyType);
            
            for(int scaleDegree = 0; scaleDegree < 8; scaleDegree++)
            {
                if(chord.Quality != pattern.Qualities[scaleDegree])
                    continue;
                
                Key possibleKey = Key(chord.Root - pattern.Intervals[scaleDegree], (KeyType)keyType);
                
                if(++KeyWeights[possibleKey] > maxWeight)
                {
                    detectedKey = possibleKey;
                    maxWeight = KeyWeights[possibleKey];
                }
            }
        }
    }
    MostLikelyKey = detectedKey;
}
