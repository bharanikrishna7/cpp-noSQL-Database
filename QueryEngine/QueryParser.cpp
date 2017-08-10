//////////////////////////////////////////////////////////////////////
// QueryParser.cpp  - Parses Client Requests to retrieve arguments. //
// Version          - 1.1                                           //
// Last Modified    - 08/10/2017                                    //
// Language         - Visual C++, Visual Studio 2017                //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10             //
// Author           - Venkata Bharani Krishna Chekuri               //
// e-mail           - bharanikrishna7@gmail.com                     //
//////////////////////////////////////////////////////////////////////
#include <stack>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "QueryParser.h"

namespace QueryScanner {
	/// <summary>
	/// Structure to Store Context Information. This Structure is the Most Crucial Part 
	/// of the Tokenizer Since it Holds all the Information.
	/// </summary>
	struct Context {
		Context();
		~Context();

		std::string token;
		std::istream* _pIn;

		int prevChar;
		int currChar;
		size_t _lineCount;

		std::unordered_map<char, std::string> _queryParams;

		bool VERBOSE;

		State * _pState;
		State * _pEatKey;
		State * _pEatType;
		State * _pEatUser;
		State * _pEatValue;
		State * _pEatOperation;
		State * _pEatParameter;
		State * _pEatWhitespace;

	};

	/// <summary>
	/// Template Class for Different State Classes. These derived States 
	/// will help in processing the Query (each state will have different
	///	eatChars methods to process specific tokens).
	/// </summary>
	class State {
		friend class Toker;
	public:
		using Token = std::string;
		State();
		State(const State&) = delete;
		State& operator=(const State&) = delete;
		virtual ~State();
		void Attach(std::istream * pIn);
		virtual void EatChars() = 0;
		void ConsumeChars() {
			_pContext->_pState->EatChars();
			_pContext->_pState = NextState();
		}
		bool nextCharMatch(char ch) {
			if (ch == 't' || ch == 'k' || ch == 'v' || ch == 'p' || ch == 'o' || ch == 'u')
				return true;
			return false;
		}
		bool CanRead() { return _pContext->_pIn->good(); }
		std::string GetTok() { return _pContext->token; }
		bool HasTok() { return _pContext->token.size() > 0; }
		State * NextState();
		size_t CurrentLineCount() { return _pContext->_lineCount; }
		void SetContext(Context * pContext);
	protected:
		Context * _pContext;
		bool CollectChar();
	};
}

using namespace QueryScanner;
using Token = std::string;



/// <summary>
/// Function to Set the Current Context.
/// </summary>
/// <param name="pContext"></param>
void State::SetContext(Context * pContext) {
	_pContext = pContext;
}

/// <summary>
/// Function to Attach String Stream to the State.
/// </summary>
/// <param name="pIn"></param>
void State::Attach(std::istream * pIn) {
	_pContext->_pState = _pContext->_pEatWhitespace;
	_pContext->_pIn = pIn;
}

/// <summary>
/// Function to Read Next Character From String Stream.
/// It updates prevChar and lineCount too along with currChar.
/// </summary>
/// <returns>Returns true if the next character is stringstream::good()</returns>
bool State::CollectChar() {
	if (_pContext->_pIn->good())
	{
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\n')
			++(_pContext->_lineCount);
		return true;
	}
	return false;
}

/// <summary>
/// Function to Transition Into Next State. It
/// analyzes the current and next characters
/// to select appropriate state.
/// </summary>
/// <returns>Appropriate state based on current and next char</returns>
State* State::NextState()
{
	if (!(_pContext->_pIn->good())) {
		return nullptr;
	}
	int chNext = _pContext->_pIn->peek();
	/* The tests should always be in the following order */
	if (_pContext->currChar == '-' && chNext == 'u') {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatUser";
		CollectChar();
		return _pContext->_pEatUser;
	}
	if (_pContext->currChar == '-' && chNext == 't') {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatType";
		CollectChar();
		return _pContext->_pEatType;
	}
	if (_pContext->currChar == '-' && chNext == 'k') {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatKey";
		CollectChar();
		return _pContext->_pEatKey;
	}
	if (_pContext->currChar == '-' && chNext == 'v') {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatValue";
		CollectChar();
		return _pContext->_pEatValue;
	}
	if (_pContext->currChar == '-' && chNext == 'o') {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatOperation";
		CollectChar();
		return _pContext->_pEatOperation;
	}
	if (_pContext->currChar == '-' && chNext == 'p') {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatParameter";
		CollectChar();
		return _pContext->_pEatParameter;
	}
	if (chNext == '\0') {
		_pContext->_pIn->clear();
		/* if peek() reads end of file character, EOF, then eofbit is set and
		* _pIn->good() will return false.  clear() restores state to good
		*/
	}
	if (std::isspace(_pContext->currChar)) {
		if (_pContext->VERBOSE)
			std::cout << "\n State := EatWhiteSpace";
		return _pContext->_pEatWhitespace;
	}
	/* If _pContext->currChar is not a state transition char
	 * then send it to Eat Value State.
	 */
	return _pContext->_pEatWhitespace;
}

/// <summary>
/// State for parsing whitespaces. Derived implementation of State Class.
/// </summary>
class EatWhitespace : public State {
public:
	EatWhitespace(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse whitespaces. It will keep on reading stringstream
	/// till no more whitespaces are encountered or end of query (whichever 
	/// comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				return;
		} while (std::isspace(_pContext->currChar));
	}
};

/// <summary>
/// State for parsing Type Query Parameter. Implementation of State Class.
/// </summary>
class EatUser : public State {
public:
	EatUser(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse Type Query Parameter. It will keep on reading and storing
	/// the stringstream characters till it encounters one of the Query Flags or end
	/// of query (whichever comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				break;
			_pContext->token.push_back(_pContext->currChar);
		} while (!(_pContext->currChar == '-' && (_pContext->_pIn->good() && nextCharMatch(_pContext->_pIn->peek()))));
		_pContext->_pIn->unget();
		_pContext->token.pop_back();
		if(!_pContext->token.empty()) {
			_pContext->_queryParams['u'] = Utilities::StringHelper::lrtrim(_pContext->token);
			if (_pContext->VERBOSE)
				std::cout << "\n Query User := " + _pContext->_queryParams['u'] + "\n";
		}
	}
};

/// <summary>
/// State for parsing Type Query Parameter. Implementation of State Class.
/// </summary>
class EatType : public State {
public:
	EatType(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse Type Query Parameter. It will keep on reading and storing
	/// the stringstream characters till it encounters one of the Query Flags or end
	/// of query (whichever comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				break;
			_pContext->token.push_back(_pContext->currChar);
		} while (!(_pContext->currChar == '-' && (_pContext->_pIn->good() && nextCharMatch(_pContext->_pIn->peek()))));
		_pContext->_pIn->unget();
		_pContext->token.pop_back();
		if(!_pContext->token.empty()) {
			_pContext->_queryParams['t'] = Utilities::StringHelper::lrtrim(_pContext->token);
			if (_pContext->VERBOSE)
				std::cout << "\n Query Type := " + _pContext->_queryParams['t'] + "\n";
		}
	}
};

/// <summary>
/// State for parsing Key Query Parameter. Implementation of State Class.
/// </summary>
class EatKey : public State {
public:
	EatKey(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse Key Query Parameter. It will keep on reading and storing
	/// the stringstream characters till it encounters one of the Query Flags or end
	/// of query (whichever comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				break;
			_pContext->token.push_back(_pContext->currChar);
		} while (!(_pContext->currChar == '-' && (_pContext->_pIn->good() && nextCharMatch(_pContext->_pIn->peek()))));
		_pContext->_pIn->unget();
		_pContext->token.pop_back();
		if(!_pContext->token.empty()) {
			_pContext->_queryParams['k'] = Utilities::StringHelper::lrtrim(_pContext->token);
			if (_pContext->VERBOSE)
				std::cout << "\n Query Key := " + _pContext->_queryParams['k'] + "\n";
		}
	}
};


/// <summary>
/// State for parsing Value Query Parameter. Implementation of State Class.
/// </summary>
class EatValue : public State {
public:
	EatValue(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse Value Query Parameter. It will keep on reading and storing
	/// the stringstream characters till it encounters one of the Query Flags or end
	/// of query (whichever comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				break;
			_pContext->token.push_back(_pContext->currChar);
		} while (!(_pContext->currChar == '-' && (_pContext->_pIn->good() && nextCharMatch(_pContext->_pIn->peek()))));
		_pContext->_pIn->unget();
		_pContext->token.pop_back();
		if(!_pContext->token.empty()) {
			_pContext->_queryParams['v'] = Utilities::StringHelper::lrtrim(_pContext->token);
			if (_pContext->VERBOSE)
				std::cout << "\n Query Value := " + _pContext->_queryParams['v'] + "\n";
		}
	}
};

/// <summary>
/// State for parsing Operation Query Parameter. Implementation of State Class.
/// </summary>
class EatOperation : public State {
public:
	EatOperation(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse Operation Query Parameter. It will keep on reading and storing
	/// the stringstream characters till it encounters one of the Query Flags or end
	/// of query (whichever comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		bool first = true;
		do {
			if (!CollectChar())
				break;
			_pContext->token.push_back(_pContext->currChar);
		} while (!(_pContext->currChar == '-' && (_pContext->_pIn->good() && nextCharMatch(_pContext->_pIn->peek()))));
		_pContext->_pIn->unget();
		_pContext->token.pop_back();
		if (!_pContext->token.empty()) {
			_pContext->_queryParams['o'] = Utilities::StringHelper::lrtrim(_pContext->token);
			if (_pContext->VERBOSE)
				std::cout << "\n Query Operation := " + _pContext->_queryParams['o'] + "\n";
		}
	}
};

/// <summary>
/// State for parsing Parameter Query Parameter. Implementation of State Class.
/// </summary>
class EatParameter : public State {
public:
	EatParameter(Context * pContext) { _pContext = pContext; }

	/// <summary>
	/// Function to parse Parameter Query Parameter. It will keep on reading and storing
	/// the stringstream characters till it encounters one of the Query Flags or end
	/// of query (whichever comes first).
	/// </summary>
	virtual void EatChars() {
		_pContext->token.clear();
		bool first = true;
		do {
			if (!CollectChar())
				break;
			_pContext->token.push_back(_pContext->currChar);
		} while (!(_pContext->currChar == '-' && (_pContext->_pIn->good() && nextCharMatch(_pContext->_pIn->peek()))));
		_pContext->_pIn->unget();
		_pContext->token.pop_back();
		if (!_pContext->token.empty()) {
			_pContext->_queryParams['p'] = Utilities::StringHelper::lrtrim(_pContext->token);
			if (_pContext->VERBOSE)
				std::cout << "\n Query Parameter := " + _pContext->_queryParams['p'] + "\n";
		}
	}
};

/// <summary>
/// Default Constructor for Context Structure. It will Initialize all
/// the States and Scopes.
/// </summary>
Context::Context() {
	_pEatKey = new EatKey(this);
	_pEatUser = new EatUser(this);
	_pEatType = new EatType(this);
	_pEatValue = new EatValue(this);
	_pEatOperation = new EatOperation(this);
	_pEatParameter = new EatParameter(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pState = _pEatWhitespace;
	_lineCount = 0;

	_queryParams.clear();
}


/// <summary>
/// Default Destructor for Context Structure. It will free
/// memory by deleting all the Created States.
/// </summary>
Context::~Context() {
	delete _pEatKey;
	delete _pEatUser;
	delete _pEatType;
	delete _pEatValue;
	delete _pEatOperation;
	delete _pEatParameter;
	delete _pEatWhitespace;
}

/// <summary>
/// Default Constructor for State Class. Does nothing.
/// </summary>
State::State() {};

/// <summary>
/// Default Destructor for State Class. Does nothing.
/// </summary>
State::~State() {};

/// <summary>
/// Constructor for Toker Class. It Initializes Context, Current State,
/// and Verbose Variable. (Verbose Variable is used to enable Debugging.)
/// </summary>
/// <param name="verbose">Enable Verbose Mode (Debugging)</param>
Toker::Toker(bool verbose) {
	_pContext = new Context();
	_pConsumer = _pContext->_pEatWhitespace;
	_pConsumer->SetContext(_pContext);

	VERBOSE = verbose;
	_pContext->VERBOSE = verbose;
	if (VERBOSE) {
		std::cout << "\n Verbose Mode Turned On for Toker";
		std::cout << "\n Verbose Mode Turned On for Context";
		
		std::cout << "\n";
	}
}

/// <summary>
/// Default Destructor for Toker Class. It will free memory by deleting
/// memory allocated to Context.
/// </summary>
Toker::~Toker() {
	delete _pContext;
	_pContext = nullptr;
}

/// <summary>
/// Function to Attach String stream to Toker Object. Returns true if the String stream
/// is good, i.e it's not null and has valid ASCII characters.
/// </summary>
/// <param name="pIn">Query Stringstream which is to be processed</param>
/// <returns>True if the String stream is good, else returns False</returns>
bool Toker::Attach(std::stringstream * pIn) {
	if (pIn != nullptr && pIn->good()) {
		_pConsumer->Attach(pIn);
		return true;
	}
	return false;
}

/// <summary>
/// Function to process State and return Token. 
/// </summary>
/// <returns>Returns the String acquired by Processing State</returns>
std::string Toker::GetTok() {
	while (true) {
		if (!_pConsumer->CanRead())
			return  "";
		_pConsumer->ConsumeChars();
		if (_pConsumer->HasTok())
			break;
	}
	return _pConsumer->GetTok();
}

/// <summary>
/// Function to Check if the String stream is readable or not.
/// </summary>
/// <returns>True if String stream is readable, False if otherwise</returns>
bool Toker::CanRead() {
	return _pConsumer->CanRead();
}

/// <summary>
/// Function to return line count. Useful for multiline query.
/// </summary>
/// <returns>Zero Indexed Line count</returns>
size_t Toker::CurrentLineCount() {
	return _pConsumer->CurrentLineCount();
}

/// <summary>
/// Function to Process Query and return Query Parameters associated with
/// that query.
/// </summary>
/// <param name="query">Query to be parsed</param>
/// <returns>unordered_map containing flags and Tokens associated with the Flags</returns>
std::unordered_map<char, std::string> Toker::Compute(const char* query) {
	std::stringstream queryToStream(query);
	Attach(&queryToStream);
	do {
		std::string tok = GetTok();
	} while (queryToStream.good());
	if (VERBOSE)
		std::cout << "\n Line Count : " << CurrentLineCount();
	return _pContext->_queryParams;
}

#ifdef TEST_QUERYPARSER

/* Include Utilities Namespace for StringHelper Functions */
using namespace Utilities;

/// <summary>
/// Function to Test Toker and Associated Classes.
/// </summary>
/// <param name="argc">Argument Count</param>
/// <param name="argv">Arguments</param>
/// <returns></returns>
int main(int argc, char* argv[]) {
	StringHelper::Title("TEST QUERY PARSER", '=');
	std::string query = "-u Anonymous -t INSERT -k key3 -v Dolores -o ADDTAG -p AI";

	StringHelper::Title("Declaring Parser");
	Toker * toker = new Toker(false);
	std::cout << "\n - Parser Created !\n";

	StringHelper::Title(std::string("Processing Query : \"" + query + "\" using Toker"));
	std::unordered_map<char, std::string> queryParams = toker->Compute(query.c_str());
	std::cout << "\n Query Processing Complete !";
	std::cout << "\n";

	StringHelper::Title("Query Parameters Status");
	if (queryParams.empty())
		std::cout << "\n Query Parameters Retrieval Failed.";
	else
		std::cout << "\n Query Parameters Successfully Retrieved.";
	std::cout << "\n";

	StringHelper::Title("Display Query Parameters");
	for (std::pair<char, std::string> pr : queryParams)
		std::cout << "\n -" << pr.first << " | " << pr.second;
	std::cout << "\n\n ";

	return 0;
}

#endif // TEST_QUERYPARSER