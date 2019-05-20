#include "GreedyProgressionFinder.h"

GreedyProgressionFinder::GreedyProgressionFinder(int memorySize) : ProgressionFinder(), KeyDetector(memorySize){}

GreedyProgressionFinder::~GreedyProgressionFinder(){}

Chord GreedyProgressionFinder::FindNextChord(Chord lastChord, unordered_map<Note, int> activeNotes)
{
    Chord optimalChord = Chord();
    
    int cost = Octave;
    
    for(Chord chord : AllChords)
    {
        int chordCost = lastChord - chord;
        
        for(auto note : activeNotes)
        {
            if(!chord.Contains(note.first))
                chordCost += NonChordTonePenalty;
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
