#pragma once

namespace sfx
{
	const unsigned int ChannelsMax = 32;

	// 0-31
	enum class Sound : unsigned int
	{
		Button_Positive = 0,
		Button_Negative,
		Button_Hover,
		SOUND_MAX = sfx::ChannelsMax - 1u
	};

	// 0-31
	enum class Music : unsigned int
	{
		Menu = 0,
		Ambient,
		MUSIC_MAX = sfx::ChannelsMax - 1u
	};

	class SoundEngine
	{
	private:

	// System:

		FMOD::System* soundSystem;		

	// Sounds:

		// SFX
		FMOD::Sound* sounds[sfx::ChannelsMax];

		// Music
		FMOD::Sound* music[sfx::ChannelsMax];

	// Channels:

		// SFX
		FMOD::Channel* soundChannels[sfx::ChannelsMax];
		FMOD::ChannelGroup* sfxGroup;

		// Music
		FMOD::Channel* musicChannels[sfx::ChannelsMax];		
		FMOD::ChannelGroup* musicGroup;

		// Master channel group
		FMOD::ChannelGroup* masterGroup;

	// Variables:

		// SFX
		float sfxVolume;

		// Music
		float musicVolume;
		float fadeStep;
		float fadeRate;
		std::map<const sfx::Music, const char*> musicFiles;

	// private: Functions:

		void initVariables();

		void initSoundSystem();

	public:

	// Constructors and Destructor:

		SoundEngine();
		virtual ~SoundEngine();

	// Accessors:

		const float& getSfxVolume() const;

		const float& getMusicVolume() const;
		const float& getFadeRate() const;
		const bool isPlaying(const sfx::Music name) const;
		const bool isMusicPlaying() const;
		const bool isPaused(const sfx::Music name) const;
		const bool isMusicPaused() const;

	// Modifiers:

		void increaseSfxVolume(const float& value);

		void increaseMusicVolume(const float& value);
		void fade(const float& fade_step);

	// Functions:

		// Should use '\\' instead of '/' in the file path and put this file to the .exe location,
		// to be able to play the game without Visual Studio.
		// @param	const unsigned int& index		Index associated with sound file.
		// @param	const char* file_path		Actual file path, 256 symbols maximum.
		void initSound(const sfx::Sound name, const char* file_path);
		void playSound(const sfx::Sound name);

		// Should use '\\' instead of '/' in the file path and put this file to the .exe location,
		// to be able to play the game without Visual Studio.
		// @param	const unsigned int& index		Index associated with music file.
		// @param	const char* file_path		Actual file path.
		void initMusic(const sfx::Music name, const char* file_path);
		void playMusic(const sfx::Music name, const bool looped = true, const bool paused = false);

		void pause(const sfx::Music name);
		void unpause(const sfx::Music name);
		void stop(const sfx::Music name);
		void setPosition(const sfx::Music name, const unsigned int milliseconds);
		void fadeIn(const float& dt);
		void fadeOut(const float& dt);

		void updateSfxVolume();
		void updateMusicVolume();
		void update(const float& dt);
	};
};
