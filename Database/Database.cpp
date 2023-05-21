#include "Database.h"

std::vector<CustVar::varList> Database::callbackResults = {};
std::vector<std::type_index> Database::callbackTypes = {};

void Database::LoadFile(const char *filename)
{
    if (db != nullptr)
    {
        sqlite3_close(db);
    }

    rc = sqlite3_open(filename, &db);
    if (rc)
    {
        throw "Database File Not Found";
    }
    zErrMsg = nullptr;

}

std::vector<CustVar::varList> Database::GetData(std::string query, std::vector<std::type_index> types)
{
    std::unique_lock<std::mutex> lock(mutex);
    callbackTypes = types;
    callbackResults.clear();

    rc = sqlite3_exec(db, query.c_str(), Database::Callback, nullptr, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        std::string errString = "Database::GetData SQL ERROR: ";
        errString.append(zErrMsg);
        throw errString;
    }

    return std::vector<CustVar::varList>(Database::callbackResults); // Create deep copy of vector
}

// Query should have '$n' (without '') in every place that data should be placed
// n should be 0 indexed
// All instances of udt::varList should have the same number of elements
void Database::InsertData(std::string query, std::vector<CustVar::varList> data)
{
    // Generate query for every element
    std::vector<std::string> queryList(data.size());
    for (size_t i = 0; i < data.size(); i++)
    {
        std::string thisQuery = query;
        for (size_t j = 0; j < data[i].size(); j++)
        {
            std::string substring = "$" + std::to_string(j);
            std::size_t subLoc = thisQuery.find(substring);
            if (subLoc != std::string::npos)
            {
                thisQuery.replace(subLoc, substring.length(), CustVar::VarToString(data.at(i).at(j)));
            }
            else
            {
                throw "Query did not specify where to insert variable";
            }
        }
        queryList.at(i) = thisQuery;
    }

    // Log results to database
    std::unique_lock<std::mutex> lock(mutex);

    for (auto& q : queryList)
    {
        sqlite3_stmt* statement = NULL;
        sqlite3_prepare_v2(db, q.c_str(), -1, &statement, NULL);
        int out = sqlite3_step(statement);
        if (out != SQLITE_OK)
        {
            if (statement != NULL)
            {
                sqlite3_finalize(statement);
            }
            else
            {
                throw "SQLite Statement was NULL";
            }
        }
        else
        {
            throw "SQLite Statement was not OK";
        }
    }

}

void Database::StartTransaction()
{
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void Database::CompleteTransaction()
{
    sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
}

Database::Database()
{

}

template<typename T> std::string GetTypeName()
{
    return typeid(T).name();
}

int Database::Callback(void*, int argc, char** argv, char**)
{
    if (argc != (int)Database::callbackTypes.size())
        throw "Invalid Database Callback Parameter Count";

    CustVar::varList singleResult;
    for (int i = 0; i < argc; i++)
    {
        std::string paramName = callbackTypes.at(i).name();
        if (paramName == GetTypeName<int>())
        {
            singleResult.push_back((int)atoi(argv[i]));
        }
        else if (paramName == GetTypeName<uint32_t>())
        {
            singleResult.push_back((uint32_t)atoi(argv[i]));
        }
        else if (paramName == GetTypeName<bool>())
        {
            singleResult.push_back((bool)atoi(argv[i]));
        }
        else if (paramName == GetTypeName<float>())
        {
            singleResult.push_back((float)atof(argv[i]));
        }
        else if (paramName == GetTypeName<double>())
        {
            singleResult.push_back((double)atof(argv[i]));
        }
        else if (paramName == GetTypeName<std::string>())
        {
            singleResult.push_back((std::string)argv[i]);
        }
        else
        {
            throw "Unknown Database Callback Parameter Type";
        }
    }

    callbackResults.push_back(singleResult);

    return 0;
}
