#include "sound-buff.h"
#include <sndfile.h>
#include <AL/alext.h>
#include <stdlib.h>
#include <stdio.h>

SoundBuffer* SoundBuffer::get()
{
	static SoundBuffer* sndBuff = new SoundBuffer;
	return sndBuff;
}

ALuint SoundBuffer::getSoundId(std::string name)
{
	return mSounds[name];
}

ALuint SoundBuffer::addSoundEffect(const char *soundpath)
{
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	sndfile = sf_open(soundpath, SFM_READ, &sfinfo);

	//File failed to open
	if(!sndfile)
	{
		fprintf(stderr, "could not open audio file %s: %s", soundpath, sf_strerror(sndfile));
		return 0;	
	}
	if(sfinfo.frames <1 || sfinfo.frames > (sf_count_t)(INT32_MAX / sizeof(short) / sfinfo.channels))
	{
		fprintf(stderr, "Bad sample count in %s (%ld)\n", soundpath, sfinfo.frames);
		sf_close(sndfile);	
		return 0;
	}

	//Figure out format
	format = AL_NONE;
	if(sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if(sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;

	if(!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if(num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%ld)\n", soundpath, num_frames);
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	//Check for any errors
	err = alGetError();
	if(err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if(buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	mSoundEffects.push_back(buffer);
	mSounds[soundpath] = buffer;

	return buffer;
}

bool SoundBuffer::removeSoundEffect(const ALuint &buffer)
{
	for(auto it = mSoundEffects.begin(); it != mSoundEffects.end(); it++)
	{
		if(*it == buffer)
		{
			alDeleteBuffers(1, &*it);
			it = mSoundEffects.erase(it);
			return true;
		}
	}

	return false;
}

SoundBuffer::SoundBuffer()
{
	mSoundEffects.clear();
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(mSoundEffects.size(), mSoundEffects.data());
	mSoundEffects.clear();
}
