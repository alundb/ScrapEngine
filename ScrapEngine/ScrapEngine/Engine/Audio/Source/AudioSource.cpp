#include <Engine/Audio/Source/AudioSource.h>

ScrapEngine::Audio::AudioSource::AudioSource(const ALuint* audio_buffer)
{
	alGenSources(static_cast<ALuint>(1), &source_);
	alSourcei(source_, AL_BUFFER, *audio_buffer);
}

ScrapEngine::Audio::AudioSource::~AudioSource()
{
	stop();
	alDeleteSources(1, &source_);
}

void ScrapEngine::Audio::AudioSource::set_source_location(const Core::SVector3& location) const
{
	alSource3f(source_, AL_POSITION, location.get_x(), location.get_y(), location.get_z());
}

void ScrapEngine::Audio::AudioSource::set_source_velocity(const Core::SVector3& velocity) const
{
	alSource3f(source_, AL_VELOCITY, velocity.get_x(), velocity.get_y(), velocity.get_z());
}

void ScrapEngine::Audio::AudioSource::set_source_pitch(const float pitch) const
{
	alSourcef(source_, AL_PITCH, pitch);
}

void ScrapEngine::Audio::AudioSource::set_source_gain(const float gain) const
{
	alSourcef(source_, AL_GAIN, gain);
}

void ScrapEngine::Audio::AudioSource::set_source_loop(const bool loop) const
{
	if(loop)
	{
		alSourcei(source_, AL_LOOPING, AL_TRUE);
	}else
	{
		alSourcei(source_, AL_LOOPING, AL_FALSE);
	}
}

bool ScrapEngine::Audio::AudioSource::is_playing() const
{
	ALenum state;

	alGetSourcei(source_, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

void ScrapEngine::Audio::AudioSource::play() const
{
	alSourcePlay(source_);
}

void ScrapEngine::Audio::AudioSource::stop() const
{
	alSourceStop(source_);
}

void ScrapEngine::Audio::AudioSource::pause() const
{
	alSourcePause(source_);
}

void ScrapEngine::Audio::AudioSource::rewind() const
{
	alSourceRewind(source_);
}