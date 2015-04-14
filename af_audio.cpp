
#include "af_audio.hpp"

SDL_AudioDeviceID afDevice = 0;
afAudioRing afAudioBuffer = {};

float afAudioRingRead(void)
{
	afAudioBuffer.laps |= !(afAudioBuffer.readpt & afAudioBuffer.writept);
	float rv = afAudioBuffer.buffer[afAudioBuffer.readpt++];
	afAudioBuffer.readpt %= AF_AUDIO_BUFFER_SIZE;
	return rv;
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
