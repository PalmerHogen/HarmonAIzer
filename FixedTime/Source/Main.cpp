/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <stdio.h>
#include <iostream>
#include "MusicalStructure/Chord.h"
#include "MusicalStructure/Key.h"
#include "MusicalStructure/Note.h"
#include "MusicalStructure/Progression.h"
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include "AI/GreedyProgressionFinder.h"

static Key KeyOfExcerpt(A, NaturalMinorKey);
Key DeduceKey(unordered_map<Note, double> noteLengths)
{
    Key maxKey;
    double maxLength = 0.0;
    unordered_map<Key, double> possibleKeys;
    for(auto noteName = (int)C; noteName <= B ; noteName++)
    {
        for(auto keyType = (int)NaturalMinorKey; keyType <= MajorKey; keyType++)
        {
            Key possibleKey = Key(Note(noteName), (KeyType)keyType);
            for(auto pair : noteLengths)
            {
                if(possibleKey.ContainsNote(pair.first))
                {
                    possibleKeys[possibleKey] += pair.second;
                    if(possibleKeys[possibleKey] > maxLength)
                    {
                        maxLength = possibleKeys[possibleKey];
                        maxKey = possibleKey;
                    }
                }
            }
        }
    }
    return maxKey;
}

//==============================================================================
// DYNAMIC PROGRAMMING ALGORITHM

unordered_set<Chord> PrunedChordsContaining(unordered_map<Note, double> thisSimultaneity, unordered_set<Chord>& rawChords)
{
    unordered_set<Note> notes;
    for(auto weightedNote : thisSimultaneity)
        notes.insert(weightedNote.first);
    rawChords = Chord::GetChordsContainingAny(notes);

    unordered_set<Chord> pruned;
    for (Chord chord : rawChords)
    {
        if(KeyOfExcerpt.ContainsChord(chord))
        {
            pruned.insert(chord);
        }
    }
    return pruned;
}

double DissonanceOf(Chord possibility, unordered_map<Note, double> notes, Progression previousChords)
{
    double dissonance = previousChords.Length() > 0 ? previousChords.LastChord() - possibility : 0;
    for(auto note : notes)
    {
        dissonance += 8 * note.second * ((3 - possibility.ExternalDistance(note.first)) % 3);
    }
    
    return dissonance;
}

static unordered_map<long, unordered_map<double, unordered_map<Chord, tuple<double, Progression>>>> Memo;

bool FoundWithinMemo(std::list<unordered_map<Note, double>> noteEvents, tuple<double, Progression> progress)
{
    return
        Memo.find(noteEvents.size()) != Memo.end() &&
        Memo[noteEvents.size()].find(get<0>(progress)) != Memo[noteEvents.size()].end() &&
        get<1>(progress).Length() > 0 &&
        Memo[noteEvents.size()][get<0>(progress)].find(get<1>(progress).LastChord()) != Memo[noteEvents.size()][get<0>(progress)].end();
}

tuple<double, Progression> GetOptimalProgression(std::list<unordered_map<Note, double>> noteEvents, tuple<double, Progression> progress)
{
    if(noteEvents.size() == 0)
    {
        return progress;
    }
    else if (FoundWithinMemo(noteEvents, progress))
    {
        return Memo[noteEvents.size()][get<0>(progress)][get<1>(progress).LastChord()];
    }
    else
    {
        unordered_map<Note, double> thisSimultaneity = *noteEvents.begin();
        unordered_set<Chord> unprunedChords;
        unordered_set<Chord> possibleChords = PrunedChordsContaining(thisSimultaneity, unprunedChords);
        
        if(possibleChords.size() == 0)
            possibleChords = unprunedChords;
        
        Progression progression = std::get<1>(progress);
        
        Chord memoChord = progression.Length() > 0 ?
            progression.LastChord() :
            Chord();
        Chord chordCandidate = memoChord;
        
        double dissonance = 10000000.0;
        
        auto input1 = noteEvents;
        auto input2 = progress;
        auto feedback = progress;
        
        Progression tryProgression = progression;
        Progression outProgression;
        
        for(auto chord : possibleChords)
        {
            double chordDissonance = DissonanceOf(chord, thisSimultaneity, progression);
            
            tryProgression = progression;
            tryProgression.Add(chord);
            input1 = noteEvents;
            input1.pop_front();
            input2 = tuple<double, Progression>(get<0>(progress) + chordDissonance, tryProgression);
            feedback = GetOptimalProgression(input1, input2);
            
            if(get<0>(feedback) <= dissonance)
            {
                dissonance = get<0>(feedback);
                outProgression = get<1>(feedback);
                chordCandidate = chord;
            }
        }
        Memo[input1.size()][dissonance][chordCandidate] = tuple<double, Progression>(dissonance, outProgression);
        return Memo[input1.size()][dissonance][chordCandidate];
    }
}

// ----- MAIN

int main (int argc, char* argv[])
{
    //File inFile = File::createFileWithoutCheckingPath("/Users/palmernhogen/Desktop/risingMotif.mid");
    File inFile = File::createFileWithoutCheckingPath("/Users/palmernhogen/Desktop/TheLampIsLow.mid");
    FileInputStream midiInStream(inFile);
    
    File outFile = File::createFileWithoutCheckingPath("/Users/palmernhogen/Desktop/TestOutputMidi.mid");
    outFile.deleteFile();
    FileOutputStream midiOutStream(outFile);
    
    MidiFile midiInFile;
    midiInFile.readFrom(midiInStream);
    
    double ticksPerQuarterNote = midiInFile.getTimeFormat();
    double WindowLength = ticksPerQuarterNote * 2;
    
    MidiFile midiOutFile = midiInFile;
    
    const MidiMessageSequence *inTrack = midiInFile.getTrack(1);

    MidiMessageSequence midiSequence;
    
    std::list<unordered_map<Note, double>> BarEvents;
    unordered_map<Note, double> noteStarts;
    unordered_map<Note, double> noteLengthsWithinBar;
    unordered_map<Note, double> aggregateNoteLengths;
    
    double nextWindowTime = WindowLength;
    
    // STORE NOTES WITHIN EACH BAR
    for(int i = 0; i < inTrack->getNumEvents(); i++)
    {
        auto message = inTrack->getEventPointer(i)->message;
        
        if(message.getTimeStamp() >= nextWindowTime)
        {
            for(auto noteStart : noteStarts)
            {
                Note note = noteStart.first;
                double startTime = noteStart.second;
                
                noteLengthsWithinBar[note] += (nextWindowTime - startTime) / WindowLength;
                aggregateNoteLengths[note] += (nextWindowTime - startTime) / WindowLength;
                noteStarts[noteStart.first] = nextWindowTime;
            }
            BarEvents.push_back(noteLengthsWithinBar);
            noteLengthsWithinBar.clear();
            nextWindowTime += WindowLength;
        }
        
        if(message.isNoteOff())
        {
            Note note = Note(message.getNoteNumber());
            double normalizedLength = (message.getTimeStamp() - noteStarts[note]) / WindowLength;
            if(normalizedLength > 0.01)
            {
                noteLengthsWithinBar[note] += normalizedLength;
                aggregateNoteLengths[note] += normalizedLength;
            }
            noteStarts.erase(note);
        }
        
        if(message.isNoteOn())
        {
            Note note = Note(message.getNoteNumber());
            noteStarts[note] = message.getTimeStamp();
        }
    }
    if(nextWindowTime <= inTrack->getEndTime())
    {
        for(auto noteStart : noteStarts)
        {
            Note note = noteStart.first;
            double startTime = noteStart.second;
            
            noteLengthsWithinBar[note] += (nextWindowTime - startTime) / WindowLength;
            aggregateNoteLengths[note] += (nextWindowTime - startTime) / WindowLength;
            noteStart.second = nextWindowTime;
        }
        BarEvents.push_back(noteLengthsWithinBar);
        noteLengthsWithinBar.clear();
        nextWindowTime += WindowLength;
    }
    
    //LEAVE THESE LINES
    KeyOfExcerpt = DeduceKey(aggregateNoteLengths);
    std::cout << "Key of Excerpt is " << KeyOfExcerpt.ToString() << endl;
    Progression prog;
    
    //DYNAMIC ALGORITHM-----------------------------
    //auto emptyProgression = Progression();
    //auto chordSequence = GetOptimalProgression(BarEvents, tuple<int, Progression>(0, emptyProgression));
    //prog = get<1>(chordSequence);
    
    //----------------------------------------------
    //GREEDY ALGORITHM------------------------------
    GreedyProgressionFinder ProgressionFinder;
    ProgressionFinder.SetKey(KeyOfExcerpt);
    for(auto barEvent : BarEvents)
    {
        unordered_map<Note, int> bar;
        for(auto notePair : barEvent)
        {
            bar[notePair.first] = notePair.second;
        }
        prog.Add(ProgressionFinder.FindNextChord(prog.Length() > 0 ? prog.LastChord() : Chord(KeyOfExcerpt.Axis, KeyOfExcerpt.Type == MajorKey ? Major : Minor), bar));
    }
    //----------------------------------------------
    nextWindowTime = 0.0;
    for(auto chord : prog)
    {
        std::cout << chord.ToString() << std::endl;
        
        for(Note note : chord.Notes)
        {
            juce::MidiMessage newNoteOn = juce::MidiMessage::noteOn(1, note.GetId() + 60, 0.5f);
            newNoteOn.setTimeStamp(nextWindowTime);
            midiSequence.addEvent(newNoteOn);
        }
        
        nextWindowTime += WindowLength;
        
        for(Note note : chord.Notes)
        {
            juce::MidiMessage newNoteOff = juce::MidiMessage::noteOff(1, note.GetId() + 60, 0.0f);
            newNoteOff.setTimeStamp(nextWindowTime);
            midiSequence.addEvent(newNoteOff);
        }
    }
    
    midiOutFile.addTrack(midiSequence);
    midiOutFile.writeTo(midiOutStream);
    
    return 0;
}
