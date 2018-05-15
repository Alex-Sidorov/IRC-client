#include "Parser.h"

void Parser::lets_pars(QString &text)
{
    if(text.isEmpty())
    {
        return;
    }

    int space = text.indexOf(" ");
    if(space == -1)
    {
        return;
    }

    _message.clear();

    if(text[0]!=':')
    {
        QString command = text.mid(0,space);
        if(command== "PONG")
        {
            int begin_trails = text.indexOf(":");
            QString trails = text.mid(begin_trails);
            _message=command +" " + trails;
            return;
        }
        else
        {
            _message+=text;
            return;
        }
    }

    QString prefix = text.mid(1,space-1);
    if(prefix.indexOf("!")==-1 && prefix.indexOf("@")==-1)
    {
        QString temp = text.mid(space+1);
        QString command = text.mid(space+1,temp.indexOf(' '));
        if(!command.isEmpty())
        {
            if(_error_commands_code.indexOf(command.toInt())!=-1)
            {
                temp = text.mid(1);
                int begin_trails = temp.indexOf(":");
                QString trails = temp.mid(begin_trails);
                _message=prefix +" :" + trails;
                return;
            }

        }

        int begin_trails = temp.indexOf(':');
        QString trails = temp.mid(begin_trails+1);

        if(command == "NOTICE")
        {
           _message+=trails;
           return;
        }

        QString parametrs = temp.mid(command.size(),begin_trails-1-command.size());

        _message = parametrs + " " + trails;
        return;
    }

    QString nick = prefix.mid(0,prefix.indexOf("!"));
    QString temp = prefix.mid(prefix.indexOf("!")+1);
    QString user = temp.mid(0,temp.indexOf("@"));
    QString host = prefix.mid(prefix.indexOf("@")+1,prefix.indexOf(" "));
    temp = text.mid(text.indexOf(" ")+1);
    QString command = temp.mid(0,temp.indexOf(" "));
    temp = text.mid(1,text.size());
    QString trails = temp.mid(temp.indexOf(":")+1);

    _message =command + " NICK:" +nick + " USER:" + user + " HOST:" + host + " " + trails;


}

QString Parser::get_message()const
{
    return _message;
}

Parser::Parser()
{
    _error_commands_code.reserve(43);

    _error_commands_code.push_back(401);
    _error_commands_code.push_back(402);
    _error_commands_code.push_back(403);
    _error_commands_code.push_back(404);
    _error_commands_code.push_back(405);
    _error_commands_code.push_back(406);
    _error_commands_code.push_back(407);
    _error_commands_code.push_back(409);
    _error_commands_code.push_back(411);
    _error_commands_code.push_back(412);
    _error_commands_code.push_back(413);
    _error_commands_code.push_back(414);
    _error_commands_code.push_back(421);
    _error_commands_code.push_back(422);
    _error_commands_code.push_back(423);
    _error_commands_code.push_back(424);
    _error_commands_code.push_back(431);
    _error_commands_code.push_back(432);
    _error_commands_code.push_back(433);
    _error_commands_code.push_back(436);
    _error_commands_code.push_back(441);
    _error_commands_code.push_back(442);
    _error_commands_code.push_back(443);
    _error_commands_code.push_back(444);
    _error_commands_code.push_back(445);
    _error_commands_code.push_back(446);
    _error_commands_code.push_back(451);
    _error_commands_code.push_back(461);
    _error_commands_code.push_back(462);
    _error_commands_code.push_back(463);
    _error_commands_code.push_back(464);
    _error_commands_code.push_back(465);
    _error_commands_code.push_back(467);
    _error_commands_code.push_back(471);
    _error_commands_code.push_back(472);
    _error_commands_code.push_back(473);
    _error_commands_code.push_back(474);
    _error_commands_code.push_back(475);
    _error_commands_code.push_back(481);
    _error_commands_code.push_back(482);
    _error_commands_code.push_back(483);
    _error_commands_code.push_back(491);
    _error_commands_code.push_back(501);
    _error_commands_code.push_back(502);
}

Parser::~Parser()
{
}
