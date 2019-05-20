#include "Chord.h"
#include <list>
#include "../Parameters/UserParameters.h"

Chord::Chord()
{
    // Default Constructor will always create C Major
    // This should not be used if you want any other chord
    Root = Note(C);
    Quality = Major;
    IsSeventh = false;
    ConstructTriad();
}

Chord::Chord(Note root, ChordQuality quality, bool isSeventh)
{
    Root = root;
    Quality = quality;
    IsSeventh = isSeventh && (quality != Augmented);
    if(IsSeventh)
        ConstructSeventh();
    else
        ConstructTriad();
}

Chord::~Chord(){}

void Chord::ConstructSeventh()
{
    Notes.clear();
    
    Notes.insert(Root);
    
    if(Quality > Minor)
    {
        Notes.insert(Root + MajorThird);
        if(Quality == Augmented)
            Notes.insert(Root + AugmentedFifth);
        else
        {
            Notes.insert(Root + PerfectFifth);
            
            if(Quality == Dominant)
                Notes.insert(Root + MinorSeventh);
            else
                Notes.insert(Root + MajorSeventh);
        }
    }
    else
    {
        Notes.insert(Root + MinorThird);
        if(Quality < Minor)
        {
            Notes.insert(Root + DiminishedFifth);
            if(Quality == Diminished)
                Notes.insert(Root + DiminishedSeventh);
            else
                Notes.insert(Root + MinorSeventh);
        }
        else
        {
            Notes.insert(Root + PerfectFifth);
            Notes.insert(Root + MinorSeventh);
        }
    }
}

void Chord::ConstructTriad()
{
    Notes.clear();
    
    Notes.insert(Root);
    
    if(Quality > Minor)
    {
        Notes.insert(Root + MajorThird);
        if(Quality == Augmented)
            Notes.insert(Root + AugmentedFifth);
        else
        {
            Notes.insert(Root + PerfectFifth);
        }
    }
    else
    {
        Notes.insert(Root + MinorThird);
        if(Quality < Minor)
        {
            Notes.insert(Root + DiminishedFifth);
        }
        else
        {
            Notes.insert(Root + PerfectFifth);
        }
    }
}

int Chord::ExternalDistance(Note note)
{
    if(Contains(note))
    {
        return 0;
    }
    else
    {
        int min = 4;
        for(auto chordTone : Notes)
        {
            int dist = note - chordTone;
            if(dist < min)
            {
                min = dist;
            }
        }
        return min;
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
    switch(Quality)
    {
        case Diminished:
            return Quality == otherChord.Quality && (Root - otherChord.Root) % 3 == 0;
        case Augmented:
            return Quality == otherChord.Quality && (Root - otherChord.Root) % 4 == 0;
        default:
            return Root == otherChord.Root && Quality == otherChord.Quality;
    }
}

int Chord::operator-(Chord otherChord)
{
    // Computes minimal chromatic voice distances between chords
    int distance = 0;
    
    for(Note note : Notes)
    {
        if(otherChord.Contains(note))
            continue;
        
        Interval minDist = AugmentedFourth;
        
        for(Note otherNote : otherChord.Notes)
        {
            if(note - otherNote < minDist)
            {
                minDist = note - otherNote;
            }
        }
        
        distance += (int)minDist;
    }
    return distance;
}

Chord Chord::operator--()
{
    if(Quality == Diminished)
        return *this;
    return Chord(Root, (ChordQuality)(Quality - 1), IsSeventh);
}

Chord Chord::operator++()
{
    if(Quality == Augmented)
        return *this;
    return Chord(Root, (ChordQuality)(Quality + 1), IsSeventh);
}

unordered_set<Chord> Chord::GetChordsContainingAny(unordered_set<Note> notes)
{
    // use this function when chord movement is restricted by notes to avoid searching hundreds of implausible solutions
    static unordered_set<Chord> chords;
    
    chords.clear();
    
    for(Note note : notes)
    {
        chords.insert((Chord(note - 7, Major)));
        chords.insert((Chord(note - 4, Major)));
        chords.insert((Chord(note, Major)));
        
        chords.insert((Chord(note - 7, Minor)));
        chords.insert((Chord(note - 3, Minor)));
        chords.insert((Chord(note, Minor)));
        
        if(UseDominantSevenths)
        {
            chords.insert((Chord(note - 10, Dominant, true)));
            chords.insert((Chord(note - 7, Dominant, true)));
            chords.insert((Chord(note - 4, Dominant, true)));
            chords.insert((Chord(note, Dominant, true)));
        }
        if(UseMajorSevenths)
        {
            chords.insert((Chord(note - 11, Major, true)));
            chords.insert((Chord(note - 7, Major, true)));
            chords.insert((Chord(note - 4, Major, true)));
            chords.insert((Chord(note, Major, true)));
        }
        if(UseMinorSevenths)
        {
            chords.insert((Chord(note - 10, Minor, true)));
            chords.insert((Chord(note - 7, Minor, true)));
            chords.insert((Chord(note - 3, Minor, true)));
            chords.insert((Chord(note, Minor, true)));
        }
        if(UseAugmentedChords)
        {
            chords.insert((Chord(note - 8, Major)));
            chords.insert((Chord(note - 4, Major)));
            chords.insert((Chord(note, Major)));
        }
        if(UseDiminishedChords)
        {
            chords.insert((Chord(note - 6, Diminished)));
            chords.insert((Chord(note - 3, Diminished)));
            chords.insert((Chord(note, Diminished)));
        }
        if(UseDiminishedSevenths)
        {
            chords.insert((Chord(note - 9, Diminished, true)));
            chords.insert((Chord(note - 6, Diminished, true)));
            chords.insert((Chord(note - 3, Diminished, true)));
            chords.insert((Chord(note, Diminished, true)));
        }
        if(UseHalfDiminishedSevenths)
        {
            chords.insert((Chord(note - 10, HalfDiminished, true)));
            chords.insert((Chord(note - 6, HalfDiminished, true)));
            chords.insert((Chord(note - 3, HalfDiminished, true)));
            chords.insert((Chord(note, HalfDiminished, true)));
        }
    }
    
    return chords;
}
