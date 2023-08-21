/*
*
* genAudio, programatically creates wave audio files
*
*/



#include<iostream>
#include<fstream>
#include<string>
#include<vector>


struct Chunks
{
		// RIFF
	const char chunkID[4] = {'R', 'I', 'F', 'F'};
	int chunkSize;
	const char format[4] = {'W', 'A', 'V', 'E'};

		// FMT subchunk
	const char SubChunk1_ID[4] = {'f', 'm', 't', 0x20};
	const int SubChunk1_size = 16;
	const short audioFormat = 1;
	const short numChannels = 2;
	const int sampleRate = 44100;
	int byteRate;
	short blockAlign;
	short bitsPerSample;

		// data sub chunk
	const char subChunk2_ID[4] = {'d', 'a', 't', 'a'};
	int subChunk2_size = 0;
};

class Audio
{
	Chunks chunk;
	
	const short bps = sizeof(float) * 8;

	int frequency, amplitude, duration; // max amp = 32760

	std::vector<float> samples;

	public:
		Audio(int freq = 250, int amp = 1, int length = 5) : frequency(freq), amplitude(amp), duration(length)
		{
			chunk.byteRate = chunk.sampleRate *  chunk.numChannels * bps / 8;

			chunk.blockAlign = chunk.numChannels * bps / 8;

			chunk.bitsPerSample = bps;
		}

		Audio& generate();

		friend std::ofstream& operator<<(std::ofstream& outs, const Audio& wav);

		friend std::ifstream& operator>>(std::ifstream& ins, Audio& wav);
};

