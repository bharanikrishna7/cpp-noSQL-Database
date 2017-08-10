//////////////////////////////////////////////////////////////////
// DBElement.h	    - Defines DBElement Object for use in noSQL //
//                    Database.                                 //
// Version          - 1.0                                       //
// Last Modified    - 08/05/2017                                //
// Language         - Visual C++, Visual Studio 2017            //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10         //
// Author           - Venkata Bharani Krishna Chekuri           //
// e-mail           - bharanikrishna7@gmail.com                 //
//////////////////////////////////////////////////////////////////
/*
 * INFORMATION
 * -----------
 * This package provides DBElement class and methods to perform operations on it.
 * The DBElement class holds data and metadata to and has methods to set and get
 * them.
 * 
 *
 * PACKAGE OPERATIONS
 * ------------------
 * - DBElement(std::string data)
 * Constructor with Data as Argument.
 *
 * - DBElement(std::string data, std::unordered_set<std::string> tags)
 * Constructor with Data and Tags Arguments.
 * 
 * - std::string setData(std::string data)
 * Method to Set Data.
 *
 * - std::string getData()
 * Method to Get Data.
 *
 * - std::string getData() const
 * Method to Get Data as const String.
 *
 * - bool addTag(std::string tag)
 * Method to Add Tag to Metadata Tags.
 *
 * - size_t addTags(std::vector<std::string> tags)
 * Method to Add Multiple Tags to Metadata Tags.
 * 
 * - bool removeTag(std::string tag)
 * Method to Remove Tag from Metadata Tags.
 * 
 * - bool tagExist(std::string tag)
 * Method to Check if a Tag exists in Metadata Tags.
 *
 * - std::unordered_set<std::string> getTags()
 * Method to get all the Metadata Tags.
 *
 * - std::unordered_set<std::string> getTags() const
 * Method to get all the Metadata Tags as const String.
 *
 * - size_t getTagCount()
 * Method to Get the Number of Tags in the Metadata Tags.
 *
 * - long long int getlastModified()
 * Method to get the Last Modified Timestamp as long long int in YYYYmmDDHHMMSS Format.
 * 
 * - long long int getlastModified() const
 * Method to get the Last Modified Timestamp as const long long int in YYYYmmDDHHMMSS Format.
 * 
 * - std::string show();
 * Method to get the DBElement Contents in a Nicely Formatted Manner.
 *
 *
 * REQUIRED FILES
 * --------------
 * Utilities.h, Utilities.cpp
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
 */
#ifndef DBELEMENT_H
#define DBELEMENT_H

#include "../Utilities/Utilities.h"

#include <string>
#include <unordered_set>

/// <summary>
/// Class to hold Objects which can be Inserted into DBEngine. Stores Data
/// in String Format. It also contains Metadata Information which include
/// Last Modified and Tags.
/// </summary>
class DBElement {
private:
	std::string _data;							// data
	long long int _timestamp;					// metadata timestamp
	std::unordered_set<std::string> _tags;		// metadata tags

	/* Member Functions */
	void setTimestamp();
public:
	/* Constructors */
	DBElement(std::string data);
	DBElement(std::string data, std::unordered_set<std::string> tags);

	/* Destructor */
	~DBElement();

	/* Member Functions */
	std::string setData(std::string data);
	std::string getData();
	std::string getData() const;
	bool addTag(std::string tag);
	size_t addTags(std::vector<std::string> tags);
	bool removeTag(std::string tag);
	bool tagExist(std::string tag);
	std::unordered_set<std::string> getTags();
	std::unordered_set<std::string> getTags() const;
	size_t getTagCount();
	long long int getlastModified();
	long long int getlastModified() const;
	std::string show();
};

#endif // !DBELEMENT_H
