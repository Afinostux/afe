#include <stdio.h>
#include "af_audio.hpp"

SDL_AudioDeviceID afDevice = 0;
afAudioRing afAudioBuffer = {};
//
//	square synth functions
//


const uint afSquareMax = 16;
uint afSquareCount = 0;
afSquareSynth afASS[afSquareMax];
afSquareSynth* afNewSynthSquare(float delay, float duration, float hertz, float volume)
{
	uint afloc = 0;
	uli start = afAudioBuffer.audiotime + afAudioBuffer.freq * delay,
	    end = start + afAudioBuffer.freq * duration;
	if (afSquareCount == afSquareMax) return 0;
	if (afSquareCount)
	{
		// keep in sorted order, more recently discarded ones at the back
		uint current = afSquareCount++;
		while (afASS[current].endTime < end && current > 0) current--;
		for (uint i = afSquareCount; --i > current;)
		{
			afASS[i] = afASS[i - 1];
		}
		afASS[current] = afSquareSynth(start, end, hertz, volume);
		afloc = current;
	}
	else
	{
		afloc = afSquareCount;
		afASS[afSquareCount++] = afSquareSynth(start, end, hertz, volume);
	}
	return &afASS[afloc];
}

//
//	audio buffer functions
//
float afAudioRingRead(void)
{
	afAudioBuffer.laps |= (afAudioBuffer.readpt == afAudioBuffer.writept);
	float rv = afAudioBuffer.buffer[afAudioBuffer.readpt++];
	afAudioBuffer.readpt %= AF_AUDIO_BUFFER_SIZE;
	return rv;
}

void afAudioRingWrite(float f)
{
	afAudioBuffer.buffer[afAudioBuffer.writept++] = f;
	afAudioBuffer.laps &= !(afAudioBuffer.readpt == afAudioBuffer.writept);
	afAudioBuffer.writept %= AF_AUDIO_BUFFER_SIZE;
}

unsigned long int afGetWriteTime(void)
{
	unsigned int wpoffset = AF_AUDIO_BUFFER_SIZE - afAudioBuffer.writept;
	unsigned long int result = (afAudioBuffer.audiotime + wpoffset)%AF_AUDIO_BUFFER_SIZE;
	result = AF_AUDIO_BUFFER_SIZE - result;
	return result + afAudioBuffer.audiotime;
}

float audioscale = 1;
const float audioceiling = 0.2f;

void afAudioCallback(void *udata, Uint8 *stream, int len)
{
	float raw;
	float rawsign;
	float *fstream = (float*)stream;
	int flen = len/sizeof(float);
	for (int i = 0; i < flen; i++)
	{
		raw = afAudioRingRead();
		rawsign = (raw < 0)?-1.f:1.f;
		raw *= rawsign;
		audioscale = (raw > audioceiling)?
			1/(raw/audioceiling):
			audioscale + (1-audioscale)/10;
		fstream[i] = raw*rawsign*audioscale;
		afAudioBuffer.audiotime++;
	}

//	if (afSquareCount)
//	{
//		for (int i = 0; i < flen; i++) {
//			fprintf(stderr, "%f, %f\n", fstream[i], audioscale);
//		}
//	}
	
}

bool afAudioInit(const SDL_AudioSpec *want, SDL_AudioSpec *have)
{
	SDL_AudioSpec wantd = *want;
	wantd.callback=afAudioCallback;
	afDevice = SDL_OpenAudioDevice(
			0,0,
			&wantd, have,
			SDL_AUDIO_ALLOW_ANY_CHANGE);
	afAudioBuffer.samples = have->samples;
	afAudioBuffer.freq = have->freq;
	afAudioBuffer.channels = have->channels;
	SDL_PauseAudioDevice(afDevice,0);
	return (want->format == have->format);
}

void afSquareSynth::addOutput(float *buffer, int len, unsigned long int start) {
	unsigned long int end = (start+len > endTime)?endTime:start+len;
	//fprintf(stderr, "writing squarewave %lu to %lu (%f seconds)\n",start, end, (float)(end - start)/ afAudioBuffer.freq);
	int i = 0;
	unsigned int denom = afAudioBuffer.freq/hertz;
	for (; start < end; start++) {
		if (start < startTime) continue;
		buffer[i] += ((start/denom)%2 == 0)?volume:-volume;
		i++;
	}
}

void afPushAudio(float frametime)
{
	if (afAudioBuffer.laps) {
		afAudioBuffer.laps = 0;
		afAudioBuffer.writept = afAudioBuffer.readpt + 1;
	}
	uli writetime = afGetWriteTime();
	uint bufferSize = frametime * afAudioBuffer.freq;
	float * buffer = (float*)malloc(bufferSize * sizeof(float));
	for (uint i = 0; i < bufferSize; i++) buffer[i] = 0.f;
	
	//fill audio buffer here
	for (uint c = afSquareCount; c--;) {
		if (afASS[c].endTime < afAudioBuffer.audiotime)
			afSquareCount--;
		else {
			afASS[c].addOutput(buffer, bufferSize, writetime);
		}
	}

	for (uint i = 0; i < bufferSize; ++i)
	{
		afAudioRingWrite(buffer[i]);
	}
	free(buffer);
}

