
#pragma once
#include <SDL_events.h>

typedef struct afeKey {
	bool pressed, released;
	uint pressedms, releasedms;
	SDL_Keycode key;

	afeKey() : 
		pressed(false), released(false),
		pressedms(0), releasedms(0), key(0)
	{}

	afeKey(SDL_Keycode k) : 
		pressed(false), released(false),
		pressedms(0), releasedms(0), key(k)
	{}

	inline void handle(SDL_KeyboardEvent * kev){
		if ((kev->keysym.sym) == (key) && !kev->repeat) {
			switch (kev->state) {
				case (SDL_PRESSED):
					pressed = true;
					released = false;
					pressedms = 0;
					break;
				case (SDL_RELEASED):
					pressed = false;
					released = true;
					releasedms = 0;
					break;
				default:
					break;
			}
		}
	}

	inline void tick(uint ms){
		if (pressed) pressedms += ms;
		if (released) releasedms += ms;
	}
} afeKey;

extern const uint max_keys;
extern uint key_count;

afeKey* getKey(SDL_Keycode code);
void handleKeys(SDL_KeyboardEvent * kev);
void tickKeys(uint ms);
