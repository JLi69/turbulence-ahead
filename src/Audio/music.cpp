#include "music.h"
#include <stdlib.h>

void MusicBuffer::Play()
{
	ALsizei i;
	alGetError();

	alSourceRewind(mMusicSrc);
	alSourcei(mMusicSrc, AL_BUFFER, 0);

	for(i = 0; i < NUM_BUFFERS; i++)
	{
		sf_count_t slen = sf_readf_short(mMusicFile, mMembuf, BUFFER_SAMPLES);
		if(slen < 1) break;
		slen *= mSfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(mBuffers[i], mFormat, mMembuf, (ALsizei)slen, mSfinfo.samplerate);
	}

	if(alGetError() != AL_NO_ERROR)
		fprintf(stderr, "Error buffer for playback\n");

	alSourceQueueBuffers(mMusicSrc, i, mBuffers);
	alSourcePlay(mMusicSrc);
	if(alGetError() != AL_NO_ERROR)
		fprintf(stderr, "Error starting playback\n");
}

void MusicBuffer::UpdateBufferStream()
{
	ALint processed, state;
	alGetError();

	alGetSourcei(mMusicSrc, AL_SOURCE_STATE, &state);
	alGetSourcei(mMusicSrc, AL_BUFFERS_PROCESSED, &processed);

	if(alGetError() != AL_NO_ERROR)
		fprintf(stderr, "Error checking music source state\n");

	while(processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(mMusicSrc, 1, &bufid);
		processed--;
		slen = sf_readf_short(mMusicFile, mMembuf, BUFFER_SAMPLES);	
		if(slen > 0)
		{
			slen *= mSfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, mFormat, mMembuf, (ALsizei)slen, mSfinfo.samplerate);
			alSourceQueueBuffers(mMusicSrc, 1, &bufid);
		}

		if(alGetError() != AL_NO_ERROR)
			fprintf(stderr, "Error buffering music data\n");
	}

	if(state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;
		alGetSourcei(mMusicSrc, AL_BUFFERS_QUEUED, &queued);
		if(queued == 0)
			return;
		alSourcePlay(mMusicSrc);
		if(alGetError() != AL_NO_ERROR)
			fprintf(stderr, "error restarting music playback\n");
	}
}

ALint MusicBuffer::getSource()
{
	return mMusicSrc;
}

MusicBuffer::MusicBuffer(const char *filename)
{
	alGenSources(1, &mMusicSrc);
	alGenBuffers(NUM_BUFFERS, mBuffers);

	size_t frame_size;
	mMusicFile = sf_open(filename, SFM_READ, &mSfinfo);
	if(!mMusicFile)
		fprintf(stderr, "Could not open music file: %s\n", filename);

	mFormat = AL_NONE;
	if(mSfinfo.channels == 1)
		mFormat = AL_FORMAT_MONO16;
	else if(mSfinfo.channels == 2)
		mFormat = AL_FORMAT_STEREO16;

	if(!mFormat)
	{
		sf_close(mMusicFile);
		mMusicFile = NULL;
		fprintf(stderr, "Unsupported channeld count from file");
	}

	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)mSfinfo.channels) * sizeof(short);
	mMembuf = static_cast<short*>(malloc(frame_size));
}

MusicBuffer::~MusicBuffer()
{
	alDeleteSources(1, &mMusicSrc);
	if(mMusicFile)
		sf_close(mMusicFile);
	mMusicFile = nullptr;
	free(mMembuf);
	alDeleteBuffers(NUM_BUFFERS, mBuffers);
}
