#include "SoundBuffer.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <stdlib.h>

SoundBuffer* SoundBuffer::get()
{
	static SoundBuffer* sndBuff = new SoundBuffer();
	return sndBuff;
}

ALuint SoundBuffer::addSoundEffect(const char *filename)
{
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;
	
	//Open the audio file and check that it can be used
	//Pass in the filename, the mode, and where the info should be put
	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	//File failed to open
	if(!sndfile)
	{
		fprintf(stderr, "could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
		return 0;
	}
	if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT32_MAX / sizeof(short) / sfinfo.channels))
	{
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
		sf_close(sndfile);	
		return 0;	
	}

	//Figure out the format of the sound file
	format = AL_NONE;
	if(sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if(sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if(sfinfo.channels == 3)
	{
		if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if(sfinfo.channels == 4)
	{
		if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if(!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;	
	}

	//Decode the whole audio file to a buffer
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if(num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	/* Buffer the audio data into a new buffer object, then free the data
	 * and close the file
	 */
	
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
	
	free(membuf);
	sf_close(sndfile);

	//Check if an error occured, then clean if so
	err = alGetError();
	if(err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Erorr: %s\n", alGetString(err));
		if(buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}
	
	mSoundEffectBuffers.push_back(buffer); //add to the list of known buffers

	return buffer;
}

bool SoundBuffer::removeSoundEffect(const ALuint &buffer)
{
	auto it = mSoundEffectBuffers.begin();
	while(it != mSoundEffectBuffers.end())
	{
		if(*it == buffer)
		{
			alDeleteBuffers(1, &*it);
			it = mSoundEffectBuffers.erase(it);
			return true;
		}
		else
			it++;
	}
	return false; //Couldn't find the buffer to remove
}

SoundBuffer::SoundBuffer()
{
	mSoundEffectBuffers.clear();
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(mSoundEffectBuffers.size(), mSoundEffectBuffers.data());
	mSoundEffectBuffers.clear();
}
