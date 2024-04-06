#ifndef _Sound_hpp_
#define _Sound_hpp_
#include <M5Unified.h> 

enum class Note {
    // Octave 2
    C2 = 65, Cs2 = 69, D2 = 73, Ds2 = 78, E2 = 82, F2 = 87, Fs2 = 93, G2 = 98, Gs2 = 104, A2 = 110, As2 = 117, B2 = 123,
    // Octave 3
    C3 = 131, Cs3 = 139, D3 = 147, Ds3 = 156, E3 = 165, F3 = 175, Fs3 = 185, G3 = 196, Gs3 = 208, A3 = 220, As3 = 233, B3 = 247,
    // Octave 4
    C4 = 262, Cs4 = 277, D4 = 294, Ds4 = 311, E4 = 330, F4 = 349, Fs4 = 370, G4 = 392, Gs4 = 415, A4 = 440, As4 = 466, B4 = 494,
    // Octave 5
    C5 = 523, Cs5 = 554, D5 = 587, Ds5 = 622, E5 = 659, F5 = 698, Fs5 = 740, G5 = 784, Gs5 = 831, A5 = 880, As5 = 932, B5 = 988,
    // Octave 6
    C6 = 1047, Cs6 = 1109, D6 = 1175, Ds6 = 1245, E6 = 1319, F6 = 1397, Fs6 = 1480, G6 = 1568, Gs6 = 1661, A6 = 1760, As6 = 1865, B6 = 1976,
    // Octave 7
    C7 = 2093, Cs7 = 2217, D7 = 2349, Ds7 = 2489, E7 = 2637, F7 = 2794, Fs7 = 2960, G7 = 3136, Gs7 = 3322, A7 = 3520, As7 = 3729, B7 = 3951,
    // Octave 8
    C8 = 4186
};

class Sound {
    const uint8_t* _data;
    size_t _size;
    int _bitrate;
public:
    template<size_t N>
    Sound(const uint8_t (&data)[N], int bitrate = 44100) {
        _data = data;
        _size = N;
        _bitrate = bitrate;
    }

    bool play(bool waitFinish = false,int repeat = 1) {
        /// The playRaw function can play raw wave data.
        /// 1st argument : data pointer, (supported  int8_t / uint8_t / int16_t)
        /// 2nd argument : Number of data array elements.
        /// 3rd argument : the sampling rate (Hz) (default = 44100)
        /// 4th argument : true=stereo / false=monaural (default = false)
        /// 5th argument : repeat count (default = 1)
        /// 6th argument : virtual channel number (If omitted, use an available channel.)
        bool ret =  M5.Speaker.playRaw( _data, _size / sizeof(_data[0]), _bitrate, false, repeat);        
        if (waitFinish) {
            while (M5.Speaker.isPlaying()) {
                delay(1);
            }
        }
        return ret;
    }
    static void playNote(Note note, int duration = 100) {
        int noteFreq = static_cast<int>(note);
        tone(noteFreq, duration);
        if (duration >= 0) {
            delay(duration);
            stop();
        }
    }
    static void stop() {
        M5.Speaker.stop();
    }

    static void tone(int freq, int duration = -1) {
        M5.Speaker.tone(freq);
        if (duration >= 0){
            delay(duration);
            stop();
        }
    }
    static void errorBeep() {
        tone(950, 500);
        delay(100);
        tone(950, 500);
    }
    static void beep(int freq = 1000, int duration = 100) {
        tone(freq, duration);
    }

    /// The setVolume function can be set the master volume in the range of 0-255. (default : 64)
    static void setVolume(int volume) {
        M5.Speaker.setVolume(volume);
    }
    /// The setAllChannelVolume function can be set the all virtual channel volume in the range of 0-255. (default : 255)
    static void SetAllChannelVolume(int volume) {
        M5.Speaker.setAllChannelVolume(volume);
    }
    /// The setChannelVolume function can be set the specified virtual channel volume in the range of 0-255. (default : 255)
    static void setChannelVolume(int channel, int volume) {
        M5.Speaker.setChannelVolume(channel, volume);
    }

};

#endif