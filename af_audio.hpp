#pragma once
#include "include/SDL_audio.h"

#define AF_AUDIO_BUFFER_SIZE 8192
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

typedef struct {
} afSquareSynth;

extern afAudioRing afAudioBuffer;



bool afAudioInit(const SDL_AudioSpec *want, SDL_AudioSpec *have);

extern SDL_AudioDeviceID afDevice;
