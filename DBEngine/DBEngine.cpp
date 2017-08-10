//////////////////////////////////////////////////////////////////
// DBEngine.cpp     - Defines DBEngine Class to hold DBElement  //
//                    Objects in an unordered_map and Perform   //
//                    various operations on them.               //
// Version          - 1.1                                       //
// Last Modified    - 08/06/2017                                //
// Language         - Visual C++, Visual Studio 2015            //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10         //
// Author           - Venkata Bharani Krishna Chekuri           //
// e-mail           - bharanikrishna7@gmail.com                 //
//////////////////////////////////////////////////////////////////

#include "DBEngine.h"

/// <summary>
/// Constructor for DBEngine with Owner as Argument.
/// </summary>
/// <param name="owner">Owner of the Database</param>
DBEngine::DBEngine(std::string owner) {
	_dbOwner = owner;
}

/// <summary>
/// Default Destructor for DBEngine. Frees Memory by clearing
/// DB Map and Tag Map.	
/// </summary>
DBEngine::~DBEngine() {
	/* Free Memory allocated to DBElements and the hash tables */
	for (std::pair<std::string, DBElement*> pr : _dbMap) {
		delete pr.second;
	}
	_dbMap.clear();
	_tagMap.clear();
}

/// <summary>
/// Function to Check whether Key Exists in Database or not.
/// </summary>
/// <param name="key">Key to Check</param>
/// <returns>True if Key Exists in Database, False if otherwise</returns>
bool DBEngine::exists(std::string key) {
	if (_dbMap.find(key) != _dbMap.end())
		return true;
	return false;
}

/// <summary>
/// Function to Add Tag to Object with Specified Key in the database.
/// </summary>
/// <param name="key">Key associated with Object to which Tag is intended to be added</param>
/// <param name="tag">Tag to be Added</param>
/// <returns></returns>
bool DBEngine::addTag(std::string key, std::string tag) {
	if (!exists(key))
		return false;
	if (_dbMap[key]->tagExist(tag))
		return true;
	_dbMap[key]->addTag(tag);
	_tagMap[tag].insert(key);
	return true;
}

/// <summary>
/// Function to Remove Tag From Object with Specified Key in the database.
/// </summary>
/// <param name="key">Key associated with Object to which Tag is intended to be removed from</param>
/// <param name="tag">Tag to be Removed</param>
/// <returns></returns>
bool DBEngine::removeTag(std::string key, std::string tag) {
	if (!exists(key))
		return false;
	if (!_dbMap[key]->tagExist(tag))
		return true;
	_dbMap[key]->removeTag(tag);
	_tagMap[tag].erase(key);
	return true;
}

/// <summary>
/// Function to Simultaneously Index Tags Whenever a new Object is Added to the Database.
/// </summary>
/// <param name="key">Key of the Object which will be Indexed on it's Tags</param>
void DBEngine::insertIndexTags(std::string key) {
	for (std::string tag : _dbMap[key]->getTags()) {
		if (_tagMap.find(tag) == _tagMap.end())
			_tagMap[tag] = std::unordered_set<std::string>();
		_tagMap[tag].insert(key);
	}
}

/// <summary>
/// Function to Simultaneously Index Tags Whenevr an Object is Removed from the Database.
/// </summary>
/// <param name="key">Key of the Object which is being removed or whose Ta</param>
void DBEngine::deleteIndexTags(std::string key) {
	for (std::string tag : _dbMap[key]->getTags()) {
		if (_tagMap.find(tag) == _tagMap.end())
			return;
		_tagMap[tag].erase(key);
	}
}

/// <summary>
/// Function to Retrieve the Owner of the DBEngine.
/// </summary>
/// <returns>Owner of the Database</returns>
std::string DBEngine::getOwner() {
	return _dbOwner;
}

/// <summary>
/// Function to Retrieve the Number of Objects in the Database.
/// </summary>
/// <returns>Number of Objects in the Database</returns>
size_t DBEngine::size() {
	return _dbMap.size();
}

/// <summary>
/// Function to Set the Owner of the Database.
/// </summary>
/// <param name="newOwner">New Owner</param>
/// <returns>Owner Value after it's Updated to New Owner</returns>
std::string DBEngine::setOwner(std::string newOwner) {
	_dbOwner = newOwner;
	return getOwner();
}

/// <summary>
/// Function to Insert DBElement into Database.
/// </summary>
/// <param name="key">Key</param>
/// <param name="value">DBElement to be Inserted</param>
/// <returns>True if DBElement Successfully Inserted, False if Otherwise</returns>
bool DBEngine::insert(std::string key, DBElement value) {
	if (exists(key))
		return false;
	DBElement * object = new DBElement(value);
	_dbMap[key] = object;
	insertIndexTags(key);
	return true;
}

/// <summary>
/// Function to Insert DBElement into Database.
/// </summary>
/// <param name="key">Key</param>
/// <param name="value">Pointer to the DBElement Which is to be Inserted</param>
/// <returns>True if DBElement Successfully Inserted, False if Otherwise</returns>
bool DBEngine::insert(std::string key, DBElement * value) {
	if(exists(key))
		return false;
	DBElement * object = new DBElement(*value);
	_dbMap[key] = object;
	insertIndexTags(key);
	return true;
}

/// <summary>
/// Function to Update DBElement associated with Key in Arguments in the Database.
/// </summary>
/// <param name="key">Key</param>
/// <param name="value">New Object to be Associated with the Key</param>
/// <returns>True if DBElement Associated with given Key is Successfully Updated in Database, False if Otherwise</returns>
bool DBEngine::update(std::string key, DBElement value) {
	if(!exists(key))
		return false;
	deleteIndexTags(key);
	DBElement * object = new DBElement(value);
	delete _dbMap[key];
	_dbMap[key] = object;
	insertIndexTags(key);
	return true;
}

/// <summary>
/// Function to Update DBElement associated with Key in Arguments in the Database.
/// </summary>
/// <param name="key">Key</param>
/// <param name="value">Pointer to New Object to be Associated with the Key</param>
/// <returns>True if DBElement Associated with given Key is Successfully Updated in Database, False if Otherwise</returns>
bool DBEngine::update(std::string key, DBElement * value) {
	if(!exists(key))
		return false;
	deleteIndexTags(key);
	DBElement * object = new DBElement(*value);
	delete _dbMap[key];
	_dbMap[key] = object;
	insertIndexTags(key);
	return true;
}

/// <summary>
/// Function to Remove DBElement associated with Key in Arguments in the Database.
/// </summary>
/// <param name="key">Key</param>
/// <returns>True if Key and the DBElement Associated with it are Successfully Removed from Database, False if Otherwise</returns>
bool DBEngine::remove(std::string key) {
	if(!exists(key))
		return false;
	deleteIndexTags(key);
	delete _dbMap[key];
	_dbMap.erase(key);
	return true;
}

/// <summary>
/// Function to Get Object Associated with given Key from Database in nicely Formatted Manner.
/// </summary>
/// <param name="key">Key</param>
/// <returns>If Key Exists then returns Object Associated with given Key from Database in nicely Formatted Manner, Else return Invalid</returns>
std::string DBEngine::getData(std::string key) {
	if (!exists(key))
		return "Invalid Key";
	std::string aggregator;
	aggregator.append(" Key : " + key + "\n");
	aggregator.append(" -----\n");
	aggregator.append(_dbMap[key]->show());
	return aggregator;
}

/// <summary>
/// Function to Get Object Associated with given Key from Database in Raw Format (as DBEngine)
/// </summary>
/// <param name="key">Key</param>
/// <returns>If given Key Exists in the Database then Return the DBElement associated with it, Else return DBElement with Invalid Key as Data</returns>
DBElement DBEngine::getDataRaw(std::string key) {
	if (!exists(key))
		return DBElement("> invalid key");
	return *_dbMap[key];
}

/// <summary>
/// Function to Update Data of the DBElement Associated with given Key in the Database.
/// </summary>
/// <param name="key">Key</param>
/// <param name="data">New Value of Data</param>
/// <returns></returns>
bool DBEngine::updateData(std::string key, std::string data)
{
	if(!exists(key))
		return false;
	_dbMap[key]->setData(data);
	return true;
}

/// <summary>
/// Function to Show Entire Database.
/// </summary>
/// <returns>Entire Database in Nicely Formatted Manner</returns>
std::string DBEngine::show() {
	std::string aggregator;
	for (std::pair<std::string, DBElement*> pr : _dbMap) {
		aggregator.append(getData(pr.first) + "\n");
	}
	return aggregator;
}

/// <summary>
/// Function to Show all the Objects associated with Keys in the Arguments which 
/// are Present in the Datbaase.
/// </summary>
/// <param name="keys">List of Keys who's associated Objects are to be retrieved</param>
/// <returns>Objects associated with Keys in the Arguments which are Present in the Datbaase, in Nicely Formatter Manner</returns>
std::string DBEngine::show(std::unordered_set<std::string> keys) {
	std::string aggregator;
	for (std::string key : keys) {
		if (exists(key)) {
			aggregator.append(getData(key) + "\n");
		}
	}
	return aggregator;
}

/// <summary>
/// Function to Retrieve All the Keys of DBElements who have a Tag which is Exactly same 
/// as Argument.
/// </summary>
/// <param name="tag">Tag</param>
/// <returns>All the Keys of DBElements who have a Tag which is Exactly same as Argument</returns>
std::unordered_set<std::string> DBEngine::getKeysWithTag(std::string tag) {
	if (_tagMap.find(tag) == _tagMap.end())
		return std::unordered_set<std::string>();
	return _tagMap[tag];
}


/// <summary>
/// Function to Show all DBElements who have a Tag which is Exactly same 
/// as Argument.
/// </summary>
/// <param name="tag">Tag</param>
/// <returns>All DBElements who have a Tag which is Exactly same as Argument, in a Nicely Formatted Manner. Returns N/A if no such Tag Exists in Database</returns>
std::string DBEngine::showUsingTag(std::string tag) {
	if (_tagMap.find(tag) == _tagMap.end())
		return "N/A";
	return show(getKeysWithTag(tag));
}

#ifdef TEST_CREATE_DBENGINE

/* Include Utilities Namespace for StringHelper Functions */
using namespace Utilities;

/// <summary>
/// Test Function to Create DBElements to be Inserted into DBEngine.
/// </summary>
/// <returns>Vector of DBElements which are to be Inserted into DBEngine</returns>
std::vector<std::pair<std::string, DBElement>> createDBObjects() {
	std::vector<std::pair<std::string, DBElement>> objects;
	DBElement * obj0 = new DBElement("Anakin Skywalker", std::unordered_set<std::string>({ "Siths", "Data", "Empire", "Star Wars" }));
	DBElement * obj1 = new DBElement("Luke Skywalker", std::unordered_set<std::string>({ "Jedi", "Data", "Star Wars" }));
	DBElement * obj2 = new DBElement("T-100", std::unordered_set<std::string>({ "Terminator", "Data", "Machine" }));
	DBElement * obj3 = new DBElement("Dolores", std::unordered_set<std::string>({ "Westworld", "Data", "Machine", "Sentient", "AI" }));
	objects.push_back(std::make_pair("key0", *obj0));
	objects.push_back(std::make_pair("key1", *obj1));
	objects.push_back(std::make_pair("key2", *obj2));
	objects.push_back(std::make_pair("key3", *obj3));
	return objects;
}

/// <summary>
/// Function to insert DBElements into the DBEngine
/// </summary>
/// <param name="db">DBEngine</param>
void insertIntoDBEngine(DBEngine * db) {
	StringHelper::Title("Insert Objects into DBEngine");
	for (std::pair<std::string, DBElement> pr : createDBObjects()) {
		db->insert(pr.first, pr.second);
	}
	std::cout << "\n > Inserted 4 objects into Database" << std::endl;
	putline();
}
#endif // TEST_CREATE_DBENGINE

#ifdef TEST_DBENGINE

/// <summary>
/// Function to Test Methods which have perform Show Type Operations.
/// </summary>
/// <param name="db">DBEngine</param>
void testShow(DBEngine * db) {
	StringHelper::Title("Test Show Functions");
	StringHelper::Title("Test Show Object Function", '~');
	std::cout << "\n > Show Object with \"key3\"\n\n" << db->getData("key3");
	putline();

	StringHelper::Title("Test Show whole database", '~');
	std::cout << "\n" << db->show();
	
	StringHelper::Title("Test Show database objects with keys : { 'key0', 'key3', 'key4' }", '~');
	std::cout << "\n" << db->show(std::unordered_set<std::string>({ "key0", "key3", "key4" }));
	putline();

	StringHelper::Title("Test Show Database Using Matching Tag Function", '~');
	std::cout << "\n Show Objects with Tag \"Machine\"\n";
	std::cout << "\n" << db->showUsingTag("Machine");
	putline();

}

/// <summary>
/// Function to Test Methods which perform Operations on Tags.
/// </summary>
/// <param name="db">DBENgine</param>
void testTagOperations(DBEngine * db) {
	StringHelper::Title("Test Show Functions");
	StringHelper::Title("Test Add Tag Function", '~');
	std::cout << "\n Before Adding Tag\n\n" << db->getData("key3") << std::endl;
	std::cout << "\n > Add tag \"Wayatt\" to \"Key3\"";
	db->addTag("key3", "Wayatt");
	std::cout << "\n After Adding Tag\n\n" << db->getData("key3");
	putline();

	StringHelper::Title("Test Remove Tag Function", '~');
	std::cout << "\n Before removing Tag\n\n" << db->getData("key3") << std::endl;
	std::cout << "\n > Remove tag \"Wayatt\" to \"Key3\"";
	db->removeTag("key3", "Wayatt");
	std::cout << "\n After Removing Tag\n\n" << db->getData("key3");
	putline();

	StringHelper::Title("Test Get Keys with Tag Function", '~');
	std::cout << "\n Get all keys with Tag : \"Star Wars\"";
	{
		std::cout << "\n > Keys : ";
		bool first = true;
		for (std::string key : db->getKeysWithTag("Star Wars")) {
			if (first) {
				first = false;
				std::cout << "\"" << key << "\"";
			}
			else {
				std::cout << ", \"" << key << "\"";
			}
		}
		std::cout << "\n\n";
	}
}

/// <summary>
/// Function to Test Methods which perform Update and Delete Operations.
/// </summary>
/// <param name="db">DBEngine</param>
void testUpdateDelete(DBEngine * db) {
	StringHelper::Title("Test Update and Delete Functions");
	StringHelper::Title("Test Update Function", '-');
	std::cout << "\n > Before Updating Object with Key : \"key2\"\n" << db->getData("key2") << std::endl;
	std::cout << "\n Update \"key2\"'s Object in the Database";
	db->update("key2", DBElement("Deckard", std::unordered_set<std::string>({ "Replicant", "Blade Runner", "Data", "AI", "Machine" })));
	std::cout << "\n > After Updating Object with Key : \"key2\"\n" << db->getData("key2") << std::endl;
	putline();

	StringHelper::Title("Test Delete Function", '-');
	std::cout << "\n > Object with \"key2\" Exists in Database ? " << db->exists("key2") << std::endl;
	std::cout << "\n Removing Oject with \"key2\" from Database";
	db->remove("key2");
	std::cout << "\n > Object with \"key2\" Exists in Database ? " << db->exists("key2") << std::endl;
	putline();
}

/// <summary>
/// Function to Test DBElement Package.
/// </summary>
/// <param name="argc">Argument Count</param>
/// <param name="argv">Arguments</param>
/// <returns></returns>
int main(int argc, char* argv[]) {
	Timer time;
	time.StartClock();

	StringHelper::Title("TESTING DBELEMENT PACKAGE", '=');
	StringHelper::Title("Create DBEngine");
	std::cout << "\n Creating DBEngine with owner : \"anonymous\"" << std::endl;
	DBEngine * db = new DBEngine("anonymous");
	putline();

	insertIntoDBEngine(db);
	testShow(db);
	testTagOperations(db);
	testUpdateDelete(db);
	
	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n ";
	return 0;
}

#endif // TEST_DBENGINE
