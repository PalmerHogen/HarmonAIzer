#include "Key.h"

KeyPattern::KeyPattern(KeyType type)
{
    switch(type)
    {
        case MajorKey:
            Intervals = {Unison, MajorSecond, MajorThird, PerfectFourth, PerfectFifth, MajorSixth, MajorSeventh};
            break;
        case NaturalMinorKey:
            Intervals = {Unison, MajorSecond, MinorThird, PerfectFourth, PerfectFifth, MinorSixth, MinorSeventh};
            break;
        case HarmonicMinorKey:
            Intervals = {Unison, MajorSecond, MinorThird, PerfectFourth, PerfectFifth, MinorSixth, MinorSeventh};
            break;
    }
}

KeyPattern::~KeyPattern(){}

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
    }
}

Key::~Key()
{
    
}

bool Key::ContainsChord(Chord chord)
{
    bool contained = true;
    for(Note note : chord.Notes)
    {
        contained &= ContainsNote(note);
    }
    return contained;
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
