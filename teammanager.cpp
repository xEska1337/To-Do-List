#include "teammanager.h"

using namespace TeamManager;

/**
 * Creates Team in database
 * @param team Team to create
 * @return True if success, false if there was any error
 */
bool TeamManager::createTeam(const Team &team) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }
    // Check if team name already exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM teams WHERE name = ?");
    checkQuery.addBindValue(QString::fromStdString(team.getName()));

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check team name: " + checkQuery.lastError().text());
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Error", "Team name already exists!");
        return false;
    }

    // Insert new user
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO teams (name, password, members) VALUES (?, ?, ?)");
    insertQuery.addBindValue(QString::fromStdString(team.getName()));
    insertQuery.addBindValue(QString::number(team.getPassword()));
    std::string membersString;
    if (!team.getMembers().empty()) {
        membersString = std::to_string(team.getMembers()[0]);
        for (int i = 1; i < team.getMembers().size(); i++) {
            membersString += ";" + std::to_string(team.getMembers()[i]);
        }
    } else {
        membersString = "";
    }

    insertQuery.addBindValue(QString::fromStdString(membersString));

    if (!insertQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to create team: " + insertQuery.lastError().text());
        return false;
    }
    return true;
}

/**
 * Gets Team from a database identifying it by name and returns as Team's object
 * @param name Team's name
 * @return Team's object, if Team->id equals 0, then Team doesn't exist, or there was an error
 */
Team TeamManager::getTeam(const std::string &name) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return Team(0, "", 0);
    }

    QSqlQuery getQuery(db);
    getQuery.prepare("SELECT * FROM teams WHERE name = ?");
    getQuery.addBindValue(QString::fromStdString(name));

    if (!getQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check team name: " + getQuery.lastError().text());
        return Team(0, "", 0);
    }

    getQuery.next();
    if (getQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "Team with that name doesn't exist!");
        return Team(0, "", 0);
    }

    std::vector<uint32_t> members;

    std::string membersString = getQuery.value("members").toString().toStdString();

    std::string memberId = "";
    for (uint64_t i = 0; i < membersString.size(); i++) {
        if (membersString[i] == ';') {
            if (memberId != "") {
                members.push_back(std::stoi(memberId));
            }
            memberId = "";
            continue;
        }
        memberId += membersString[i];
    }
    if (memberId != "") {
        members.push_back(std::stoi(memberId));
    }

    Team team{
        getQuery.value("id").toUInt(),
        getQuery.value("name").toString().toStdString(),
        getQuery.value("password").toULongLong(),
        members
    };

    return team;
}

/**
 * Gets Team from a database identifying it by ID and returns as Team's object
 * @param id Team's ID
 * @return Team's object, if Team->id equals 0, then Team doesn't exist, or there was an error
 */
Team TeamManager::getTeam(uint32_t id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return Team(0, "", 0);
    }

    QSqlQuery getQuery(db);
    getQuery.prepare("SELECT * FROM teams WHERE id = ?");
    getQuery.addBindValue(id);

    if (!getQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check team id: " + getQuery.lastError().text());
        return Team(0, "", 0);
    }

    getQuery.next();
    if (getQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "Team with that id doesn't exist!");
        return Team(0, "", 0);
    }

    std::vector<uint32_t> members;

    std::string membersString = getQuery.value("members").toString().toStdString();

    std::string memberId = "";
    for (uint64_t i = 0; i < membersString.size(); i++) {
        if (membersString[i] == ';') {
            if (memberId != "") {
                members.push_back(std::stoi(memberId));
            }
            memberId = "";
            continue;
        }
        memberId += membersString[i];
    }
    if (memberId != "") {
        members.push_back(std::stoi(memberId));
    }

    Team team{
        getQuery.value("id").toUInt(),
        getQuery.value("name").toString().toStdString(),
        getQuery.value("password").toULongLong(),
        members
    };

    return team;
}

/**
 * Updates Team in a database
 * @param team Team to update
 * @return True if success, false if there was any error
 */
bool TeamManager::updateTeam(const Team &team) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }

    // Check if team exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM teams WHERE id = ?");
    checkQuery.addBindValue(team.getId());

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check id: " + checkQuery.lastError().text());
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "Team doesn't exist!");
        return false;
    }

    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE teams SET name=?, password=?, members=? WHERE id=?");
    updateQuery.addBindValue(QString::fromStdString(team.getName()));
    updateQuery.addBindValue(team.getPassword());
    std::string membersString;
    if (!team.getMembers().empty()) {
        membersString = std::to_string(team.getMembers()[0]);
        for (int i = 1; i < team.getMembers().size(); i++) {
            membersString += ";" + std::to_string(team.getMembers()[i]);
        }
    } else {
        membersString = "";
    }
    updateQuery.addBindValue(QString::fromStdString(membersString));
    updateQuery.addBindValue(team.getId());
    if (!updateQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to update team: " + updateQuery.lastError().text());
        return false;
    }
    return true;
}

/**
 * Deletes Team from a database
 * @param team Team to delete
 * @return True if success, false if Team doesn't exist or there was any error
 */
bool TeamManager::deleteTeam(const Team &team) {
    return TeamManager::deleteTeam(team.getId());
}

/**
 * Deletes Team using his ID from a database
 * @param id ID of a Team to delete
 * @return True if success, false if Team doesn't exist or there was any error
 */
bool TeamManager::deleteTeam(uint32_t id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }

    // Check if team exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM teams WHERE id = ?");
    checkQuery.addBindValue(id);

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check id: " + checkQuery.lastError().text());
        return false;
    }

    if (!checkQuery.next()) {
        QMessageBox::warning(nullptr, "Error", "Team doesn't exist!");
        return false;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM teams WHERE id = ?");
    deleteQuery.addBindValue(id);
    if (!deleteQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to delete team: " + deleteQuery.lastError().text());
        return false;
    }

    return true;
}

/**
 * Returns all Teams from a database
 * @return Vector of all Teams from a database
 */
std::vector<Team> TeamManager::getAllTeams() {
    std::vector<Team> teams;
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return teams;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM teams");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to get teams: " + query.lastError().text());
        return teams;
    }

    while (query.next()) {
        std::vector<uint32_t> members;
        std::string membersString = query.value("members").toString().toStdString();

        // Parse members string
        std::string memberId = "";
        for (uint64_t i = 0; i < membersString.size(); i++) {
            if (membersString[i] == ';') {
                if (memberId != "") {
                    members.push_back(std::stoi(memberId));
                }
                memberId = "";
                continue;
            }
            memberId += membersString[i];
        }
        if (memberId != "") {
            members.push_back(std::stoi(memberId));
        }

        Team team{
            query.value("id").toUInt(),
            query.value("name").toString().toStdString(),
            query.value("password").toULongLong(),
            members
        };
        teams.push_back(team);
    }

    return teams;
}

/**
 * Returns all Teams that the User belongs to from a database
 * @return Vector of all Teams that the User belongs to
 */
std::vector<Team> TeamManager::getTeamsForUser(uint32_t userId) {
    std::vector<Team> userTeams;
    std::vector<Team> allTeams = getAllTeams();

    for (const auto& team : allTeams) {
        if (team.containsUser(userId)) {
            userTeams.push_back(team);
        }
    }

    return userTeams;
}

/**
 * Returns first User's Team from a database
 * @return First User's Team
 */
Team TeamManager::getTeamForUser(uint32_t userId) {
    std::vector<Team> userTeams = getTeamsForUser(userId);
    if (!userTeams.empty()) {
        return userTeams[0];
    }
    return Team(0, "", 0);
}
