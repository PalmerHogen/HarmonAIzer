#include "KeyDetector.h"
#include "../Parameters/UserParameters.h"

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
        QualifyKeys();
        DisqualifyKeys();
        Updated = true;
    }
    return MostLikelyKey;
}

void KeyDetector::QualifyKeys()
{
    KeyPattern patterns[3] = {KeyPattern(NaturalMinorKey), KeyPattern(HarmonicMinorKey), KeyPattern(MajorKey)};
    
    for(Chord chord : Memory)
    {
        for(int keyType = NaturalMinorKey; keyType <= MajorKey; keyType++)
        {
            for(int scaleDegree = 0; scaleDegree < 8; scaleDegree++)
            {
                ChordQuality quality = patterns[keyType].Qualities[scaleDegree];
                Interval interval = patterns[keyType].Intervals[scaleDegree];
                
                Key possibleKey = Key(chord.Root - interval, (KeyType)keyType);
                
                if(chord.Quality == quality)
                    PossibleKeys.insert(possibleKey);
            }
        }
    }
}

void KeyDetector::DisqualifyKeys()
{
    int minimumDeviance = Octave;
    for(Key key : PossibleKeys)
    {
        int keyDeviance = 0;
        
        for(Chord chord : Memory)
        {
            if(!key.ContainsChord(chord) && !key.GetRelativeKey().ContainsChord(chord))
            {
                if(key.IsSecondary(chord))
                    keyDeviance += SecondaryDominantPenalty;
                else if(key.GetParallelKey().ContainsChord(chord))
                    keyDeviance += ModalMixturePenalty;
                else if(key.GetParallelKey().IsSecondary(chord))
                    keyDeviance += ModalMixturePenalty + SecondaryDominantPenalty;
                else
                    keyDeviance += OutOfKeyPenalty;
            }
        }
        
        if(keyDeviance < minimumDeviance)
        {
            minimumDeviance = keyDeviance;
            MostLikelyKey = key;
        }
    }
    PossibleKeys.clear();
}
