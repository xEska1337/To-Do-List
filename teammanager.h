#ifndef TEAMMANAGER_H
#define TEAMMANAGER_H
#include "team.h"

#include <string>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace TeamManager {

    /**
     * Creates Team in a database
     * @param team Team to create
     * @return True if success, false if there was any error
     */
    bool createTeam(const Team& team);
    /**
     * Gets Team from a database identifying it by name and returns as Team's object
     * @param name Team's name
     * @return Team's object, if Team->id equals 0, then Team doesn't exist, or there was an error
     */
    Team getTeam(const std::string& name);
    /**
     * Gets Team from a database identifying it by ID and returns as Team's object
     * @param id Team's ID
     * @return Team's object, if Team->id equals 0, then Team doesn't exist, or there was an error
     */
    Team getTeam(uint32_t id);
    /**
     * Updates Team in a database
     * @param team Team to update
     * @return True if success, false if there was any error
     */
    bool updateTeam(const Team& team);
    /**
     * Deletes Team from a database
     * @param team Team to delete
     * @return True if success, false if Team doesn't exist or there was any error
     */
    bool deleteTeam(const Team& team);
    /**
     * Deletes Team using his ID from a database
     * @param id ID of a Team to delete
     * @return True if success, false if Team doesn't exist or there was any error
     */
    bool deleteTeam(uint32_t id);

    /**
     * Returns all Teams from a database
     * @return Vector of all Teams from a database
     */
    std::vector<Team> getAllTeams();
    /**
     * Returns all Teams that the User belongs to from a database
     * @return Vector of all Teams that the User belongs to
     */
    std::vector<Team> getTeamsForUser(uint32_t userId);
    /**
     * Returns first User's Team from a database
     * @return First User's Team
     */
    Team getTeamForUser(uint32_t userId);


}

#endif // TEAMMANAGER_H
