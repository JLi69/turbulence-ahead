#include <AL/al.h>
#include <sndfile.h>

#define BUFFER_SAMPLES 8192
#define NUM_BUFFERS 4

class MusicBuffer
{	
	ALuint mMusicSrc;	
	ALuint mBuffers[NUM_BUFFERS];
	
	SNDFILE* mMusicFile;
	SF_INFO mSfinfo;
	short* mMembuf;
	ALenum mFormat;
public:
	void Play();
	void UpdateBufferStream();
	ALint getSource();

	MusicBuffer(const char *filename);
	~MusicBuffer();
};
