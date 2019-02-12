#include "Note.h"

Note::Note()
{
    BaseNoteNumber = 0;
}

Note::Note(NoteName name, NoteModifier modifier = Natural)
{
	switch(name)
	{
		case(A):
            BaseNoteNumber = 9;
			break;
		case(B):
            BaseNoteNumber = 11;
			break;
		case(C):
            BaseNoteNumber = 0;
			break;
		case(D):
            BaseNoteNumber = 2;
			break;
		case(E):
            BaseNoteNumber = 4;
			break;
		case(F):
            BaseNoteNumber = 5;
			break;
		case(G):
            BaseNoteNumber = 7;
			break;
	}
	switch(modifier)
	{
		case(Natural):
			break;
		case(Sharp):
            BaseNoteNumber++;
			break;
		case(Flat):
            BaseNoteNumber--;
			break;
	}
    Normalize();
}

Note::Note(int noteNumber)
{
    BaseNoteNumber = noteNumber;
    Normalize();
}

Note::~Note(){}

void Note::Normalize()
{
    // Ensures that base note numbers are always in the first octave
    while(BaseNoteNumber < 0)
        BaseNoteNumber += 12;
    BaseNoteNumber %= 12;
}

std::string Note::ToString()
{
    switch(BaseNoteNumber)
    {
        case 1:
            return "C#";
        case 2:
            return "D";
        case 3:
            return "Eb";
        case 4:
            return "E";
        case 5:
            return "F";
        case 6:
            return "F#";
        case 7:
            return "G";
        case 8:
            return "Ab";
        case 9:
            return "A";
        case 10:
            return "Bb";
        case 11:
            return "B";
        default:
            return "C";
    }
}

bool Note::operator==(const Note otherNote) const
{
    return BaseNoteNumber == otherNote.BaseNoteNumber;
}

bool Note::operator!=(const Note otherNote) const
{
    return BaseNoteNumber != otherNote.BaseNoteNumber;
}

Interval Note::operator-( const Note otherNote) const
{
    // Returns the minimum chromatic difference between two notes
    Interval downInterval  = (Interval)((BaseNoteNumber + 12 - otherNote.BaseNoteNumber) % 12);
    Interval upInterval = (Interval)((otherNote.BaseNoteNumber + 12 - BaseNoteNumber) % 12);
    return downInterval < upInterval ? (Interval)downInterval : (Interval)upInterval;
}

Note Note::operator+(const Interval interval) const
{
    // Returns a note that is [interval] steps above the note
    return Note(BaseNoteNumber + interval);
}

Note Note::operator-(const Interval interval) const
{
    // Returns a note that is [interval] steps below the note
    return Note(BaseNoteNumber - interval);
}

int Note::GetId() const
{
    // used for hashing
    return BaseNoteNumber;
}
