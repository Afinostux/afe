
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
	uli start = afAudioBuffer.audiotime + afAudioBuffer.freq * delay,
	    end = start + afAudioBuffer.freq * duration;
	if (afSquareCount == afSquareMax) return 0;
	if (afSquareCount)
	{
		// keep in sorted order, more recently discarded ones at the back
		uint current = afSquareCount++;
		while (afASS[current].endTime < end && current > 0) current--;
		for (uint i = afSquareCount; i-- >= current;)
		{
			afASS[i+1] = afASS[i];
		}
		afASS[current] = afSquareSynth(start, end, hertz, volume);
	}
	else
	{
		afASS[afSquareCount++] = afSquareSynth(start, end, hertz, volume);
	}
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
	if (afAudioBuffer.laps) return afAudioBuffer.audiotime;
	unsigned int wpoffset = AF_AUDIO_BUFFER_SIZE - afAudioBuffer.writept;
	unsigned long int result = (afAudioBuffer.audiotime + wpoffset)%AF_AUDIO_BUFFER_SIZE;
	result = AF_AUDIO_BUFFER_SIZE - result;
	return result + afAudioBuffer.audiotime;
}

float audioscale = 1;
const float audioceiling = 0.4;

void afAudioCallback(void *udata, Uint8 *stream, int len)
{
	float raw;
	float rawsign;
	for (int i = 0; i < len; i++)
	{
		raw = afAudioRingRead();
		rawsign = (raw < 0)?-1.f:1.f;
		raw *= rawsign;
		audioscale = (raw > audioceiling)?
			audioceiling/(raw * (1/audioceiling)):
			audioscale + (1-audioscale)/10;
		stream[i] = raw*rawsign*audioscale;
		afAudioBuffer.audiotime++;
	}
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

void afPushAudio(float frametime)
{
	if (afAudioBuffer.laps) {
		afAudioBuffer.laps = 0;
		afAudioBuffer.writept = afAudioBuffer.readpt + 1;
	}
	uli writetime = afGetWriteTime();
	uint bufferSize = frametime * afAudioBuffer.freq;
	float * buffer = (float*)malloc(bufferSize * sizeof(float));

	//fill audio buffer here
	for (uint c = afSquareCount; c--;) {
		if (afASS[c].endTime < writetime)
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

