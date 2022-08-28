#ifndef MUSIC_BUFFER_H
#include <AL/al.h>
#include <sndfile.h>

class MusicBuffer
{
	ALuint mSource;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint mBuffers[NUM_BUFFERS];
	SNDFILE* mSndFile;
	SF_INFO mSfinfo;
	short* mMembuf;
	ALenum mFormat;

	MusicBuffer() = delete;
public:
	void Play();

	void UpdateBufferStream();

	ALint getSource();
	MusicBuffer(const char *filename);
	~MusicBuffer();
};
#define MUSIC_BUFFER_H
#endif
