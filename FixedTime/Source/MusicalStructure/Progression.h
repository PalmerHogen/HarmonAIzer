#pragma once

#include "Chord.h"
#include <list>

using namespace std;

class Progression
{
public:
    Progression();
    Progression(list<Chord> chords);
    virtual ~Progression();
    
    virtual __list_iterator<Chord, void*> begin();
    virtual __list_iterator<Chord, void*> end();
    void Add(Chord chord);
    void AddFront(Chord chord);
    void Append(Progression& chordProgression);
    Chord PopFirst();
    Chord PopLast();
    void RemoveFirst();
    void RemoveLast();
    unsigned long Length();
    Chord LastChord();
protected:
    list<Chord> Chords;
};
