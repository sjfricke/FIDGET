#ifndef __AUDIO_VOICE_H__
#define __AUDIO_VOICE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>

#define PS_BUF_SIZE 4096
#define PS_BUF_TIME 15

#define PS_CHARLIE_LM "/home/linaro/FIDGET/boards/dragonAudio/charlie.lm"
#define PS_CHARLIE_DICT "/home/linaro/FIDGET/boards/dragonAudio/charlie.dict"

#define PS_ALSA_VOICE_MIC_HW "plughw:0,2"

void voiceHardwareSetup();

void voiceDictionarySetup();

// return -1 if invalid command
int voiceCommand();

void voiceCleanUp();

int playAudio(const char* clip);

int commandDetect(char const* voice_str);

#endif
