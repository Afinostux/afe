#include "af_keys.h"

const uint max_keys = 32;
uint key_count = 0;
afeKey afkeys[max_keys];

afeKey* getKey(SDL_Keycode code){
	uint kc;
	for (kc = key_count; kc--;)
	{
		if (afkeys[kc].key == code) return &afkeys[kc];
	}
	if (key_count == max_keys) return 0;
	afkeys[key_count] = afeKey(code);
	return &afkeys[key_count++];
}

void handleKeys(SDL_KeyboardEvent * kev){
	for (uint kc = key_count; kc--;)
	{
		afkeys[kc].handle(kev);
	}
}

void tickKeys(uint ms){
	for (uint kc = key_count; kc--;)
	{
		afkeys[kc].tick(ms);
	}
}
