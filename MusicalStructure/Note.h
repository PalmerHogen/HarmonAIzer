#pragma once

#include <string>
#include <unordered_set>

using namespace std;

enum NoteName : int
{
	C = 0,
	D = 2,
	E = 4,
	F = 5,
	G = 7,
    A = 9,
    B = 11
};

enum NoteModifier : int
{
	Natural = 0,
	Sharp = 1,
	Flat = -1
};

enum Interval : int
{
    Unison = 0,
    MinorSecond = 1,
    MajorSecond = 2,
    MinorThird = 3,
    MajorThird = 4,
    PerfectFourth = 5,
    AugmentedFourth = 6,
    DiminishedFifth = 6,
    PerfectFifth = 7,
    AugmentedFifth = 8,
    MinorSixth = 8,
    MajorSixth = 9,
    DiminishedSeventh = 9,
    MinorSeventh = 10,
    MajorSeventh = 11,
    Octave = 12
};

class Note
{
public:
    Note();
	Note(NoteName name, NoteModifier modifier = Natural);
	Note(int noteNumber);
	virtual ~Note();
    
    string ToString();
    
    bool operator==(const Note otherNote) const;
    bool operator!=(const Note otherNote) const;
    Interval operator-(const Note otherNote) const;
    Note operator+(const Interval interval) const;
    Note operator-(const Interval interval) const;
    int GetId() const;
private:
    void Normalize();
	int BaseNoteNumber;
};

template<> struct hash<Note>
{
    size_t operator()(const Note& note) const
    {
        return hash<int>()(note.GetId());
    }
};
