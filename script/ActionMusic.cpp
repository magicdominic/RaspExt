
#include "script/ActionMusic.h"
#include "ConfigManager.h"
#include "SoundManager.h"
#include "util/Debug.h"

Action* ActionMusic::load(QDomElement* root)
{
    ActionMusic* action = new ActionMusic();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if(elem.tagName().toLower().compare("filename") == 0)
        {
            action->m_filename = elem.text().toStdString();
        }
        if(elem.tagName().toLower().compare("musicaction") == 0)
        {
            if( elem.text().toLower().compare("play") == 0)
                action->m_action = Play;
            else if( elem.text().toLower().compare("stop") == 0)
                action->m_action = Stop;
        }

        elem = elem.nextSiblingElement();
    }

    if(action->m_action == Play && action->m_filename.empty())
    {
        delete action;
        return NULL;
    }

    return action;
}

QDomElement ActionMusic::save(QDomElement* root, QDomDocument* document)
{
    QDomElement action = Action::save(root, document);

    QDomElement type = document->createElement("type");
    QDomText typeText = document->createTextNode("Music");
    type.appendChild(typeText);

    action.appendChild(type);

    QDomElement name = document->createElement("Filename");
    QDomText nameText = document->createTextNode( QString::fromStdString( m_filename ) );
    name.appendChild(nameText);

    action.appendChild(name);

    QDomElement musicAction = document->createElement("MusicAction");
    QDomText musicActionText = document->createTextNode( m_action == Play ? "Play" : "Stop" );
    musicAction.appendChild(musicActionText);

    action.appendChild(musicAction);

    return action;
}

void ActionMusic::init(ConfigManager *config)
{
    m_soundManager = config->getSoundManager();
}

void ActionMusic::deinit()
{
    m_soundManager = NULL;
}

bool ActionMusic::execute(unsigned int start)
{
    if(m_action == Play)
        m_soundManager->play(m_filename);
    else
        m_soundManager->stop();

    return true;
}

std::string ActionMusic::getDescription() const
{
    std::string str;

    if( m_action == Play)
    {
        str = "Play music ";
        str.append( m_filename );
    }
    else
    {
        str = "Stop music";
    }

    return str;
}
