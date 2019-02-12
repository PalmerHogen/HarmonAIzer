#include "MusicalStructure/Key.h"
#include "AI/DynamicProgressionFinder.h"
#include "AI/GreedyProgressionFinder.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
    Chord startingChord = Chord(C, Major);
    Chord endingChord = Chord(A, Minor);
    Key CMajor = Key(C, MajorKey);
    
    int progressionLength = 8;
    
    cout << endl << "Dynamic Progression Finder:" << endl;
    DynamicProgressionFinder dynamicProgressionFinder;
    dynamicProgressionFinder.SetKey(CMajor);
    list<Chord> dynamicProgression = dynamicProgressionFinder.FindChordProgression(startingChord, endingChord, progressionLength);
    for(Chord chord : dynamicProgression)
        cout << chord.ToString() << endl;
    
    
    cout << endl << "Greedy Progression Finder:" << endl;
    GreedyProgressionFinder greedyProgressionFinder;
    greedyProgressionFinder.SetKey(CMajor);
    list<Chord> greedyProgression;
    for(int i = 0; i < progressionLength; i++)
    {
        Chord chord = greedyProgressionFinder.FindNextChord();
        greedyProgression.push_back(chord);
        greedyProgressionFinder.CatalogChord(chord);
        cout << chord.ToString() << endl;
    }
    
    
    cout << endl << "Greedy Progression Finder with Continuously-updating Key Center:" << endl;
    GreedyProgressionFinder greedyExplorer;
    greedyExplorer.SetKey(CMajor);
    list<Chord> explorativeProgression;
    for(int i = 0; i < progressionLength; i++)
    {
        Chord chord = greedyExplorer.FindNextChord();
        explorativeProgression.push_back(chord);
        greedyExplorer.CatalogChord(chord);
        greedyExplorer.SetKey(greedyExplorer.DetectKey());
        cout << chord.ToString() << endl;
    }
    
    return 0;
}
