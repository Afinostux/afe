
#include "af_audio.hpp"

SDL_AudioDeviceID afDevice = 0;

void afAudioCallback(void *udata, Uint8 *stream, int len)
{
	SDL_memset(stream, 0, len);
}

bool afAudioInit(const SDL_AudioSpec *want, SDL_AudioSpec *have)
{
	SDL_AudioSpec wantd = *want;
	wantd.callback=afAudioCallback;
	afDevice = SDL_OpenAudioDevice(
			0,0,
			&wantd, have,
			SDL_AUDIO_ALLOW_ANY_CHANGE);
	SDL_PauseAudioDevice(afDevice,0);
	return (want->format == have->format);
}
