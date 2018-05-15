#ifndef PARSER_H
#define PARSER_H

#include <QVector>
#include <QString>

class Parser
{
public:
    void lets_pars(QString &text);
    QString get_message()const;

    Parser();
    virtual ~Parser();

private:
    QVector<int>  _error_commands_code;
    QString _message;
};

#endif // PARSER_H
