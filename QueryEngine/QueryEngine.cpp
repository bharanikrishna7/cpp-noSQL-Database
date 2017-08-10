/////////////////////////////////////////////////////////////
// QueryEngine.cpp  - Perform Client Requests on DBEngine. //
// Version          - 1.1                                  //
// Last Modified    - 08/09/2017                           //
// Language         - Visual C++, Visual Studio 2015       //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10    //
// Author           - Venkata Bharani Krishna Chekuri      //
// e-mail           - bharanikrishna7@gmail.com            //
/////////////////////////////////////////////////////////////
#include "QueryEngine.h"

using namespace QueryScanner;

/// <summary>
/// Function to Parse Query and Retrieve Query Arguments.
/// </summary>
/// <param name="query">Query which is to be Parsed</param>
/// <param name="verbose">Enable or Disable Verbose Mode (Debugging)</param>
/// <returns></returns>
std::unordered_map<char, std::string> QueryEngine::ParseQuery(const char* query, bool verbose) {
	Toker * toker = new Toker(verbose);
	return toker->Compute(query);
}

/// <summary>
/// Static Function to Perform a Query on DBEngine.
/// </summary>
/// <param name="db">DBEngine on which Query will be performed</param>
/// <param name="query">Query to be performed</param>
/// <param name="verbose">Enable or Disable Verbose Mode (Debugging)</param>
/// <returns>String describing the Status of Executed Query</returns>
std::string QueryEngine::ProcessQuery(DBEngine * db, std::string query, bool verbose) {
	/* Done to make sure that pop back token would not mess up the last token */
	query.push_back(' ');
	std::unordered_map<char, std::string> arguments = ParseQuery(query.c_str(), verbose);
	if (arguments.find('t') == arguments.end())
		return "Invalid Query Syntax. Query Type is Undefined.";
	if (arguments['t'] == "INSERT")
		return ProcessInsertQuery(db, arguments);
	if (arguments['t'] == "DELETE")
		return ProcessDeleteQuery(db, arguments);
	if (arguments['t'] == "UPDATE")
		return ProcessUpdateQuery(db, arguments);
	if (arguments['t'] == "SHOW")
		return ProcessShowQuery(db, arguments);
	return "Invalid Query Syntax. Given Query Type is Not Supported.";
}

/// <summary>
/// Static Function to Perform Insert Type Queries on DBEngine.
/// </summary>
/// <param name="db">DBEngine on which Query will be performed</param>
/// <param name="arguments">List of Parameters extracted from Query</param>
/// <returns>String describing the Status of Executed Query</returns>
std::string QueryEngine::ProcessInsertQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments) {
	if (arguments.find('k') == arguments.end() || arguments.find('v') == arguments.end())
		return "Invalid Query Syntax. Insert Query Requires both Key and Value Arguments.";
	if (arguments.find('o') != arguments.end() || arguments.find('o') != arguments.end())
		return "Invalid Query Syntax. Insert Query Should not contain Operation or Parameter Arguments.";
	DBElement * value = new DBElement(arguments['v']);
	if (db->insert(arguments['k'], value))
		return "Object Successfully inserted into Database.";
	return "An object with given key already exists in the Database.";
}

/// <summary>
/// Static Function to Perform Delete Type Queries on DBEngine.
/// </summary>
/// <param name="db">DBEngine on which Query will be performed</param>
/// <param name="arguments">List of Parameters extracted from Query</param>
/// <returns>String describing the Status of Executed Query</returns>
std::string QueryEngine::ProcessDeleteQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments) {
	if (arguments.find('k') == arguments.end())
		return "Invalid Query Syntax. Delete Query Required Key Argument.";
	if (arguments.find('v') != arguments.end() || arguments.find('o') != arguments.end()
		|| arguments.find('p') != arguments.end())
		return "Invalid Query Syntax. Delete Query Should not contain Value or Operation or Parameter Arguments.";
	if (db->remove(arguments['k']))
		return "Object with key successfully removed from the Database.";
	return "No Object with given key exists in the Database.";
}

/// <summary>
/// Helper Function to Decide the Sub Query Type for UPDATE and SHOW Queries.
/// </summary>
/// <param name="arguments">List of Parameters extracted from Query</param>
/// <returns>Integer Describing Sub Query Type</returns>
int QueryEngine::QueryHelper(std::unordered_map<char, std::string>& arguments) {
	bool key = false, value = false, operation = false, parameter = false;
	if (arguments.find('k') != arguments.end())
		key = true;
	if (arguments.find('v') != arguments.end())
		value = true;
	if (arguments.find('o') != arguments.end())
		operation = true;
	if (arguments.find('p') != arguments.end())
		parameter = true;

	/* Update Value */
	if (key && value && !operation & !parameter)
		return 1;
	/* Update Tags */
	if (key && !value && operation && parameter)
		return 2;
	/* Show Object with given Key */
	if (key && !value && !operation && !parameter)
		return 3;
	/* Show Objects which have specific Tag */
	if (!key && !value && operation && parameter)
		return 4;
	/* Show All Objects in the Database */
	if (!key && !value && !operation && !parameter)
		return 5;
	/* Invalid Sub Query Type */
	return -1;
}

/// <summary>
/// Static Function to Perform Update Type Queries on DBEngine.
/// </summary>
/// <param name="db">DBEngine on which Query will be performed</param>
/// <param name="arguments">List of Parameters extracted from Query</param>
/// <returns>String describing the Status of Executed Query</returns>
std::string QueryEngine::ProcessUpdateQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments) {
	int querySubType = QueryHelper(arguments);
	if (querySubType == 1) {
		if (db->updateData(arguments['k'], arguments['v']))
			return "Successfully updated the value associated with the given key.";
		return "No Object with given key exists in the Database.";
	}
	if (querySubType == 2) {
		if (arguments['o'] == "AddTag") {
			if (db->addTag(arguments['k'], arguments['p']))
				return "Successfully Added Tag to Object with given Key in Database.";
			return "No Object with given key exists in the Database.";
		}
		if (arguments['o'] == "RemoveTag") {
			if (db->removeTag(arguments['k'], arguments['p']))
				return "Successfully Removed Tag from Object with given Key in Database.";
			return "No Object with given key exists in the Database.";
		}
		return "Invalid Query Syntax. Invalid Operation Argument.";
	}
	return "Invalid Query Syntax.";
}

/// <summary>
/// Static Function to Perform Show Type Queries on DBEngine.
/// </summary>
/// <param name="db">DBEngine on which Query will be performed</param>
/// <param name="arguments">List of Parameters extracted from Query</param>
/// <returns>String describing the Status of Executed Query</returns>
std::string QueryEngine::ProcessShowQuery(DBEngine * db, std::unordered_map<char, std::string>& arguments) {
	int querySubType = QueryHelper(arguments);
	if (querySubType == 3) {
		return db->getData(arguments['k']);
	}
	if (querySubType == 4) {
		if (arguments['o'] != "ByTag")
			return "Invalid Query Syntax. Operation Not Defined for Show Query.";
		return db->showUsingTag(arguments['p']);
	}
	if (querySubType == 5) {
		return db->show();
	}
	return "Invalid Query Syntax.";
}


#ifdef TEST_QUERYENGINE

/* Include Utilities Namespace for StringHelper Functions */
#include "../Utilities/Utilities.h"

using namespace Utilities;

/// <summary>
/// Function to Test Show Type of Queries.
/// </summary>
/// <param name="db">DBEngine</param>
void TestShowQueries(DBEngine * db)
{
	StringHelper::Title("Test Show Type Query");
	StringHelper::Title("Show All Objects in Database", '~');
	std::string query = "-t SHOW";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - Shows all Objects in Database\n\n" << QueryEngine::ProcessQuery(db, query.c_str());

	StringHelper::Title("Show Object with Specified Key in Database", '~');
	query = "-t SHOW -k key0";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - Object Details\n\n" << QueryEngine::ProcessQuery(db, query.c_str());
	putline();

	StringHelper::Title("Show Object with Specified Tag in Database", '~');
	query = "-t SHOW -o ByTag -p Machine";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - List of Objects with Matching Tag\n\n" << QueryEngine::ProcessQuery(db, query.c_str());
}

/// <summary>
/// Function to Test Update Type of Queries.
/// </summary>
/// <param name="db">DBEngine</param>
void TestUpdateQueries(DBEngine * db) {
	StringHelper::Title("Test Update Type Query");
	StringHelper::Title("Update Value of Specified Key in Database", '~');
	std::cout << "\n Before Update Object.\n" << db->getData("key2");
	std::string query = "-t UPDATE -k key2 -v T-1000 ";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - Response : " << QueryEngine::ProcessQuery(db, query);
	std::cout << "\n\n After Update Object.\n" << db->getData("key2");
	putline();

	StringHelper::Title("Add Tag to a Specified Key in Database", '~');
	std::cout << "\n Before Add Tag to Object.\n" << db->getData("key2");
	query = "-t UPDATE -k key2 -o AddTag -p AI";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - Response : " << QueryEngine::ProcessQuery(db, query);
	std::cout << "\n\n After Add Tag to Object.\n" << db->getData("key2");
	putline();

	StringHelper::Title("Remove Tag from a Specified Key in Database", '~');
	std::cout << "\n Before Remove Tag from Object.\n" << db->getData("key2");
	query = "-t UPDATE -k key2 -o RemoveTag -p Data";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - Response : " << QueryEngine::ProcessQuery(db, query);
	std::cout << "\n\n After Remove Tag from Object.\n" << db->getData("key2");
	putline();
}

/// <summary>
/// Function to Test Queries.
/// </summary>
/// <param name="db">DBEngine</param>
void TestQueries(DBEngine * db) {
	StringHelper::Title("Test Insert Type Query");
	std::string query = "-t INSERT -k key6 -v Wyatt";
	std::cout << "\n Query : \"" << query << "\"";
	std::cout << "\n - Response : " << QueryEngine::ProcessQuery(db, query.c_str());
	std::cout << "\n\n" << db->getData("key6");
	putline();

	StringHelper::Title("Test Delete Type Query ");
	std::cout << "\n Object With \"key6\" in Database Before Executing Query ? " << db->exists("key6");
	query = "-t DELETE -k key6";
	std::cout << "\n\n Query : \"" << query << "\"";
	std::cout << "\n - Response : " << QueryEngine::ProcessQuery(db, query.c_str());
	std::cout << "\n\n Object With \"key6\" in Database After Executing Query ? " << db->exists("key6") << "\n";
	putline();

	TestShowQueries(db);
	TestUpdateQueries(db);

}

/// <summary>
/// Function to Test Query Engine Package.
/// </summary>
/// <param name="argc">Argument Count</param>
/// <param name="argv">Arguments</param>
/// <returns></returns>
int main(int argc, char* argv[]) {
	Timer time;
	time.StartClock();

	StringHelper::Title("TESTING QUERY ENGINE PACKAGE", '=');
	StringHelper::Title("Create DBEngine");
	std::cout << "\n Creating DBEngine with owner : \"anonymous\"" << std::endl;
	DBEngine * db = new DBEngine("anonymous");
	putline();

	insertIntoDBEngine(db);
	
	StringHelper::Title("Show Objects in DBEngine");
	std::cout << "\n" << db->show();
	putline();

	TestQueries(db);

	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n ";
	return 0;
}

#endif // TEST_QUERYENGINE
