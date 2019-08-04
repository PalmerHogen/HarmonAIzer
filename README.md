# Intelligent Harmonizer
Project Harmonizes Input signals or MIDI notes from a Common Practice Period Perspective

## How does it work?

1. Input Pitch is Determined; for MIDI input this is trivial, but for signals this can prove difficult. For pitch detection systems, accuracy and latency are strongly correlated; after trying several algorithms, the two strongest contenders appear to be Fast Lifting Haar Wavelets, and the Yin algorithm.

[Paper on Wavelet Pitch Tracking](https://pdfs.semanticscholar.org/1ecf/ae4b3618f92b4267912afbc59e3a3ea1d846.pdf)<br/>
[Paper on Yin Pitch Tracking](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf)

2. Once a pitch is deduced, a chord is recommended on the basis of:<br/>
  • Minimizing dissonance with the detected pitch; if the chord contains the pitch, this is ideal<br/>
  • Minimizing voice-movement from the surrounding chords that were recommended; using the same chord or a new chord with many common tones is ideal<br/>
  • Minimizing deviance from a perceived or prescribed key center (optional, though recommended for generating tonal music)<br/>
  • Minimizing tense intervals or densely-packed chord voicings (several optional parameters; this is especially useless if the goal is jazz harmony)<br/>

3. After a chord is recommended, a smooth voicing (w.r.t. the surrounding chords) is determined via Modular Arithmetic and user parameters

4. MIDI events are generated from the voicing, and flow as output from the program

## Two Main Folders exist for this project:

1. A Real-Time capable VST plugin, which does not implement any beat detection or chord-lookahead

2. A Fixed-time shell program, which uses a more intelligent Dynamic Programming algorithm to factor in future chord choices, and can place chords on particular beats

### A third folder contains Max/MSP patches that show how the plugin may be used within a DAW, and some early prototype attempts that instead use a markov model to generate harmony on the fly.
