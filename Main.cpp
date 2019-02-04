#include "MusicalStructure/Note.h"
#include "MusicalStructure/Chord.h"
#include "MusicalStructure/Key.h"
#include "Pathfinding/HarmonyPlanner.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
    // Sample Program
    Chord startingChord = Chord(C, Major);
    Chord endingChord = Chord(G, Dominant);
    
    HarmonyPlanner planner;
    planner.SetKey(Key(C, MajorKey));
    
    list<Chord> progression = planner.GetChordProgression(startingChord, endingChord, 4);
    
    for(Chord chord : progression)
        cerr << chord.ToString() << endl;
    
    return 0;
}
