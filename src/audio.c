#include "audio.h"

#ifdef _WIN32
#include <windows.h>
#endif

void AudioInit(void) {
}

void AudioShutdown(void) {
}

void AudioPlayPair(void) {
#ifdef _WIN32
	Beep(880, 20);
#endif
}

void AudioPlayMismatch(void) {
#ifdef _WIN32
	Beep(320, 60);
#endif
}

void AudioPlayGameOver(void) {
#ifdef _WIN32
	Beep(220, 90);
#endif
}
