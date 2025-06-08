#ifndef TEAMMANAGER_H
#define TEAMMANAGER_H
#include "team.h"

#include <string>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace TeamManager {

    bool createTeam(const Team& team);
    Team getTeam(const std::string& name);
    Team getTeam(uint32_t id);
    bool updateTeam(const Team& team);
    bool deleteTeam(const Team& team);
    bool deleteTeam(uint32_t id);

    std::vector<Team> getAllTeams();
    std::vector<Team> getTeamsForUser(uint32_t userId);
    Team getTeamForUser(uint32_t userId);


}

#endif // TEAMMANAGER_H
