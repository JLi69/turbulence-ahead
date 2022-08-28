#include "MusicBuffer.h"
#include <AL/alext.h>
#include <malloc.h>

void MusicBuffer::Play()
{
	ALsizei i;

	//Clear any errors
	alGetError();

	//Rewind the source position and clear the buffer queue
	alSourceRewind(mSource);
	alSourcei(mSource, AL_BUFFER, 0);
	
	//Fill the buffer queue
	for(i = 0; i < NUM_BUFFERS; i++)
	{
		//Get some data to give it to the buffer
		sf_count_t slen = sf_readf_short(mSndFile, mMembuf, BUFFER_SAMPLES);
		if(slen < 1) break;
		slen *= mSfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(mBuffers[i], mFormat, mMembuf, (ALsizei)slen, mSfinfo.samplerate);	
	}
	if(alGetError() != AL_NO_ERROR)
		throw("Error buffering for playback");
	//Now queue and start playback
	alSourceQueueBuffers(mSource, i, mBuffers);
	alSourcePlay(mSource);
	if(alGetError() != AL_NO_ERROR)
		throw("Error starting playback");
}

void MusicBuffer::UpdateBufferStream()
{
	ALint processed, state;

	//clear error
	alGetError();

	//Get relevant source info
	alGetSourcei(mSource, AL_SOURCE_STATE, &state);
	alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
	if(alGetError() != AL_NO_ERROR)
		throw("Error checking music source state");

	//Unqueue and handle each processed buffer
	while(processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(mSource, 1, &bufid);
		processed--;

		//Read the next chunk of data, refill the buffer, and queue it back into the source
		slen = sf_readf_short(mSndFile, mMembuf, BUFFER_SAMPLES);
		if(slen > 0)
		{
			slen *= mSfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, mFormat, mMembuf, (ALsizei)slen, mSfinfo.samplerate);	
			alSourceQueueBuffers(mSource, 1, &bufid);	
		}
		if(alGetError() != AL_NO_ERROR)
			throw("error buffering music data");	
	}
	
	if(state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;
		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
		if(queued == 0)
			return;
		alSourcePlay(mSource);
		if(alGetError() != AL_NO_ERROR)
			throw("error restarting music playback");
	}
}

ALint MusicBuffer::getSource()
{
	return mSource;
}

MusicBuffer::MusicBuffer(const char *filename)
{
	alGenSources(1, &mSource);
	alGenBuffers(NUM_BUFFERS, mBuffers);

	size_t frame_size;

	mSndFile = sf_open(filename, SFM_READ, &mSfinfo);
	if(!mSndFile)
		throw("could not open provided music file -- check path");
	
	//Get the sound formate, and figure out the OpenAL format
	mFormat = AL_NONE;	
	if(mSfinfo.channels == 1)
		mFormat = AL_FORMAT_MONO16;
	else if(mSfinfo.channels == 2)
		mFormat = AL_FORMAT_STEREO16;
	else if(mSfinfo.channels == 3)
	{	
		if(sf_command(mSndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			mFormat = AL_FORMAT_BFORMAT2D_16;
	}	
	else if(mSfinfo.channels == 4)
	{	
		if(sf_command(mSndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			mFormat = AL_FORMAT_BFORMAT3D_16;
	}
	
	if(!mFormat)
	{
		sf_close(mSndFile);
		mSndFile = NULL;
		throw("Unsupported channeld count from file");
	}
	
	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)mSfinfo.channels) * sizeof(short);
	mMembuf = static_cast<short*>(malloc(frame_size));
}

MusicBuffer::~MusicBuffer()
{
	alDeleteSources(1, &mSource);

	if(mSndFile)
		sf_close(mSndFile);

	mSndFile = nullptr;

	free(mMembuf);

	alDeleteBuffers(NUM_BUFFERS, mBuffers);
}
