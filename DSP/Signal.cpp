#include "Signal.h"

SignalC Signals::FromReal(SignalR signal, int length)
{
    SignalC out = new Complex[length];
    for(int n = 0; n < length; n++)
        out[n] = signal[n];
    return out;
}

SignalR Signals::FromComplex(SignalC signal, int length)
{
    SignalR out = new Sample[length];
    for(int n = 0; n < length; n++)
        out[n] = signal[n].real();
    return out;
}

void Separate(SignalC signal, int length)
{
    int midPoint = length >> 1;
    Complex oddData[midPoint];
    for (int i = 0; i < midPoint; i++)
        oddData[i] = signal[2*i+1];
    for(int i = 0; i < midPoint; i++)
        signal[i] = signal[2*i];
    for(int i = 0; i < midPoint; i++)
        signal[i+midPoint] = oddData[i];
}

void FFTRecurse(SignalC signal, int length, int direction)
{
    if(length >= 2)
    {
        Separate(signal, length);
        FFTRecurse(signal, length / 2, direction);
        FFTRecurse(signal + (length / 2), length / 2, direction);
        for (int k = 0; k < (length / 2); k++)
        {
            Complex even = signal[k];
            Complex odd = signal[k + (length / 2)];
            Complex w = exp(Complex(0, -direction * 2.0 * M_PI * k / length));
            signal[k] = even + w * odd;
            signal[k + (length / 2)] = even - w * odd;
        }
    }
}

void FFTNormalize(SignalC signal, double length)
{
    for(int n = 0; n < length; n++)
        signal[n] /= length;
}

SignalC Signals::FFT(SignalC signal, int length)
{
    FFTRecurse(signal, length, 1);
    return signal;
}

SignalC Signals::IFFT(SignalC signal, int length)
{
    FFTRecurse(signal, length, -1);
    FFTNormalize(signal, length);
    return signal;
}

SignalC Signals::CrossCorrelate(SignalR signalA, SignalR signalB, int length)
{
    SignalC aComplex, bComplex;
    aComplex = FromReal(signalA, length);
    bComplex = FromReal(signalB, length);
    
    FFT(aComplex, length);
    FFT(bComplex, length);
    
    for(int n = 0; n < length; n++)
        aComplex[n] = aComplex[n].real() * bComplex[n].real() + aComplex[n].imag() * bComplex[n].imag();
    
    delete bComplex;
    return IFFT(aComplex, length);
}

SignalC Signals::AutoCorrelate(SignalR signal, int length)
{
    return CrossCorrelate(signal, signal, length);
}
