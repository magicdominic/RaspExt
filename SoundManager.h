#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>

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
