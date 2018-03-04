#include "voice.h"

// Single instance of PocketSphinx voice
// ps variables
static ps_decoder_t *ps;
static cmd_ln_t *config;

// mic in variables
static ad_rec_t *ad;
static int16_t adbuf[PS_BUF_SIZE];
static uint32_t buf_count;
static int32_t k;

char const* voice_str;

static char command[256];

void voiceHardwareSetup() {

  // Set up output through speakers
  sprintf(command, "amixer -c 0 cset iface=MIXER,name='SPK DAC Switch' 1");
  system(command);
  sprintf(command, "amixer -c 0 cset iface=MIXER,name='RX3 MIX1 INP1' 'RX1'");
  system(command);
  sprintf(command, "amixer cset iface=MIXER,name='RX3 Digital Volume' 136");
  system(command);

  // default input to mic over audio jack
  sprintf(command, "amixer -c 0 cset iface=MIXER,name='DEC1 MUX' 'ADC2'");
  system(command);
  sprintf(command, "amixer cset iface=MIXER,name='ADC2 Volume' 0");
  system(command);
  // INP3 is for mic on board, INP2 is for auxilliary line in.
  sprintf(command, "amixer -c 0 cset iface=MIXER,name='ADC2 MUX' 'INP3'");
  system(command);
}

void voiceDictionarySetup() {
  config = cmd_ln_init(NULL, ps_args(), TRUE,
		       "-hmm", "/usr/local/share/pocketsphinx/model/en-us/en-us",
		       "-lm", PS_CHARLIE_LM,
		       "-dict", PS_CHARLIE_DICT,
		       NULL);

  if (config == NULL) {
    fprintf(stderr, "Failed to create config object, see log for details\n");
    exit(-1);
  }

  ps = ps_init(config);
  if (ps == NULL) {
    fprintf(stderr, "Failed to create recognizer, see log for details\n");
    exit(-1);
  }
}

int voiceCommand() {

  if ((ad = ad_open_dev(PS_ALSA_VOICE_MIC_HW, 16000)) == NULL) {
    fprintf(stderr, "Failed to open audio device\n");
    exit(-1);
  }

  // start recording
  if (ad_start_rec(ad) < 0) {
    fprintf(stderr, "Failed to start recording\n");
    return -1;
  }

  if (ps_start_utt(ps) < 0) {
    fprintf(stderr, "Failed to start utterance\n");
    return -1;
  }

  uint8_t utt_started = 0;
  uint8_t in_speech;

  for (;;) {
    // Start getting data
    if ((k = ad_read(ad, adbuf, PS_BUF_SIZE)) < 0) {
      fprintf(stderr, "Failed to read audio\n");
      ad_close(ad);
      return -1;
    }

    ps_process_raw(ps, adbuf, k, FALSE, FALSE);
    in_speech = ps_get_in_speech(ps);

    if (in_speech && !utt_started) {
      utt_started = 1;
    }

    if (!in_speech && utt_started) {
      ps_end_utt(ps);
      voice_str = ps_get_hyp(ps, NULL);
      if (voice_str != NULL) {
	// do all the switch case in seperate file
	ad_close(ad);
	return commandDetect(voice_str);
      } else {
	printf("LOG: Voice_str was null");
	ad_close(ad);
	return -1;
      }

      if (ps_start_utt(ps) < 0) {
	fprintf(stderr, "Failed to start utterance\n");
	ad_close(ad);
	return -1;
      }
    }
  } // for(;;)
}

void voiceCleanUp()
{
  ps_free(ps);
  cmd_ln_free_r(config);
}

int playAudio(const char* clip) {
  sprintf(command, "aplay -D plughw:0,1 audio/%s", clip);
  system(command);
  return 0;
}

int commandDetect(char const* voice_str) {

  char* detect;

  printf("DEBUG - Said: %s\n", voice_str);

  if(strstr(voice_str, "CHARLIE") == NULL){
    return 0;
  }

  if(strstr(voice_str, "PLAY") != NULL) {
      if(strstr(voice_str, "TOTO") != NULL) {
	return playAudio(A_TOTO);
      }
      if((strstr(voice_str, "ALL") != NULL) && (strstr(voice_str, "STAR") != NULL)) {
	return playAudio(A_ALL_STAR);
      }

      return playAudio(A_SONG_NOT);
  }
  
  if(strstr(voice_str, "SOYLENT") != NULL) {
    return playAudio(A_SOYLENT);
  }

  if ((strstr(voice_str, "KNOW") != NULL) && (strstr(voice_str, "WAY") != NULL)) {
    return playAudio(A_KNOW_WAY);
  }

  // nothing found
  return -1;
}
