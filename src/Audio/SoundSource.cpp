#include "SoundSource.h"
#include <iostream>

SoundSource::SoundSource()
{
	alGenSources(1, &mSource);
	alSourcef(mSource, AL_PITCH, mPitch);
	alSourcef(mSource, AL_GAIN, mGain);
	alSource3f(mSource, AL_POSITION, mPosition[0], mPosition[1], mPosition[2]);
	alSource3f(mSource, AL_VELOCITY, mVelocity[0], mVelocity[1], mVelocity[2]);
	alSourcei(mSource, AL_LOOPING, mLoopSound);
	alSourcei(mSource, AL_BUFFER, mBuffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &mSource);
}

void SoundSource::Play(const ALuint buffer_to_play)
{
	if(buffer_to_play != mBuffer)
	{
		mBuffer = buffer_to_play;
		alSourcei(mSource, AL_BUFFER, (ALint)mBuffer);
	}

	alSourcePlay(mSource);

	ALint state = AL_PLAYING;
	printf("Playing sound\n");	
	while(state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		//printf("currently playing sound\n");
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);	
	}
	printf("done playing sound\n");
}
