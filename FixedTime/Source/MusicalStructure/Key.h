#pragma once

#include "Note.h"
#include "Chord.h"
#include <vector>

using namespace std;

enum KeyType : int
{
    NaturalMinorKey = 0,
    HarmonicMinorKey = 1,
	MajorKey = 2
};

class KeyPattern
{
public:
    vector<Interval> Intervals;
    
    KeyPattern(KeyType type = MajorKey);
    ~KeyPattern();
};

class Key
{
public:
    Key();
	Key(Note axis, KeyType type);
	~Key();
	bool ContainsChord(Chord chord);
    bool ContainsNote(Note note);
	bool IsSecondary(Chord chord);
	Key GetRelativeKey();
	Key GetParallelKey();
    Note Axis;
    KeyType Type;
    string ToString();
    int GetId() const;
    bool operator==(const Key& otherKey) const;
    bool operator!=(const Key& otherKey) const;
private:
    unordered_set<Note> Notes;
};

template<> struct hash<Key>
{
    size_t operator()(const Key& key) const
    {
        return hash<int>()(key.GetId());
    }
};
