/*
*
* genAudio, programatically creates wave audio files
*
*/



#include "genAudio.h"
#include<cmath>
#include<cstring>
#include<utility>


Audio& Audio::generate()
{
	int numSamples = duration * chunk.sampleRate;

	for(int s = 0; s < numSamples; s++)
	{
		float chan1 = amplitude1 * sin(2 * M_PI * s * frequency1 + phase1) + amplitude1 * cos(2 * M_PI * s * frequency1 + phase1);

		float chan2 = amplitude2 * cos(2 * M_PI * s * frequency2 + phase2) + amplitude2 * sin(2 * M_PI * s * frequency2 + phase2);;

		samples.push_back(chan1);
		samples.push_back(chan2);
	}

	chunk.subChunk2_size = numSamples * chunk.numChannels * chunk.bitsPerSample / 8; // data size

	chunk.chunkSize = 36 + chunk.subChunk2_size;

	return *this;
}

std::ofstream& operator<<(std::ofstream& outs, const Audio& wav)
{
	if(wav.samples.size() == 0)
		throw std::string("No Audio Data!!");
			
	char* buffer = new char[sizeof(Chunks) + wav.samples.size() * sizeof(float) + sizeof(int) * 7];
			
	std::memcpy(buffer, reinterpret_cast<const char *>(&wav.chunk), sizeof(Chunks));
			
	int offset = sizeof(Chunks);
	std::memcpy(buffer + offset, reinterpret_cast<const char *>(wav.samples.data()), wav.samples.size() * sizeof(float));
			
	offset += wav.samples.size() * sizeof(float);
	int params[] = {wav.frequency1, wav.amplitude1, wav.phase1, wav.frequency2, wav.amplitude2, wav.phase2, wav.duration};
	std::memcpy(buffer + offset, reinterpret_cast<const char *>(params), sizeof(int) * 7);
			
	outs.write(buffer, offset + sizeof(int) * 7);
			
	if(!outs.good())
		throw std::string("File Write ERROR!!");
			
	delete[] buffer;

	return outs;
}

std::ifstream& operator>>(std::ifstream& ins, Audio& wav)
{
	ins.seekg(0, std::ios_base::end);
	int end = ins.tellg();
	ins.seekg(0, std::ios_base::beg);
			
	if(!ins.good())
		throw std::string("File Read ERROR!!");
			
	char* buffer = new char[end];

	ins.read(buffer, end);
			
	if(!ins.good())
		throw std::string("File Read ERROR!!");

	std::pair<const char*, short> chunkIDs[] = { std::pair<const char*, short>(wav.chunk.chunkID, 0),
												 std::pair<const char*, short>(wav.chunk.format, 8),
												 std::pair<const char*, short>(wav.chunk.SubChunk1_ID, 12),
												 std::pair<const char*, short>(wav.chunk.subChunk2_ID, 36) };
			
	for(auto& [id, o]: chunkIDs)
	{
		int equal = std::memcmp(buffer + o, id, 4);
				
		if(equal != 0)
			throw std::string("Invalid File Format!!");
	}

	std::memcpy(reinterpret_cast<char *>(&wav.chunk.chunkSize), buffer + 4, 4);
			
	std::memcpy(reinterpret_cast<char *>(&wav.chunk.byteRate), buffer + 28, 4);
	std::memcpy(reinterpret_cast<char *>(&wav.chunk.blockAlign), buffer + 32, 2);
	std::memcpy(reinterpret_cast<char *>(&wav.chunk.bitsPerSample), buffer + 34, 2);
			
	std::memcpy(reinterpret_cast<char *>(&wav.chunk.subChunk2_size), buffer + 40, 4);
			
	float data[wav.chunk.subChunk2_size / sizeof(float)];
			
	std::memcpy(reinterpret_cast<char *>(data), buffer + 44, wav.chunk.subChunk2_size);
			
	wav.samples = std::vector<float>(data, data + wav.chunk.subChunk2_size / sizeof(float));
			
	int pds = end - (sizeof(Chunks) + wav.chunk.subChunk2_size); // parameter data size
			
	if(pds == sizeof(int) * 7)
	{
		int* params[] = {&wav.frequency1, &wav.amplitude1, &wav.phase1, &wav.frequency2, &wav.amplitude2, &wav.phase2, &wav.duration};
				
		int offset = wav.chunk.subChunk2_size;

		for(auto& p: params) {
			std::memcpy(reinterpret_cast<char *>(p), buffer + offset, sizeof(int));
			offset += sizeof(int);
		}
	}
	else
		throw std::string("No Parameter Data!!");

	delete[] buffer;

	return ins;
}

