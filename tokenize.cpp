#include "tokenize.hpp"
#include <cctype>

#include <iostream>

//This checks if the token being added is empty
// if not it will add it otherwise it is discarded
void store_ifnot_empty(std::string & token, TokenSequenceType & seq) {
	if (!token.empty()) {
		seq.emplace_back(token);
		token.clear();
	}
}

//This will take an input stream and separate
// each word/token into strings and put them into
// a deque container
TokenSequenceType tokenize(std::istream & seq) {
	TokenSequenceType tokens;
	std::string token;
	bool ok = true;
	while (ok) {
		char c = seq.get();
		if (seq.eof()) break;
		switch (c) {
		case COMMENT: {
			while ((!seq.eof()) && (c != '\n')) {
				c = seq.get();
			}
			if (seq.eof()) ok = false;
		} break;
		case OPEN: {
			store_ifnot_empty(token, tokens);
			tokens.push_back("(");
		} break;
		case CLOSE: {
			store_ifnot_empty(token, tokens);
			tokens.push_back(")");
		} break;
		default: {
			bool flag = isspace(c) != 0;
			switch (flag) {
			case true: store_ifnot_empty(token, tokens);
				break;
			default: token.push_back(c);
				break;
			}
		} break;
		}
	}
	store_ifnot_empty(token, tokens);

	return tokens;
}
