#ifndef ACTIONMUSIC_H
#define ACTIONMUSIC_H

#include "script/Action.h"

class SoundManager;

/**
 * @brief The ActionMusic class is the action which controls the music. Music can only be started and stopped.
 * Pausing and continuing is not possible. Also there can only be one song playing.
 * If a second one is started, this stops the first one first and then starts the second one.
 */
class ActionMusic : public Action
{
public:
    enum MusicAction
    {
        Play = 0,
        Stop = 1
    };

    static Action* load(QDomElement* root);
    virtual QDomElement save(QDomElement* root, QDomDocument* document);

    void init(ConfigManager* config);
    void deinit();

    bool execute(unsigned int step);

    Type getType() const { return Music;}
    std::string getDescription() const;

    MusicAction getMusicAction() const { return m_action;}
    void setMusicAction(MusicAction action) { m_action = action;}

    std::string getFilename() const { return m_filename;}
    void setFilename(std::string name) { m_filename = name;}

private:
    SoundManager* m_soundManager;

    MusicAction m_action;
    std::string m_filename;
};

#endif // ACTIONMUSIC_H
