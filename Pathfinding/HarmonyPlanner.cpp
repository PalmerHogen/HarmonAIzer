#include "../MusicalStructure/Note.h"
#include "../MusicalStructure/Chord.h"
#include "../MusicalStructure/Key.h"
#include "HarmonyPlanner.h"
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

using namespace std;

HarmonyPlanner::HarmonyPlanner(){}

HarmonyPlanner::~HarmonyPlanner(){}

tuple<list<Chord>, int> HarmonyPlanner::GetSubPath(Chord start, Chord finish, int pathLength)
{
    if (!SubPathTraversed(pathLength, start))
    {
        if(pathLength <= 1)
        {
            CatalogSubPath(pathLength, start, tuple<list<Chord>, int>(list<Chord>({finish}), start - finish));
        }
        else
        {
            int cost = pathLength * OCTAVE;
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
                            subPathCost += MODAL_MIXTURE_PENALTY;
                        else if(PrimaryKey.IsSecondary(possibleChord) || RelativeKey.IsSecondary(possibleChord))
                            subPathCost += SECONDARY_DOMINANT_PENALTY;
                        else if(ParallelKey.IsSecondary(possibleChord))
                            subPathCost += MODAL_MIXTURE_PENALTY + SECONDARY_DOMINANT_PENALTY;
                        else
                            subPathCost += OUT_OF_KEY_PENALTY;
                    }
                }
                
                for(Chord chord : subPathChords)
                {
                    if(chord == start)
                        subPathCost += REPEATED_CHORD_PENALTY;
                    if(chord.Root == start.Root)
                        subPathCost += REPEATED_ROOT_PENALTY;
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

void HarmonyPlanner::SetKey(Key key)
{
    PrimaryKey = key;
    RelativeKey = PrimaryKey.GetRelativeKey();
    ParallelKey = PrimaryKey.GetParallelKey();
    UsingKeySignature = true;
}

void HarmonyPlanner::UnsetKey()
{
    UsingKeySignature = false;
}

void HarmonyPlanner::CatalogSubPath(int pathLength, Chord start, tuple<list<Chord>, int> entry)
{
    if(Memo.find(pathLength) == Memo.end())
    {
        Memo[pathLength] = unordered_map<Chord, tuple<list<Chord>, int>>();
    }
    Memo[pathLength][start] = entry;
}

bool HarmonyPlanner::SubPathTraversed(int pathLength, Chord start)
{
    return Memo.find(pathLength) != Memo.end() && Memo[pathLength].find(start) != Memo[pathLength].end();
}

list<Chord> HarmonyPlanner::GetChordProgression(Chord start, Chord destination, int length)
{
    Memo.clear();
    GenerateAllChords();
    return get<0>(GetSubPath(start, destination, length));
}

void HarmonyPlanner::GenerateAllChords()
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
