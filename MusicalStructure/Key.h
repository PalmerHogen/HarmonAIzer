#ifndef KEY
#define KEY

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
    vector<ChordQuality> Qualities;
    vector<Interval> Intervals;
    
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
	unordered_set<Chord> Chords;
    unordered_set<Note> Notes;
};

template<> struct hash<Key>
{
    size_t operator()(const Key& key) const
    {
        return hash<int>()(key.GetId());
    }
};

#endif
