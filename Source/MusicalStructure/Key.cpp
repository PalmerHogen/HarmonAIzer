#include "Key.h"

KeyPattern::KeyPattern(KeyType type)
{
    switch(type)
    {
        case MajorKey:
            Qualities = {Major, Minor, Minor, Major, Dominant, Minor, HalfDiminished};
            Intervals = {Unison, MajorSecond, MajorThird, PerfectFourth, PerfectFifth, MajorSixth, MajorSeventh};
            break;
        case NaturalMinorKey:
            Qualities = {Minor, HalfDiminished, Major, Minor, Minor, Major, Dominant};
            Intervals = {Unison, MajorSecond, MinorThird, PerfectFourth, PerfectFifth, MinorSixth, MinorSeventh};
            break;
        case HarmonicMinorKey:
            Qualities = {Minor, HalfDiminished, Major, Minor, Dominant, Major, Dominant};
            Intervals = {Unison, MajorSecond, MinorThird, PerfectFourth, PerfectFifth, MinorSixth, MinorSeventh};
            break;
    }
}

KeyPattern::~KeyPattern(){}

Chord &ChordDescriptors::operator[](int index)
{
    switch (index % 8)
    {
        case 0:
            return Tonic;
        case 1:
            return SuperTonic;
        case 2:
            return Mediant;
        case 3:
            return SubDominant;
        case 4:
            return Dominant;
        case 5:
            return SubMediant;
        case 6:
            return SubTonic;
        default:
            return Dominant;
    }
}

Key::Key()
{
    // Default Constructor will always construct C Major key,
    // use the other constructor for custom behavior
    Key(Note(C), MajorKey);
}

Key::Key(Note axis, KeyType type)
{
    Axis = axis;
    Type = type;
    
    KeyPattern pattern = KeyPattern(Type);
    
    for(int i = 0; i < 8; i++)
    {
        Note note;
        Notes.insert(note = Axis + pattern.Intervals[i]);
        Chords.insert(MemberChords[i] = Chord(note, pattern.Qualities[i]));
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
        return ContainsNote(chord.Root + PerfectFourth);
    else if (chord.Quality == Diminished)
        return ContainsNote(chord.Root + MinorSecond);
    else
        return false;
}

Key Key::GetRelativeKey()
{
    return Type == MajorKey ? Key(Axis - MinorThird, HarmonicMinorKey) : Key(Axis + MinorThird, MajorKey);
}

Key Key::GetParallelKey()
{
    return Type == MajorKey ? Key(Axis, NaturalMinorKey) : Key(Axis, MajorKey);
}

int Key::GetId() const
{
    return 13*(int)Type + 97*Axis.GetId();
}

string Key::ToString()
{
    switch(Type)
    {
        case NaturalMinorKey:
            return Axis.ToString() + " Natural Minor";
        case HarmonicMinorKey:
            return Axis.ToString() + " Harmonic Minor";
        case MajorKey:
            return Axis.ToString() + " Major";
    }
}

bool Key::operator==(const Key& otherKey) const
{
    return Type == otherKey.Type && Axis == otherKey.Axis;
}

bool Key::operator!=(const Key& otherKey) const
{
    return !operator==(otherKey);
}
