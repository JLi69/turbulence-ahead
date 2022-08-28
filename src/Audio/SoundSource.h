#ifndef SOUND_DEV_H
#include <AL/al.h>

class SoundSource
{
	ALuint mSource;
	float mPitch = 1.0f,
		  mGain = 1.0f,
		  mPosition[3] = { 0.0f, 0.0f, 0.0f },
		  mVelocity[3] = { 0.0f, 0.0f, 0.0f };
	bool mLoopSound = false;
	ALuint mBuffer = 0;
public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffer_to_play);
};
#define SOUND_DEV_H
#endif
