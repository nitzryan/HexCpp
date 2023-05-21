#pragma once

#include "sqlite3.h"
#include "../CustomVariant.h"
#include <typeindex>
#include <mutex>

class Database
{
public:
    static Database& GetDatabase() {
        static Database instance;
        return instance;
    }

    void LoadFile(const char* filename);
    std::vector<CustVar::varList> GetData(std::string query, std::vector<std::type_index> types);
    void InsertData(std::string query, std::vector<CustVar::varList> data);
    void StartTransaction();
    void CompleteTransaction();
private:
    Database();
    Database(const Database&) = delete;
    Database& operator = (const Database&) = delete;
    std::mutex mutex;

    // SQLite required variables
    sqlite3* db = nullptr;
    int rc = 0;
    char* zErrMsg = nullptr;

    // Variables used for callback function
    static int Callback(void* param, int argc, char** argv, char** azColName);
    static std::vector<CustVar::varList> callbackResults;
    static std::vector<std::type_index> callbackTypes; // How the data should be interpreted
};
