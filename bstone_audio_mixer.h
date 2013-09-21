#ifndef BSTONE_AUDIO_MIXER_H
#define BSTONE_AUDIO_MIXER_H


#include <deque>
#include <list>
#include <vector>

#include "SDL.h"

#include "bstone_audio_decoder.h"


namespace bstone {


enum ActorChannel {
    AC_VOICE,
    AC_WEAPON,
    AC_ITEM,
    AC_WALL_HIT,
}; // enum ActorChannel

enum SoundType {
    ST_NONE,
    ST_ADLIB_MUSIC,
    ST_ADLIB_SFX,
    ST_PCM,
}; // enum SoundType


class AudioMixer {
public:
    AudioMixer();

    ~AudioMixer();

    bool initialize(
        int dst_rate);

    void uninitialize();

    bool is_initialized() const;

    bool play_adlib_music(
        const void* data,
        int data_size);

    // Negative index of an actor defines a non-positional sound.
    bool play_adlib_sound(
        const void* data,
        int data_size,
        int actor_index = -1,
        ActorChannel actor_channel = AC_VOICE);

    // Negative index of an actor defines a non-positional sound.
    bool play_pcm_sound(
        const void* data,
        int data_size,
        int actor_index = -1,
        ActorChannel actor_channel = AC_VOICE);

    bool stop_music();

    bool set_mute(
        bool value);

    bool is_music_playing() const;

    bool is_non_music_playing() const;

    bool is_anything_playing() const;

    static int get_min_rate();

    static int get_max_channels();

private:
    typedef int16_t Sample;
    typedef std::vector<Sample> Samples;

    typedef float MixSample;
    typedef std::vector<MixSample> MixSamples;

    class CacheItem {
    public:
        bool is_active;
        bool is_invalid;
        SoundType sound_type;
        int samples_count;
        int decoded_count;
        Samples samples;
        AudioDecoder* decoder;

        CacheItem();

        CacheItem(
            const CacheItem& that);

        ~CacheItem();

        CacheItem& operator=(
            const CacheItem& that);

        bool is_decoded() const;
    }; // class CacheItem

    typedef std::vector<CacheItem> Cache;
    typedef Cache::iterator CacheIt;

    class Sound {
    public:
        SoundType type;
        CacheItem* cache;
        int decode_offset;
        int actor_index;
        ActorChannel actor_channel;
    }; // class Sound

    typedef std::list<Sound> Sounds;
    typedef Sounds::iterator SoundsIt;
    typedef Sounds::const_iterator SoundsCIt;

    enum CommandType {
        CMD_PLAY,
        CMD_STOP_MUSIC,
    }; // enum CommandType

    class Command {
    public:
        CommandType command;
        Sound sound;
        const void* data;
        int data_size;
    }; // class Command

    typedef std::deque<Command> PlayCommands;
    typedef PlayCommands::iterator PlayCommandsIt;
    typedef PlayCommands::const_iterator PlayCommandsCIt;

    bool is_initialized_;
    int dst_rate_;
    SDL_AudioDeviceID device_id_;
    SDL_mutex* mutex_;
    SDL_Thread* thread_;
    int mix_samples_count_;
    Samples buffer_;
    MixSamples mix_buffer_;
    volatile bool is_data_available_;
    volatile bool quit_thread_;
    Sounds sounds_;
    PlayCommands commands_;
    PlayCommands commands_queue_;
    bool mute_;
    Cache adlib_music_cache_;
    Cache adlib_sfx_cache_;
    Cache pcm_cache_;

    void callback(
        Uint8* dst_data,
        int dst_length);

    void mix();

    void mix_samples();

    void handle_commands();

    void handle_play_command(
        const Command& command);

    void handle_stop_music_command();

    bool initialize_cache_item(
        const Command& command,
        CacheItem& cache_item);

    bool decode_sound(
        const Sound& sound);

    bool play_sound(
        SoundType sound_type,
        const void* data,
        int data_size,
        int actor_index = -1,
        ActorChannel actor_channel = AC_VOICE);

    CacheItem* get_cache_item(
        SoundType sound_type,
        int sound_index);

    static void callback_proxy(
        void* user_data,
        Uint8* dst_data,
        int dst_length);

    static int mix_proxy(
        void* user_data);

    static int calculate_mix_samples_count(
        int dst_rate);

    static AudioDecoder* create_decoder_by_sound_type(
        SoundType sound_type);

    static bool is_sound_type_valid(
        SoundType sound_type);

    static bool is_sound_index_valid(
        int sound_index,
        SoundType sound_type);

    AudioMixer(
        const AudioMixer& that);

    AudioMixer& operator=(
        const AudioMixer& that);
}; // class AudioMixer


} // namespace bstone


#endif // BSTONE_AUDIO_MIXER_H