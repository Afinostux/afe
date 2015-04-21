#pragma once
#include "include/SDL_audio.h"

#define AF_AUDIO_BUFFER_SIZE 8192
typedef unsigned long int uli;
typedef struct {
	float buffer[AF_AUDIO_BUFFER_SIZE];
	unsigned int
		readpt,
		writept,
		laps,
		samples,
		freq,
		channels;
	unsigned long int 
		audiotime;
} afAudioRing;

typedef struct afSquareSynth {
	unsigned long int
		startTime,
		endTime;
	float
		hertz,
		volume;
	afSquareSynth():
		startTime(0),
		endTime(0),
		hertz(0),
		volume(0)
	{}
	afSquareSynth(uli start, uli end, float hz, float vol):
		startTime(start),
		endTime(end),
		hertz(hz),
		volume(vol)
	{}
	void addOutput(float *buffer,
			int len,
			unsigned long int start);
} afSquareSynth;

extern const uint afSquareMax;
extern uint afSquareCount;

afSquareSynth* afNewSynthSquare(float delay, float duration, float hertz, float volume);

extern afAudioRing afAudioBuffer;


unsigned long int afGetWriteTime(void);

void afPushAudio(float frametime);

bool afAudioInit(const SDL_AudioSpec *want, SDL_AudioSpec *have);

extern SDL_AudioDeviceID afDevice;
