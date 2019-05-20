#pragma once

#include "../MusicalStructure/Key.h"
#include "../MusicalStructure/Progression.h"

using namespace std;

class ProgressionFinder
{
public:
    // Algorithm Hyperparameters
    int OutOfKeyPenalty = 6;
    int NonChordTonePenalty = 6;
    int SecondaryDominantPenalty = 1;
    int ModalMixturePenalty = 3;
    int PedalTonePenalty  = 0;
    int RepeatedChordPenalty = 0;
    
    // Chord Permissions
    bool UseDominantSevenths = true;
    bool UseMajorSevenths = true;
    bool UseMinorSevenths = true;
    bool UseAugmentedChords = true;
    bool UseDiminishedChords = true;
    bool UseDiminishedSevenths = true;
    bool UseHalfDiminishedSevenths = true;
    
    ProgressionFinder();
    virtual ~ProgressionFinder();
    
    virtual void SetKey(Key key);
    virtual void UnsetKey();
    void GenerateAllChords();
protected:
    bool UsingKeySignature = false;
    Key PrimaryKey, RelativeKey, ParallelKey;
    
    unordered_set<Chord> AllChords;
};
