#ifndef _KEY_
#define _KEY_

#include "Note.h"
#include "Chord.h"
#include <unordered_set>

enum KeyType
{
	MajorKey,
	NaturalMinorKey,
    HarmonicMinorKey
};

class KeyPattern
{
public:
    ChordQuality* QualityArray;
    int* IntervalArray;
    
    KeyPattern(KeyType type = MajorKey);
    ~KeyPattern();
};

struct ChordDescriptors
{
    Chord Tonic;
    Chord SuperTonic;
    Chord Mediant;
    Chord SubDominant;
    Chord Dominant;
    Chord SubMediant;
    Chord SubTonic;
    
    Chord &operator[](int index);
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
    ChordDescriptors MemberChords;
    int GetId() const;
    bool operator==(const Key& otherKey) const;
    bool operator!=(const Key& otherKey) const;
private:
	std::unordered_set<Chord> Chords;
    std::unordered_set<Note> Notes;
};

namespace std
{
    template<> struct hash<Key>
    {
        size_t operator()(const Key& key) const
        {
            return hash<int>()(key.GetId());
        }
    };
}

#endif
