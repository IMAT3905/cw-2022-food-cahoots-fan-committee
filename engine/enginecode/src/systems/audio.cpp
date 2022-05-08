#include "engine_pch.h"
#include "systems/audio.h"

namespace Engine
{
	bool AudioSystem::addSound(const std::string& tag, const std::string& filepath)
	{
		m_sounds[tag] = SoLoud::Wav();
		auto result = m_sounds[tag].load(filepath.c_str());
		return result == 0;
	}
	bool AudioSystem::playSound(const std::string& tag, float volume, float pan, float playSpeed)
	{
		auto sound = m_sounds.find(tag);
		if (sound != m_sounds.end())
		{
			int handle = m_audioSystem.play(sound->second);
			m_audioSystem.setVolume(handle, volume);
			m_audioSystem.setPan(handle, pan);
			m_audioSystem.setRelativePlaySpeed(handle, playSpeed);
			return true;
		}

		return false;
	}
	bool AudioSystem::setSoundLooping(const std::string& tag, bool looping)
	{
		auto sound = m_sounds.find(tag);
		if (sound != m_sounds.end())
		{
			sound->second.setLooping(looping);
			return true;
		}
		return false;
	}

	void AudioSystem::StopAllSounds() {
		m_audioSystem.stopAll();
	}
}