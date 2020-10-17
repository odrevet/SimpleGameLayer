#include "sound.h"

char sound_init()
{
  if (Mix_OpenAudio(48000, AUDIO_S16MSB, 2, 512) < 0)
  {
    fprintf(stderr, "Unable to init sound engine: %s\n", SDL_GetError());
    return 0;
  }
  return 1;
}

Mix_Music *music_load(char *full_path)
{
  Mix_Music *tmp_music;
  tmp_music = Mix_LoadMUS(full_path);
  if (!tmp_music)
  {
    fprintf(stderr, "Mix_LoadMUS(\"%s\"): %s\n", full_path, Mix_GetError());
    return NULL;
  }
  return tmp_music;
}

void music_play(Mix_Music *music)
{
  if (Mix_PlayMusic(music, -1) == -1)
  {
    fprintf(stderr, "Mix_PlayMusic: %s\n", Mix_GetError());
  }
}

Mix_Chunk *sample_load(char *full_path)
{
  Mix_Chunk *sample;
  sample = Mix_LoadWAV(full_path);
  if (!sample)
  {
    fprintf(stderr, "Mix_LoadWAV: %s\n", Mix_GetError());
  }
  return sample;
}

void sample_play(Mix_Chunk *sample)
{
  int channel;
  channel = Mix_PlayChannel(-1, sample, 0);
  if (channel == -1)
  {
    fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
  }
}
