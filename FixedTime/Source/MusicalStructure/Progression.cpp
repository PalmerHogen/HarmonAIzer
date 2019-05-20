#include "Progression.h"

Progression::Progression(){}

Progression::Progression(list<Chord> chords)
{
    Chords.insert(Chords.end(), chords.begin(), chords.end());
}

Progression::~Progression(){}

void Progression::Add(Chord chord)
{
    Chords.push_back(chord);
}

void Progression::AddFront(Chord chord)
{
    Chords.push_front(chord);
}

void Progression::Append(Progression& chordProgression)
{
    Chords.insert(Chords.end(), chordProgression.begin(), chordProgression.end());
}

Chord Progression::PopFirst()
{
    Chord front = Chords.front();
    RemoveFirst();
    return front;
}

Chord Progression::PopLast()
{
    Chord back = Chords.back();
    RemoveLast();
    return back;
}

void Progression::RemoveFirst()
{
    Chords.pop_front();
}

void Progression::RemoveLast()
{
    Chords.pop_back();
}

unsigned long Progression::Length()
{
    return Chords.size();
}

Chord Progression::LastChord()
{
    return Chords.back();
}

__list_iterator<Chord, void*> Progression::begin()
{
    return Chords.begin();
}

__list_iterator<Chord, void*> Progression::end()
{
    return Chords.end();
}
