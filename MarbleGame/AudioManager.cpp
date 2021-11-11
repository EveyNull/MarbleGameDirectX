#include "AudioManager.h"
#include "Window.h"
#include <direct.h>

AudioManager::AudioManager(int numberWavs)
{
	numSounds = numberWavs;
	secondaryBuffers = new IDirectSoundBuffer * [numberWavs];
	
	HWND hwnd = Window::MainWindow();
	DirectSoundCreate8(NULL, &directSound, NULL);
	directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL);

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	primaryBuffer->SetFormat(&waveFormat);
}

void AudioManager::LoadWav(const char* fileName, int index)
{
	if (index > numSounds - 1) return;

	IDirectSoundBuffer** secondaryBuffer = &secondaryBuffers[index];

	char* buffer;
	buffer = _getcwd(NULL, 0);
	FILE* file;
	WaveHeaderType waveFileHeader;
	int error;
	unsigned int count;
	
	error = fopen_s(&file, fileName, "rb");
	if (error != 0)
	{
		return;
	}

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, file);
	if(count != 1)
	{
		return;
	}

	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	IDirectSoundBuffer* tempBuffer;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	HRESULT result;

	result = directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	
	tempBuffer->Release();

	fseek(file, sizeof(WaveHeaderType), SEEK_SET);


	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	waveData = new unsigned char[waveFileHeader.dataSize];
	count = fread(waveData, 1, waveFileHeader.dataSize, file);
	if (count != waveFileHeader.dataSize)
	{
		return;
	}
	result = fclose(file);

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);

	delete[] waveData;
}

void AudioManager::PlayWav(int index)
{
	secondaryBuffers[index]->SetCurrentPosition(0);
	secondaryBuffers[index]->SetVolume(DSBVOLUME_MAX);
	secondaryBuffers[index]->Play(0, 0, 0);
}