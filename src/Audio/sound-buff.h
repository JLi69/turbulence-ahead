#include <AL/al.h>
#include <vector>
#include <string>
#include <map>

class SoundBuffer
{
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> mSoundEffects;
	std::map<std::string, ALuint> mSounds;
public:
	static SoundBuffer* get();

	ALuint getSoundId(std::string name);
	ALuint addSoundEffect(const char *soundpath);
	bool removeSoundEffect(const ALuint &buffer);
};
