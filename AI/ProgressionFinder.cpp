#include "ProgressionFinder.h"

ProgressionFinder::ProgressionFinder()
{
    GenerateAllChords();
}

ProgressionFinder::~ProgressionFinder(){}
    
void ProgressionFinder::SetKey(Key key)
{
    PrimaryKey = key;
    RelativeKey = PrimaryKey.GetRelativeKey();
    ParallelKey = PrimaryKey.GetParallelKey();
    UsingKeySignature = true;
}

void ProgressionFinder::UnsetKey()
{
    UsingKeySignature = false;
}

void ProgressionFinder::GenerateAllChords()
{
    AllChords.clear();
    for(int noteNumber = C; noteNumber <= B ; noteNumber++)
    {
        for(int quality = Diminished; quality <= Augmented; quality++)
        {
            AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality));
        }
    }
}
