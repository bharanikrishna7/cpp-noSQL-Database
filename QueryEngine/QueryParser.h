//////////////////////////////////////////////////////////////////////
// QueryParser.h    - Parses Client Requests to retrieve arguments. //
// Version          - 1.1                                           //
// Last Modified    - 08/10/2017                                    //
// Language         - Visual C++, Visual Studio 2017                //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10             //
// Author           - Venkata Bharani Krishna Chekuri               //
// e-mail           - bharanikrishna7@gmail.com                     //
//////////////////////////////////////////////////////////////////////
/*
 * INTRODUCTION
 * ------------
 * This package Provides a public Toker class, private State class and
 * a Context Struct.
 * 
 * The Toker Reads words from std::stream sequentially and Performs 
 * operations depending on the Squence of Tokens it has Read. The Objective
 * of This Toker is to Parse Client Requests and Retrieve the Arguments which
 * have been passed to it.
 *
 * Uses State Design Pattern which makes it Scalable.
 *
 *
 * PACKAGE OPERATIONS
 * ------------------
 * - Toker(bool verbose = false)
 * Constructor with Verbose Argument. Setting Verbose to True will
 * print the Debugging Output to Console.
 *
 * - std::unordered_map<char, std::string> Compute(const char* query)
 * Parses Client's Request and Returns the Arguments.
 *
 *
 * DEPENDANT FILES
 * ---------------
 * Utilities.h, Utilities.cpp
 *
 *
 * CHANGELOG
 * ---------
 * ver 1.0 : 08/07/2017
 * - First Release.
 *
 * ver 1.1 : 08/10/2017
 * - Updated Context and Added a new State to Parse User Argument (EatUser).
 *
 */
#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <iosfwd>
#include <string>
#include <iostream>

#include "../Utilities/Utilities.h"

/// <summary>
/// Namespace containing classes and methods to parse query.
/// </summary>
namespace QueryScanner {
	/* Forward Declaration */
	class State;
	/* Forward Declaration */
	struct Context;

	/// <summary>
	/// Class which will read a Query (String) Character by Character
	/// to generate Query Parameters.
	/// 
	/// This is a State Based Tokenizer i.e. Different States will be
	/// used to parse and retrieve different types of tokens (query 
	/// parameter).
	/// </summary>
	class Toker {
	private:
		std::string GetTok();
		bool CanRead();
		size_t CurrentLineCount();

		State * _pConsumer;
		Context * _pContext;
		std::string FilePath;

		bool VERBOSE;
	public:
		Toker(bool verbose = false);
		Toker(const Toker&) = delete;
		~Toker();
		Toker& operator=(const Toker&) = delete;
		bool Attach(std::stringstream* pIn);
		std::unordered_map<char, std::string> Compute(const char* query);
	};
}

#endif // QUERYPARSER_H
