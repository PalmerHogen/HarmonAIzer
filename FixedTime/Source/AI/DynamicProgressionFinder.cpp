#include "DynamicProgressionFinder.h"

DynamicProgressionFinder::DynamicProgressionFinder() : ProgressionFinder(){}

DynamicProgressionFinder::~DynamicProgressionFinder() {}

tuple<Progression, int> DynamicProgressionFinder::GetSubProgression(Chord start, Chord finish, int pathLength)
{
    if (!SubProgressionTraversed(pathLength, start))
    {
        if(pathLength <= 1)
            CatalogSubProgression(pathLength, start, tuple<Progression, int>(Progression({finish}), start - finish));
        
        else
        {
            int cost = pathLength * Octave;
            Progression path;
            
            for(Chord possibleChord : AllChords)
            {
                Progression chords({start});
                
                int pathCost = start - possibleChord;
                
                if(UsingKeySignature)
                {
                    if(!PrimaryKey.ContainsChord(possibleChord) && !RelativeKey.ContainsChord(possibleChord))
                    {
                        if(ParallelKey.ContainsChord(possibleChord))
                            pathCost += ModalMixturePenalty;
                        else if(PrimaryKey.IsSecondary(possibleChord) || RelativeKey.IsSecondary(possibleChord))
                            pathCost += SecondaryDominantPenalty;
                        else if(ParallelKey.IsSecondary(possibleChord))
                            pathCost += ModalMixturePenalty + SecondaryDominantPenalty;
                        else
                            pathCost += OutOfKeyPenalty;
                    }
                }
                
                auto subPath = GetSubProgression(possibleChord, finish, pathLength - 1);
                
                pathCost += get<1>(subPath);
                auto subPathChords = get<0>(subPath);
                
                for(Chord chord : subPathChords)
                {
                    if(chord == start)
                        pathCost += RepeatedChordPenalty;
                    if(chord.Root == start.Root)
                        pathCost += PedalTonePenalty;
                }
                
                chords.Append(subPathChords);
                
                if(pathCost < cost)
                {
                    cost = pathCost;
                    path = chords;
                }
            }
            CatalogSubProgression(pathLength, start, tuple<Progression, int>(path, cost));
        }
    }
    return SubProgressions[pathLength][start];
}

void DynamicProgressionFinder::CatalogSubProgression(int pathLength, Chord start, tuple<Progression, int> entry)
{
    if(SubProgressions.find(pathLength) == SubProgressions.end())
        SubProgressions[pathLength] = unordered_map<Chord, tuple<Progression, int>>();
    
    SubProgressions[pathLength][start] = entry;
}

bool DynamicProgressionFinder::SubProgressionTraversed(int pathLength, Chord start)
{
    return SubProgressions.find(pathLength) != SubProgressions.end() && SubProgressions[pathLength].find(start) != SubProgressions[pathLength].end();
}

Progression DynamicProgressionFinder::FindChordProgression(Chord startChord, Chord endChord, int numberOfChords)
{
    SubProgressions.clear();
    return get<0>(GetSubProgression(startChord, endChord, numberOfChords));
}
