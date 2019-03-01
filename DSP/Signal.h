#pragma once

#include "Sample.h"

#include <complex.h>

using namespace std;
typedef complex<double> Complex;

typedef Sample* SignalR;
typedef Complex* SignalC;

namespace Signals
{
    SignalC FromReal(SignalR signal, int length);
    
    SignalR FromComplex(SignalC signal, int length);
    
    SignalC FFT(SignalC signal, int length);
    
    SignalC IFFT(SignalC signal, int length);
    
    SignalC Convolve(SignalC signalA, SignalC signalB, int length);
    
    SignalC CrossCorrelate(SignalR signalA, SignalR signalB, int length);
    
    SignalC AutoCorrelate(SignalR signal, int length);
}

