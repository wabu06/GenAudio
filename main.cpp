/*
*
* genAudio, programatically creates wave audio files
*
*/


#define MAIN

#include "genAudio.h"


int main(int argc, char** argv)
{
	std::string fn;

	if(argc > 1)
		fn = std::string(argv[1]) + ".wav";
	else
		fn = "myAudio.wav";

	Audio wav;

	std::ofstream fout(fn, std::ios::binary);

	if(!fout)
	{
		std::cerr << "\nFile I/O Error!!" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		fout << wav.generate();
	}
	catch(const std::string& exp)
	{
		std::cerr << '\n' << exp << std::endl;
		fout.close();
		return EXIT_FAILURE;
	}

	fout.close();

	return EXIT_SUCCESS;
}
