#ifndef CHORD
#define CHORD
#include "Note.h"
#include <stdlib.h>
#include <unordered_set>

// In lieu of implementing an interval class,
// I've defined these macros for readability
// An interval class would have just been a pointless wrapper for integers
#define OCTAVE 12
#define MAJOR_SEVENTH 11
#define MINOR_SEVENTH 10
#define MAJOR_SIXTH 9
#define MINOR_SIXTH 8
#define PERFECT_FIFTH 7
#define DIMINISHED_FIFTH 6
#define AUGMENTED_FOURTH 6
#define PERFECT_FOURTH 5
#define MAJOR_THIRD 4
#define MINOR_THIRD 3
#define MAJOR_SECOND 2
#define MINOR_SECOND 1
#define UNISON 0

enum ChordQuality
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
    Chord(); // Default Constructor Should Not Be Used
    Chord(Note root, ChordQuality quality);
    ~Chord();
    Note Root;
    ChordQuality Quality;
    bool Contains(Note note);
    
    std::string ToString();
    int GetId() const;
    bool operator==(const Chord otherChord) const;
    int operator-(Chord otherChord);
    Chord operator--();
    Chord operator++();
    
    std::unordered_set<Note> Notes;
private:
    void ConstructSeventhChord();
};

namespace std
{
    template<> struct hash<Chord>
    {
        size_t operator()(const Chord& chord) const
        {
            return hash<int>()(chord.GetId());
        }
    };
}

#endif
