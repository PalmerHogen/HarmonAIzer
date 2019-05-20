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
        
        for(Chord pastChord : StoredProgression)
        {
            if(chord == pastChord)
                chordCost += RepeatedChordPenalty;
            if(chord.Root == pastChord.Root)
                chordCost += PedalTonePenalty;
        }
        
        if(UsingKeySignature)
        {
            if(!PrimaryKey.ContainsChord(chord) && !RelativeKey.ContainsChord(chord))
            {
                if(ParallelKey.ContainsChord(chord))
                    chordCost += ModalMixturePenalty;
                if(PrimaryKey.IsSecondary(chord) || RelativeKey.IsSecondary(chord))
                    chordCost += SecondaryDominantPenalty;
                if(ParallelKey.IsSecondary(chord))
                    chordCost += ModalMixturePenalty + SecondaryDominantPenalty;
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
