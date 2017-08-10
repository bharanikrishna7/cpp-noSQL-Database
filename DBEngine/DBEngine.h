//////////////////////////////////////////////////////////////////
// DBEngine.h       - Defines DBEngine Class to hold DBElement  //
//                    Objects in an unordered_map and Perform   //
//                    various operations on them.               //
// Version          - 1.1                                       //
// Last Modified    - 08/06/2017                                //
// Language         - Visual C++, Visual Studio 2017            //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10         //
// Author           - Venkata Bharani Krishna Chekuri           //
// e-mail           - bharanikrishna7@gmail.com                 //
//////////////////////////////////////////////////////////////////
/*
 * INFORMATION
 * -----------
 * This package provides DBEngine class and methods to perform operations on it.
 * The DBEngine class holds DBElement Obects in an unordered_map. When inserting
 * the DBElement Objects a copy of the DBElement is created and the created copy
 * is Stored in the database instead of the Object in the Argument to protect the
 * users from Tampering the data.
 * 
 * DBEngine also provides Auto-Indexing using a separate unordered_map to hold tags
 * and the keys of DBElements which have those tags present in them. This allows 
 * usres to directly get all the Objects which have the specified tag.
 *
 *
 * PACKAGE OPERATIONS
 * ------------------
 * - void insertIndexTags(std::string key)
 * Helper Method To Index Database based on Tags when a New DBElement is inserted.
 *
 * - void deleteIndexTags(std::string key);
 * Helper Method to Index Database based on Tags when a DBElement is removed.
 *
 * - DBEngine(std::string owner);
 * Constructor with Owner as Argument.
 *
 * - size_t size();
 * Method to return the Number of Objects in the Database.
 *
 * - std::string getOwner()
 * Method to get the Owner.
 *
 * - std::string setOwner(std::string newOwner)
 * Method to set the Owner.
 *
 * - bool insert(std::string key, DBElement value)
 * Method to Insert DBElement into Database.
 *
 * - bool insert(std::string key, DBElement* value)
 * Method to Insert DBElement into Database.
 *
 * - bool update(std::string key, DBElement value)
 * Method to Update the DBElement Object associated with Specified Key in the Database.
 *
 * - bool update(std::string key, DBElement* value)
 * Method to Update the DBElement Object associated with Specified Key in the Database.
 *
 * - bool remove(std::string key)
 * Method to Remove DBElement Object associated with Specified Key in the Database. 
 *
 * - bool exists(std::string key)
 * Method to Check if Specified Key Exists in the Database.
 *
 * - bool addTag(std::string key, std::string tag)
 * Method to Add Tag to DBElement Object with Specified Key present in the Database.
 *
 * - bool removeTag(std::string key, std::string tag)
 * Method to Remove Tag to DBElement Object with Specified Key present in the Database.
 *
 * - std::string getData(std::string key)
 * Method to Get DBElement Object present in Database in a Nicely Formatted Manner using it's Key.
 *
 * - DBElement getDataRaw(std::string key)
 * Method to Get DBElement Object present in Database using it's Key.
 *
 * - bool updateData(std::string key, std::string data)
 * Method to Update the Data of DBElement Present in Database.
 *
 * - std::unordered_set<std::string> getKeysWithTag(std::string tag)
 * Method to return Key of DBElements present in Database which have a Specified Tag.
 *
 * - std::string show()
 * Method to Show all the DBElement Objects Present in the Database.
 *
 * - std::string show(std::unordered_set<std::string> keys)
 * Method to Show DBElements Objects present in the Database using Keys.
 *
 * - std::string showUsingTag(std::string tag)
 * Method to Show All DBElement Objects present in Database with Specified Tag.
 *
 *
 * REQUIRED FILES
 * --------------
 * DBElement.h, DBEElement.cpp, Utilities.h, Utilities.cpp
 *
 *
 * OTHER DEPENDENCIES
 * ------------------
 * Platform : Requires Visual C++
 *
 *
 * CHANGELOG
 * ---------
 * ver 1.0 : 08/05/2017
 * - First release.
 *
 * ver 1.1 : 08/06/2017
 * - Added Auto-Indexing and retrieval using Tags.
 *
 */
#ifndef DBENGINE_H
#define DBENGINE_H

#include "../DBElement/DBElement.h"

#include <unordered_map>

/// <summary>
/// noSQL Database Class which holds Data an unordered_map. 
/// The Key if of type String and Data if of type DBElement.
/// 
/// DBEngine also Auto-Indexes the Database objects using Tags
/// associated with each DBElement.
/// </summary>
class DBEngine {
private:
	std::string _dbOwner;															// Database Owner
	std::unordered_map<std::string, DBElement*> _dbMap;								// unordered_map to hold DBElements
	std::unordered_map<std::string, std::unordered_set<std::string>> _tagMap;		// unordered_map used to Auto-Indexing Database using Tags

	/* Helper Functions For Indexing Using Tags */
	void insertIndexTags(std::string key);
	void deleteIndexTags(std::string key);
public:
	/* Constructor */
	DBEngine(std::string owner);
	
	/* Destructor */
	~DBEngine();

	/* Member Functions */
	size_t size();
	std::string getOwner();
	std::string setOwner(std::string newOwner);
	bool insert(std::string key, DBElement value);
	bool insert(std::string key, DBElement* value);
	bool update(std::string key, DBElement value);
	bool update(std::string key, DBElement* value);
	bool remove(std::string key);
	bool exists(std::string key);
	bool addTag(std::string key, std::string tag);
	bool removeTag(std::string key, std::string tag);
	std::string getData(std::string key);
	DBElement getDataRaw(std::string key);
	bool updateData(std::string key, std::string data);
	std::unordered_set<std::string> getKeysWithTag(std::string tag);
	std::string show();
	std::string show(std::unordered_set<std::string> keys); 
	std::string showUsingTag(std::string tag);
};

#ifdef TEST_CREATE_DBENGINE
std::vector<std::pair<std::string, DBElement>> createDBObjects();
void insertIntoDBEngine(DBEngine * db);
#endif // TEST_CREATE_DBENGINE

#endif // !DBENGINE_H
