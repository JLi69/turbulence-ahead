#include <AL/al.h>

class SoundSource
{
	ALuint mSrc;
	ALuint mBuff = 0;

public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffToPlay);

	ALuint getId();
};
