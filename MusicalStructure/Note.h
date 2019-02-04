#ifndef NOTE
#define NOTE

#include <string>
#include <stdlib.h>
#include <unordered_set>

enum NoteName
{
	C = 0,
	D = 2,
	E = 4,
	F = 5,
	G = 7,
    A = 9,
    B = 11
};

enum NoteModifier
{
	Natural = 0,
	Sharp = 1,
	Flat = -1
};

class Note
{
public:
    Note();
	Note(NoteName name, NoteModifier modifier);
	Note(int noteNumber);
	~Note();
    
    std::string ToString();
    
    bool operator==(const Note otherNote) const;
    bool operator!=(const Note otherNote) const;
    int operator-(const Note otherNote) const;
    Note operator+(const int interval) const;
    Note operator-(const int interval) const;
    int GetId() const;
private:
    void Normalize();
	int BaseNoteNumber;
};

namespace std
{
    template<> struct hash<Note>
    {
        size_t operator()(const Note& note) const
        {
            return hash<int>()(note.GetId());
        }
    };
}

#endif
