//////////////////////////////////////////////////////////////////
// DBElement.cpp    - Defines DBElement Object for use in noSQL //
//                    Database.                                 //
// Version          - 1.0                                       //
// Last Modified    - 08/05/2017                                //
// Language         - Visual C++, Visual Studio 2015            //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10         //
// Author           - Venkata Bharani Krishna Chekuri           //
// e-mail           - bharanikrishna7@gmail.com                 //
//////////////////////////////////////////////////////////////////

#include "DBElement.h"

/// <summary>
/// Constructor with Data and Tags as Arguments.
/// </summary>
/// <param name="data">Data</param>
/// <param name="tags">Metadata Tags</param>
DBElement::DBElement(std::string data, std::unordered_set<std::string> tags) {
	_data = data;
	_tags = tags;
	setTimestamp();
}

/// <summary>
/// Default Constructor.
/// </summary>
DBElement::~DBElement() {
	_data.clear();
	_tags.clear();
}

/// <summary>
/// Constructor with Data as Argument.
/// </summary>
/// <param name="data">Data</param>
DBElement::DBElement(std::string data) {
	_data = data;
	setTimestamp();
}

/// <summary>
/// Method to Set Last Modified Timestamp.
/// </summary>
void DBElement::setTimestamp() {
	_timestamp = Utilities::TimeHelper::getCurrentTimestamp();
}

/// <summary>
/// Method to Retrieve Last Modified Timestamp.
/// </summary>
/// <returns>Last Mofidied Timestamp in YYYYmmDDHHMMSS Format</returns>
long long int DBElement::getlastModified() {
	return _timestamp;
}

/// <summary>
/// Method to Retrieve Last Modified Timestamp as const.
/// </summary>
/// <returns>Last Mofidied Timestamp in YYYYmmDDHHMMSS Format</returns>
long long int DBElement::getlastModified() const {
	return _timestamp;
}

/// <summary>
/// Method to Set Data variable.
/// </summary>
/// <param name="data">Data</param>
/// <returns></returns>
std::string DBElement::setData(std::string data) {
	_data = data;
	setTimestamp();
	return getData();
}

/// <summary>
/// Method to Get Data.
/// </summary>
/// <returns>Data</returns>
std::string DBElement::getData() {
	return _data;
}

/// <summary>
/// Method to Get Data as const.
/// </summary>
/// <returns>Data</returns>
std::string DBElement::getData() const {
	return _data;
}

/// <summary>
/// Method to Check if the Tag in Argument Exist in the Metadata Tags.
/// </summary>
/// <param name="tag">Tag</param>
/// <returns>True if Tag Exist, False if otherwise</returns>
bool DBElement::tagExist(std::string tag) {
	if (_tags.find(tag) != _tags.end())
		return true;
	return false;
}

/// <summary>
/// Method to Add Tag to the Metadata Tags.
/// </summary>
/// <param name="tag">Tag</param>
/// <returns>True if Tag has been Added, False if tag was already present</returns>
bool DBElement::addTag(std::string tag) {
	if (tagExist(tag))
		return false;
	_tags.insert(tag);
	setTimestamp();
	return true;
}

/// <summary>
/// Method to Add Multiple set of Tags to Metadata Tags.
/// </summary>
/// <param name="tags">Vector of Tags</param>
/// <returns>Number of Tags that have been successfully added</returns>
size_t DBElement::addTags(std::vector<std::string> tags) {
	size_t inserted = 0;
	for (std::string tag : tags) {
		if (addTag(tag))
			inserted++;
	}
	if(inserted > 0)
		setTimestamp();
	return inserted;
}

/// <summary>
/// Get the count of Metadata Tags.
/// </summary>
/// <returns>Number of Metadata Tags</returns>
size_t DBElement::getTagCount() {
	return _tags.size();
}

/// <summary>
/// Method to Remove a Tag From Metadata Tags.
/// </summary>
/// <param name="tag">Tag</param>
/// <returns>True if Tag is removed, False if Tag is not present in Metadata Tags</returns>
bool DBElement::removeTag(std::string tag) {
	if (!tagExist(tag))
		return false;
	_tags.erase(tag);
	setTimestamp();
	return true;
}

/// <summary>
/// Method to get all the Metadata Tags
/// </summary>
/// <returns>All the Tags Present in Metadata Tags</returns>
std::unordered_set<std::string> DBElement::getTags() {
	return _tags;
}

/// <summary>
/// Method to get all the Metadata Tags as const
/// </summary>
/// <returns>All the Tags Present in Metadata Tags</returns>
std::unordered_set<std::string> DBElement::getTags() const {
	return _tags;
}

/// <summary>
/// Method to Show DBElement in a nice Formatted Manner
/// </summary>
/// <returns>DBElement in a nicely Formatter Manner</returns>
std::string DBElement::show() {
	std::string aggregator;
	aggregator.append(" Data      : " + getData() + "\n");
	aggregator.append(" Timestamp : " + Utilities::TimeHelper::timestamptoStrimg(_timestamp) + "\n");
	aggregator.append(" Tags      : ");
	if (_tags.empty()) {
		aggregator.append("\"N/A\"\n");
	} else {
		bool first = true;
		for (std::string tag : _tags) {
			if (first) {
				aggregator.append("\"" + tag + "\"");
				first = false;
			}
			else {
				aggregator.append(", \"" + tag + "\"");
			}
		}
		aggregator.push_back('\n');
	}
	return aggregator;
}

#ifdef TEST_DBELEMENT

/* Include Utilities Namespace for StringHelper Functions */
using namespace Utilities;

/// <summary>
/// Method to Test DBElement Package
/// </summary>
/// <param name="argc">Argument Count</param>
/// <param name="argv">Arguments</param>
/// <returns></returns>
int main(int argc, char* argv[]) {
	Timer time;
	time.StartClock();

	StringHelper::Title("TESTING DBELEMENT PACKAGE", '=');
	StringHelper::Title("Create DBElement with variable name := 'Object_1'");
	DBElement * object = new DBElement("Darth Vadar");
	std::cout << "\n DBElement Created" << std::endl;
	putline();

	StringHelper::Title("Test getData Method");
	std::cout << "\n > Data : " << object->getData() << std::endl;
	putline();

	StringHelper::Title("Test setData Method");
	std::cout << "\n Set Data Value to Anakin Skywalker";
	std::cout << "\n > Data : " << object->setData("Anakin Skywalker") << std::endl;
	putline();

	StringHelper::Title("Test addTag Method");
	std::cout << "\n Add Tags := 'Data', 'Sith', 'Empire' to DBElement";
	object->addTags({ "Data", "Sith", "Empire" });
	std::cout << "\n > DBElement Tags : ";
	for (std::string tag : object->getTags()) {
		std::cout << "\"" << tag << "\" ";
	}
	putline();
	putline();

	StringHelper::Title("Test removeTag Method");
	std::cout << "\n Remove Tag 'Data' from DBElement";
	object->removeTag("Data");
	std::cout << "\n > DBElement Tags : ";
	for (std::string tag : object->getTags()) {
		std::cout << "\"" << tag << "\" ";
	}
	putline();
	putline();

	StringHelper::Title("Test tagExist Method");
	std::cout << "\n Check if 'Data' tag Exists";
	std::cout << "\n > Tag Exists : " << object->tagExist("Data");
	std::cout << "\n Check if 'Sith' tag Exists";
	std::cout << "\n > Tag Exists : " << object->tagExist("Sith") << std::endl;
	putline();

	StringHelper::Title("Test show Method");
	std::cout << "\n" << object->show();
	putline();

	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n ";
	return 0;
}

#endif // TEST_DBELEMENT
