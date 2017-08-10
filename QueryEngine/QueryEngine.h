/////////////////////////////////////////////////////////////
// QueryEngine.h    - Perform Client Requests on DBEngine. //
// Version          - 1.1                                  //
// Last Modified    - 08/09/2017                           //
// Language         - Visual C++, Visual Studio 2015       //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10    //
// Author           - Venkata Bharani Krishna Chekuri      //
// e-mail           - bharanikrishna7@gmail.com            //
/////////////////////////////////////////////////////////////
/*
 * INTRODUCTION
 * ------------
 * This package Provides Static Function to Perform Queries on Database.
 *
 *
 * PACKAGE OPERATIONS
 * ------------------
 * - std::string ProcessQuery(DBEngine * db, std::string query, bool verbose)
 * Function to Parse Query, Perform Operation on DBEngine and Finally return
 * Response to the Client.
 *
 * DEPENDANT FILES
 * ---------------
 * QueryParser.h, QueryParser.cpp, DBEngine.h, DBEngine.cpp,
 * DBElement.h, DBElement.cpp, Utilities.h, Utilities.cpp
 *
 *
 * CHANGELOG
 * ---------
 * ver 1.0 : 08/07/2017
 * - First Release.
 *
 * ver 1.1 : 08/09/2017
 * - Fixed a Bug Which would cause last Argument to be incorrectly processed (It would
 * be missing the last character).
 *
 * 
 * TO-DO
 * -----
 * Change Process Query to Take Query Arguments as Parameter once Control Node 
 * Package is Created.
 *
 */

#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <unordered_map>

#include "QueryParser.h"
#include "../DBEngine/DBEngine.h"
#include "../DBElement/DBElement.h"

/// <summary>
/// Class to Execute Queries on DBEngine. Uses Builder Pattern to Build the Query Processor
/// based on Query Type.
/// </summary>
class QueryEngine {
	static int QueryHelper(std::unordered_map<char, std::string>& arguments);
	static std::unordered_map<char, std::string> ParseQuery(const char* query, bool verbose);
	static std::string ProcessShowQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments);
	static std::string ProcessInsertQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments);
	static std::string ProcessDeleteQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments);
	static std::string ProcessUpdateQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments);
public:
	static std::string ProcessQuery(DBEngine * db, std::string query, bool verbose = false);
};

#endif // QUERYENGINE_H
