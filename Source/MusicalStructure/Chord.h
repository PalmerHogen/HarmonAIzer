#pragma once

#include "Note.h"
#include <unordered_set>

using namespace std;

enum ChordQuality : int
{
    Diminished = 0,
    HalfDiminished = 1,
    Minor = 2,
    Dominant = 3,
    Major = 4,
    Augmented = 5
};

class Chord
{
public:
    Chord();
    Chord(Note root, ChordQuality quality);
    virtual ~Chord();
    Note Root;
    ChordQuality Quality;
    bool Contains(Note note);
    
    string ToString();
    int GetId() const;
    bool operator==(const Chord otherChord) const;
    int operator-(Chord otherChord);
    Chord operator--();
    Chord operator++();
    
    unordered_set<Note> Notes;
private:
    void ConstructSeventhChord();
};

template<> struct hash<Chord>
{
    size_t operator()(const Chord& chord) const
    {
        return hash<int>()(chord.GetId());
    }
};
