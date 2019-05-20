#pragma once


    // Algorithm Hyperparameters
    static int OutOfKeyPenalty = 6;
    static int NonChordTonePenalty = 6;
    static int SecondaryDominantPenalty = 1;
    static int ModalMixturePenalty = 3;
    static int PedalTonePenalty  = 0;
    static int RepeatedChordPenalty = 0;

    // Chord Permissions
    static bool UseDominantSevenths = true;
    static bool UseMajorSevenths = true;
    static bool UseMinorSevenths = true;
    static bool UseAugmentedChords = true;
    static bool UseDiminishedChords = true;
    static bool UseDiminishedSevenths = true;
    static bool UseHalfDiminishedSevenths = true;
