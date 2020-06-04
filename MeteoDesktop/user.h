#ifndef USER_H
#define USER_H

#include <string>
#include <QString>

using std::string;

class User
{
public:
    User();

    QString  get_login(void) const;
    void    set_login(QString const login);
    void    set_level_access(int const);
    int    get_level_access() const;

private:
    QString  login;
    int     level_access;
};

#endif // USER_H
