#include "DynamicProgressionFinder.h"

DynamicProgressionFinder::DynamicProgressionFinder() : ProgressionFinder(){}

DynamicProgressionFinder::~DynamicProgressionFinder() {}

tuple<list<Chord>, int> DynamicProgressionFinder::GetSubPath(Chord start, Chord finish, int pathLength)
{
    if (!SubPathTraversed(pathLength, start))
    {
        if(pathLength <= 1)
        {
            CatalogSubPath(pathLength, start, tuple<list<Chord>, int>(list<Chord>({finish}), start - finish));
        }
        else
        {
            int cost = pathLength * Octave;
            list<Chord> path;
            
            for(Chord possibleChord : AllChords)
            {
                list<Chord> chords({start});
                
                int immediateCost = start - possibleChord;
                auto subPath = GetSubPath(possibleChord, finish, pathLength - 1);
                int subPathCost = get<1>(subPath);
                auto subPathChords = get<0>(subPath);
                
                if(UsingKeySignature)
                {
                    if(!PrimaryKey.ContainsChord(possibleChord) && !RelativeKey.ContainsChord(possibleChord))
                    {
                        if(ParallelKey.ContainsChord(possibleChord))
                            subPathCost += ModalMixturePenalty;
                        else if(PrimaryKey.IsSecondary(possibleChord) || RelativeKey.IsSecondary(possibleChord))
                            subPathCost += SecondaryDominantPenalty;
                        else if(ParallelKey.IsSecondary(possibleChord))
                            subPathCost += ModalMixturePenalty + SecondaryDominantPenalty;
                        else
                            subPathCost += OutOfKeyPenalty;
                    }
                }
                
                for(Chord chord : subPathChords)
                {
                    if(chord == start)
                        subPathCost += RepeatedChordPenalty;
                    if(chord.Root == start.Root)
                        subPathCost += PedalTonePenalty;
                }
                
                chords.insert(chords.end(), subPathChords.begin(), subPathChords.end());
                
                if(immediateCost + subPathCost < cost)
                {
                    cost = immediateCost + subPathCost;
                    path = chords;
                }
            }
            CatalogSubPath(pathLength, start, tuple<list<Chord>, int>(path, cost));
        }
    }
    return Memo[pathLength][start];
}

void DynamicProgressionFinder::CatalogSubPath(int pathLength, Chord start, tuple<list<Chord>, int> entry)
{
    if(Memo.find(pathLength) == Memo.end())
    {
        Memo[pathLength] = unordered_map<Chord, tuple<list<Chord>, int>>();
    }
    Memo[pathLength][start] = entry;
}

bool DynamicProgressionFinder::SubPathTraversed(int pathLength, Chord start)
{
    return Memo.find(pathLength) != Memo.end() && Memo[pathLength].find(start) != Memo[pathLength].end();
}

list<Chord> DynamicProgressionFinder::FindChordProgression(Chord start, Chord destination, int length)
{
    Memo.clear();
    return get<0>(GetSubPath(start, destination, length));
}
