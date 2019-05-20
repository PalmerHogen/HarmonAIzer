#include "ProgressionFinder.h"

ProgressionFinder::ProgressionFinder()
{
    GenerateAllChords();
}

ProgressionFinder::~ProgressionFinder(){}
    
void ProgressionFinder::SetKey(Key key)
{
    PrimaryKey = Key(key.Axis, key.Type);
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
            switch(quality)
            {
                case Diminished:
                {
                    if(UseDiminishedChords)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality));
                    if(UseDiminishedSevenths)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality, true));
                    break;
                }
                case HalfDiminished:
                {
                    if(UseHalfDiminishedSevenths)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality, true));
                    break;
                }
                case Minor:
                {
                    if(UseMinorSevenths)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality, true));
                    AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality));
                    break;
                }
                case Dominant:
                {
                    if(UseDominantSevenths)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality, true));
                    break;
                }
                case Major:
                {
                    if(UseMajorSevenths)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality, true));
                    AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality));
                    break;
                }
                case Augmented:
                {
                    if(UseAugmentedChords)
                        AllChords.insert(Chord(Note(noteNumber), (ChordQuality)quality));
                    break;
                }
            }
        }
    }
}
