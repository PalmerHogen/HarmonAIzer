#include "Note.h"
#include "Chord.h"
#include <list>
#include <iostream>

Chord::Chord()
{
    // Default Constructor will always create C Major
    // This should not be used if you want any other chord
    Root = Note(C);
    Quality = Major;
    ConstructSeventhChord();
}

Chord::Chord(Note root, ChordQuality quality)
{
    Root = root;
    Quality = quality;
    ConstructSeventhChord();
}

Chord::~Chord(){}

void Chord::ConstructSeventhChord()
{
    Notes.clear();
    
    Notes.insert(Root);
    
    if(Quality > Minor)
    {
        Notes.insert(Root + 4);
        if(Quality == Augmented)
            Notes.insert(Root + 8);
        else
        {
            Notes.insert(Root + 7);
            
            if(Quality == Dominant)
                Notes.insert(Root + 10);
            else
                Notes.insert(Root + 11);
        }
    }
    else
    {
        Notes.insert(Root + 3);
        if(Quality < Minor)
        {
            Notes.insert(Root + 6);
            if(Quality == Diminished)
                Notes.insert(Root + 9);
            else
                Notes.insert(Root + 10);
        }
        else
        {
            Notes.insert(Root + 7);
            Notes.insert(Root + 10);
        }
    }
}

bool Chord::Contains(Note note)
{
    return Notes.find(note) != Notes.end();
}

std::string Chord::ToString()
{
    switch(Quality)
    {
        case Diminished:
            return Root.ToString() + " Diminished";
        case HalfDiminished:
            return Root.ToString() + " Half-Diminished";
        case Minor:
            return Root.ToString() + " Minor";
        case Dominant:
            return Root.ToString() + " Dominant";
        case Major:
            return Root.ToString() + " Major";
        case Augmented:
            return Root.ToString() + " Augmented";
    }
}

int Chord::GetId() const
{
    // Used for Hashing
    return (int)Quality + (int)Augmented + Root.GetId();
}

bool Chord::operator==(const Chord otherChord) const
{
    if(Quality == Diminished)
    {
        return Quality == otherChord.Quality && (Root - otherChord.Root) % 3 == 0;
    }
    else if (Quality == Augmented)
    {
        return Quality == otherChord.Quality && (Root - otherChord.Root) % 4 == 0;
    }
    return Root == otherChord.Root && Quality == otherChord.Quality;
}

int Chord::operator-(Chord otherChord)
{
    // Computes minimal chromatic voice distances between chords
    int distance = 0;
    
    for(Note note : Notes)
    {
        if(otherChord.Contains(note))
            continue;
        
        int minDist = AUGMENTED_FOURTH;
        
        for(Note otherNote : otherChord.Notes)
        {
            if(note - otherNote < minDist)
            {
                minDist = note - otherNote;
            }
        }
        
        distance += minDist;
    }
    return distance;
}

Chord Chord::operator--()
{
    if(Quality == Diminished)
        return *this;
    return Chord(Root, (ChordQuality)(Quality - 1));
}

Chord Chord::operator++()
{
    if(Quality == Augmented)
        return *this;
    return Chord(Root, (ChordQuality)(Quality + 1));
}
