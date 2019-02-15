#include "GreedyProgressionFinder.h"

GreedyProgressionFinder::GreedyProgressionFinder(int memorySize) : ProgressionFinder(), KeyDetector(memorySize){}

GreedyProgressionFinder::~GreedyProgressionFinder(){}

Chord GreedyProgressionFinder::FindNextChord(list<Note> noteRestrictions)
{
    Chord optimalChord = UsingKeySignature ? PrimaryKey.MemberChords.Tonic : Chord();
    
    if(StoredProgression.Length() == 0)
        return optimalChord;
    
    int cost = Octave;
    
    for(Chord chord : AllChords)
    {
        int chordCost = StoredProgression.LastChord() - chord;
        
        for(Note note : noteRestrictions)
        {
            if(!chord.Contains(note))
                chordCost += NonChordTonePenalty;
        }
        
        for(Chord previousChord : StoredProgression)
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
            optimalChord = chord;
        }
    }
    return optimalChord;
}
