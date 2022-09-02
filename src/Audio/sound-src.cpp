#include "sound-src.h"
#include <stdio.h>

SoundSource::SoundSource()
{
	alGenSources(1, &mSrc);
	alSourcef(mSrc, AL_PITCH, 1.0f);
	alSourcef(mSrc, AL_GAIN, 1.0f);
	alSource3f(mSrc, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(mSrc, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(mSrc, AL_LOOPING, 0);
	alSourcei(mSrc, AL_BUFFER, mBuff);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &mSrc);
}

void SoundSource::Play(const ALuint buffToPlay)
{
	if(buffToPlay != mBuff)
	{
		mBuff = buffToPlay;
		alSourcei(mSrc, AL_BUFFER, (ALint)mBuff);
	}

	alSourcePlay(mSrc);	
}

ALuint SoundSource::getId()
{
	return mSrc;
}
