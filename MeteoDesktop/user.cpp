#include "user.h"

User::User()
{

}

QString User::get_login(void) const { return this->login; }

void User::set_login(const QString login) { this->login = login; }

int User::get_level_access() const { return this->level_access; }

void    User::set_level_access(int const level)
{
    this->level_access = level;
}

