#pragma once

#include <Dsound.h>
#include <stdio.h>
#pragma comment (lib, "Dsound.lib")

struct WaveHeaderType
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};

class AudioManager
{
public:
	static AudioManager* Instance();
	AudioManager(int number);
	~AudioManager() = default;
	void LoadWav(const char* wavName, int index);
	void PlayWav(int index);

private:
	IDirectSound8* directSound;
	IDirectSoundBuffer* primaryBuffer;
	IDirectSoundBuffer** secondaryBuffers;
	int numSounds;
};

