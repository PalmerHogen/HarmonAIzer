#include "GreedyProgressionFinder.h"

GreedyProgressionFinder::GreedyProgressionFinder(int memorySize) : ProgressionFinder()
{
    MemorySize = memorySize;
}

GreedyProgressionFinder::~GreedyProgressionFinder(){}

void GreedyProgressionFinder::CatalogChord(Chord chord)
{
    Memory.push_back(chord);
    if(Memory.size() > MemorySize)
        Memory.pop_front();
}

Chord GreedyProgressionFinder::FindNextChord(list<Note> noteRestrictions)
{
    int cost = Octave;
    Chord bestChord;
    if(UsingKeySignature)
        bestChord = PrimaryKey.MemberChords.Tonic;
    else
        bestChord = Chord(C, Major);
    for(Chord chord : AllChords)
    {
        int chordCost = 0;
        for(Note note : noteRestrictions)
        {
            if(!chord.Contains(note))
                chordCost += NonChordTonePenalty;
        }
        for(Chord previousChord : Memory)
        {
            if(chord == previousChord)
                chordCost += RepeatedChordPenalty;
            if(chord.Root == previousChord.Root)
                chordCost += PedalTonePenalty;
        }
        
        if(UsingKeySignature)
        {
            if(!PrimaryKey.ContainsChord(chord) && !RelativeKey.ContainsChord(chord))
            {
                if(ParallelKey.ContainsChord(chord))
                    chordCost += ModalMixturePenalty;
                else if(PrimaryKey.IsSecondary(chord) || RelativeKey.IsSecondary(chord))
                    chordCost += SecondaryDominantPenalty;
                else if(ParallelKey.IsSecondary(chord))
                    chordCost += ModalMixturePenalty + SecondaryDominantPenalty;
                else
                    chordCost += OutOfKeyPenalty;
            }
        }
        
        if(chordCost < cost)
        {
            cost = chordCost;
            bestChord = chord;
        }
    }
    return bestChord;
}
