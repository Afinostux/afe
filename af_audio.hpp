#pragma once
#include "include/SDL_audio.h"

void afAudioCallback(void *udata, Uint8 *stream, int len);

bool afAudioInit(const SDL_AudioSpec *want, SDL_AudioSpec *have);

extern SDL_AudioDeviceID afDevice;
