#include <stdio.h>

/*! Enumeration of MIDI types */
enum MidiType
{
    InvalidType           = 0x00,    ///< For notifying errors
    NoteOff               = 0x80,    ///< Channel Message - Note Off
    NoteOn                = 0x90,    ///< Channel Message - Note On
    AfterTouchPoly        = 0xA0,    ///< Channel Message - Polyphonic AfterTouch
    ControlChange         = 0xB0,    ///< Channel Message - Control Change / Channel Mode
    ProgramChange         = 0xC0,    ///< Channel Message - Program Change
    AfterTouchChannel     = 0xD0,    ///< Channel Message - Channel (monophonic) AfterTouch
    PitchBend             = 0xE0,    ///< Channel Message - Pitch Bend
    SystemExclusive       = 0xF0,    ///< System Exclusive
	SystemExclusiveStart  = SystemExclusive,   ///< System Exclusive Start
    TimeCodeQuarterFrame  = 0xF1,    ///< System Common - MIDI Time Code Quarter Frame
    SongPosition          = 0xF2,    ///< System Common - Song Position Pointer
    SongSelect            = 0xF3,    ///< System Common - Song Select
    Undefined_F4          = 0xF4,
    Undefined_F5          = 0xF5,
    TuneRequest           = 0xF6,    ///< System Common - Tune Request
	SystemExclusiveEnd    = 0xF7,    ///< System Exclusive End
    Clock                 = 0xF8,    ///< System Real Time - Timing Clock
    Undefined_F9          = 0xF9,
    Tick                  = Undefined_F9, ///< System Real Time - Timing Tick (1 tick = 10 milliseconds)
    Start                 = 0xFA,    ///< System Real Time - Start
    Continue              = 0xFB,    ///< System Real Time - Continue
    Stop                  = 0xFC,    ///< System Real Time - Stop
    Undefined_FD          = 0xFD,
    ActiveSensing         = 0xFE,    ///< System Real Time - Active Sensing
    SystemReset           = 0xFF,    ///< System Real Time - System Reset
};

unsigned char test_midi_arr[] = {
    0x90, 0x3C, 0x40,
    0xF8,
    0xf0,
    0x90, 0x3D, 0x40,
    0xf7,
    0x90, 0x3E, 0xFA, 0x40,
    0x90, 0x3E, 0x41,
    0xC0, 0xf6, 0x01,
    0x90, 0x3D, 0x40
};

// State of the dispatcher - it controlls further workflow and processing the rest of MIDI messages 
typedef enum {
    Undefined,
    SysEx,
    SysExEnd, // the last byte of SysEx msg
    TwoDataBytes, // e.g. NOTE ON/OFF, CC
    OneDataByte, // e.g. PrgChange, Song Select
}
StatusDispatcherState;

// some state is always returned
StatusDispatcherState setState(unsigned char byte) {
    static StatusDispatcherState state;

    // is it status byte?
    if ( (byte >> 7) == 0x01 ) {

        // start SysEx mode
        if ( (byte == SystemExclusive) && (state != SysEx) ) {
            state = SysEx;
            return state;
        }

        // if state == SysEx do nothing except of the last SysEx byte
        if (state == SysEx) {
            if (byte == SystemExclusiveEnd) {
                state = SysExEnd;
            }
            return state;
        }

        // SysExEnd should take for only one byte. This state should never happen.
        if (state == SysExEnd) {
            state = Undefined;
            return state;
        }

        unsigned char msb_nybble = byte & 0xF0;

        // two data bytes state
        if ( (msb_nybble == NoteOff) ||
                    (msb_nybble == NoteOn) ||
                    (msb_nybble == AfterTouchPoly) ||
                    (msb_nybble == ControlChange) ||
                    (msb_nybble == PitchBend) ||
                    (msb_nybble == SongPosition) ) {
                        state = TwoDataBytes;
                        return state;
                    }
        // one data bytes state
        else if ( (msb_nybble == ProgramChange) ||
                    (msb_nybble == AfterTouchChannel) ||
                    (byte == TimeCodeQuarterFrame) ||
                    (byte == SongSelect) ) {
                        state = OneDataByte;
                        return state;
                    }
    }

    // the rest of messages doesn't change the state - we return the last one
    return state;
}

void status_dispatcher(unsigned char byte) {
    static unsigned char full_msg[3]; // container for full MIDI messages
    static int msg_byte_idx; // index for writing full messages

    static StatusDispatcherState state;
    StatusDispatcherState new_state = setState(byte);

    // every state change resets write idx for full messages
    if (state != new_state) {
        state = new_state;
        msg_byte_idx = 0;
    }

    // SysEx
    if ( (state == SysEx) || (state == SysExEnd) ) {
        printf("Handling SysEx: %02x\n", byte);
        return;
    }

    // anywhere in the message there can be a special byte (e.g. syystem realtime) that is send immediatelly
    if ( (byte == Undefined_F4) ||
            (byte == Undefined_F5) ||
            (byte == TuneRequest) ||
            (byte == Clock) ||
            (byte == Undefined_F9) ||
            (byte == Tick) ||
            (byte == Start) ||
            (byte == Continue) ||
            (byte == Stop) ||
            (byte == Undefined_FD) ||
            (byte == ActiveSensing) ||
            (byte == SystemReset) ) {
        printf("Handling system realtime: %02x\n", byte);
        return;
    }

    // handling full messages - 1. status byte, 2. data1, (3. data2)
    if (state == OneDataByte) {
        full_msg[msg_byte_idx] = byte;
        msg_byte_idx++;
        if (msg_byte_idx >= 2) {
            msg_byte_idx = 0;
            printf("Have 2B msg: ");
            printf("%02x", full_msg[0]);
            printf("%02x", full_msg[1]);
            printf("\n");
            return;
        }
    }
    if (state == TwoDataBytes) {
        full_msg[msg_byte_idx] = byte;
        msg_byte_idx++;
        if (msg_byte_idx >= 3) {
            msg_byte_idx = 0;
            printf("Have 3B msg: ");
            printf("%02x", full_msg[0]);
            printf("%02x", full_msg[1]);
            printf("%02x", full_msg[2]);
            printf("\n");
            return;
        }
    }
}

int main()
{
 
    for (int i=0; i<sizeof(test_midi_arr); i++) {
        status_dispatcher(test_midi_arr[i]);
    }

 
    return 0;
}