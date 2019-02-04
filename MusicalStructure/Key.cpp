#include "Note.h"
#include "Chord.h"
#include <unordered_set>
#include "Key.h"

KeyPattern::KeyPattern(KeyType type)
{
    switch(type)
    {
        case MajorKey:
            QualityArray = new ChordQuality[7]{Major, Minor, Minor, Major, Dominant, Minor, HalfDiminished};
            IntervalArray = new int[7]{0, 2, 4, 5, 7, 9, 11};
            break;
        case NaturalMinorKey:
            QualityArray = new ChordQuality[7]{Minor, HalfDiminished, Major, Minor, Minor, Major, Dominant};
            IntervalArray = new int[7]{0, 2, 3, 5, 7, 8, 10};
            break;
        case HarmonicMinorKey:
            QualityArray = new ChordQuality[7]{Minor, HalfDiminished, Major, Minor, Dominant, Major, Dominant};
            IntervalArray = new int[7]{0, 2, 3, 5, 7, 8, 10};
            break;
    }
}

KeyPattern::~KeyPattern()
{
    delete QualityArray;
    delete IntervalArray;
}

Chord &ChordDescriptors::operator[](int index)
{
    switch (index % 8)
    {
        case 1:
            return Tonic;
        case 2:
            return SuperTonic;
        case 3:
            return Mediant;
        case 4:
            return SubDominant;
        case 5:
            return Dominant;
        case 6:
            return SubMediant;
        case 7:
            return SubTonic;
        default:
            return Dominant;
    }
}

Key::Key()
{
    // Default Constructor will always construct C Major key,
    // use a different constructor for custom behavior
    Axis = Note(C);
    Type = MajorKey;
    
    KeyPattern pattern = KeyPattern(Type);
    
    for(int i = 0; i < 8; i++)
    {
        Note note;
        Notes.insert(note = Axis + pattern.IntervalArray[i]);
        Chords.insert(MemberChords[i] = Chord(note, pattern.QualityArray[i]));
    }
}

Key::Key(Note axis, KeyType type)
{
    Axis = axis;
    Type = type;
    
    KeyPattern pattern = KeyPattern(Type);
    
    for(int i = 0; i < 8; i++)
    {
        Note note;
        Notes.insert(note = Axis + pattern.IntervalArray[i]);
        Chords.insert(MemberChords[i] = Chord(note, pattern.QualityArray[i]));
    }
}

Key::~Key()
{
    
}

bool Key::ContainsChord(Chord chord)
{
    return Chords.find(chord) != Chords.end();
}

bool Key::ContainsNote(Note note)
{
    return Notes.find(note) != Notes.end();
}

bool Key::IsSecondary(Chord chord)
{
    if(chord.Quality == Dominant)
        return ContainsNote(chord.Root + 5);
    else if (chord.Quality == Diminished)
        return ContainsNote(chord.Root + 1);
    else
        return false;
}

Key Key::GetRelativeKey()
{
    return Type == MajorKey ? Key(Axis - 3, HarmonicMinorKey) : Key(Axis + 3, MajorKey);
}

Key Key::GetParallelKey()
{
    return Type == MajorKey ? Key(Axis, NaturalMinorKey) : Key(Axis, MajorKey);
}

int Key::GetId() const
{
    return 13*(int)Type + 97*Axis.GetId();
}

bool Key::operator==(const Key& otherKey) const
{
    return Type == otherKey.Type && Axis == otherKey.Axis;
}

bool Key::operator!=(const Key& otherKey) const
{
    return !operator==(otherKey);
}
