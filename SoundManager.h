#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>

/**
 * @brief The SoundManager class implements a simple interface for playing music.
 * It uses sdl_mixer as underlying library, therefore sdl_mixer is a dependency of this project.
 */
class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    void play(std::string filename, int loop = 0);
    void stop();

private:
    Mix_Music* m_music;
};

#endif // SOUNDMANAGER_H
