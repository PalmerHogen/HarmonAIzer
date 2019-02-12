#include "MusicalStructure/Key.h"
#include "AI/DynamicProgressionFinder.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
    // Dynamic Chord Progression Finder Test
    Chord startingChord = Chord(C, Major);
    Chord endingChord = Chord(A, Minor);
    
    DynamicProgressionFinder planner;
    planner.SetKey(Key(C, MajorKey));
    
    list<Chord> progression = planner.FindChordProgression(startingChord, endingChord, 4);
    
    for(Chord chord : progression)
        cout << chord.ToString() << endl;
    
    return 0;
}
