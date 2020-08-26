#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

//This class is based on the plotScript project provided in earlier semesters

//Helper for parsing - converts a token to an expression
bool setHead(Expression &exp, const std::string &token) {

	Atom a;
	token_to_atom(token, a);

	exp.head = a;

	return !(a.type == NoneType);
}
//Helper for parsing - adds a token to given expression
bool append(Expression *exp, const std::string &token) {

	Atom a;
	token_to_atom(token, a);

	exp->tail.emplace_back(a);

	return !(a.type == NoneType);
}

//Helper for parsing; builds the AST for the interpreter
//Uses a stack to track the last created nodes and adds it to the tree
Expression Interpreter::parseRead(const TokenSequenceType &tokens) noexcept {
	Expression exp;
	if (tokens.empty()) {
		return Expression();
	}
	bool atHead = false;
	std::stack<Expression *> stack;
	std::size_t num_tokens_seen = 0;
	for (auto &t : tokens) {
		if (t == "(") {
			atHead = true;
		}
		else if (t == ")") {
			if (stack.empty()) {
				return Expression();
			}
			stack.pop();
			if (stack.empty()) {
				num_tokens_seen += 1;
				break;
			}
		}
		else {
			if (atHead) {
				if (stack.empty()) {
					if (!setHead(exp, t)) {
						return Expression();
					}
					stack.push(&exp);
				}
				else {
					if (stack.empty()) {
						return Expression();
					}
					if (!append(stack.top(), t)) {
						return Expression();
					}
					Expression * ptr = nullptr;
					if (!stack.top()->tail.empty()) {
						ptr = &stack.top()->tail.back();
					}
					stack.push(ptr);
				}
				atHead = false;
			}
			else {
				if (stack.empty()) {
					return Expression();
				}
				if (!append(stack.top(), t)) {
					return Expression();
				}
			}
		}
		num_tokens_seen += 1;
	}

	if (stack.empty() && (num_tokens_seen == tokens.size())) {
		return exp;
	}

	return Expression();
}

//Main parsing function
bool Interpreter::parse(std::istream & expression) noexcept {
	TokenSequenceType tokens = tokenize(expression);

	ast = parseRead(tokens);

	return (!(ast == Expression()));
};

//Will execute the necessary procedure from the environment map
Expression Interpreter::apply(const Atom & op, const std::vector<Expression> & args, const Environment & env) {
	if (op.type != SymbolType) {
		throw InterpreterSemanticError("Error during evaluation: procedure name not symbol.");
	}
	if (!env.is_proc(op)) {
		throw InterpreterSemanticError("Error during evaluation: symbol does not name a procedure.");
	}
	Procedure proc = env.get_proc(op);
	return proc(args);
}

//Handles the lookup for defined symbols or pre-built symbols like pi
Expression Interpreter::handle_lookup(const Atom & head, const Environment & env) {
	switch (head.type) {
	case SymbolType: {
		if (env.is_exp(head)) {
			return env.get_exp(head);
		}
		else {
			throw InterpreterSemanticError("Error during evaluation: unknown symbol");
		}
	} break;
	case BooleanType: {
		return Expression(head);
	} break;
	case NumberType: {
		return Expression(head);
	} break;
	default: {
		throw InterpreterSemanticError("Error during evaluation: Invalid type in terminal expression");
	} break;
	}
}
/*
 * The following few functions are used to handle the special forms define, begin, and if
 * They require separate functions in interpreter as some of their arguments could be
 * procedures that need to be evaluated with the eval function, or have another special form within itself
 */


 //Handles the special begin
Expression Interpreter::handle_begin(Expression exp, Environment & env) {
	if (exp.tail.empty()) {
		throw InterpreterSemanticError("Error during evaluation: zero arguments to begin");
	}
	Expression result;
	for (auto it = exp.tail.begin(); it != exp.tail.end(); ++it) {
		result = evalHelper(*it, env);
	}
	return result;
}

//Handles the special define
Expression Interpreter::handle_define(Expression exp, Environment & env) {
	if (exp.tail.size() != 2) {
		throw InterpreterSemanticError("Error during evaluation: invalid number of arguments to define");
	}
	if (exp.tail[0].head.type != SymbolType) {
		throw InterpreterSemanticError("Error during evaluation: first argument to define not symbol");
	}
	std::string s = exp.tail[0].head.value.sym_value;
	if ((s == "define") || (s == "begin") || (s == "if")) {
		throw InterpreterSemanticError("Error during evaluation: attempt to redefine a special-form");
	}
	if (env.is_proc(exp.head)) {
		throw InterpreterSemanticError("Error during evaluation: attempt to redefine a built-in procedure");
	}
	Expression result = evalHelper(exp.tail[1], env);

	env.add_exp(exp.tail[0].head, result);

	return result;
}

//Handles the special if
Expression Interpreter::handle_if(Expression exp, Environment & env) {
	if (exp.tail.size() != 3) {
		throw InterpreterSemanticError("Error during evaluation: invalid number of arguments for if.");
	}
	Expression arg1 = evalHelper(exp.tail[0], env);
	Expression arg2 = evalHelper(exp.tail[1], env);
	Expression arg3 = evalHelper(exp.tail[2], env);
	if (arg1.head.type != BooleanType) {
		throw InterpreterSemanticError("Error during evaluation: invalid argument type.");
	}
	bool checks = arg1.head.value.bool_value;
	switch (checks) {
	case true:  return arg2;
		break;
	case false: return arg3;
		break;
	default:    throw InterpreterSemanticError("Error during evaluation: unknown error");
		break;
	}

}

void Interpreter::draw(Expression exp) {
    graphics.push_back(exp.head);
}

//Evaluate helper function - Recursive function to evaluate the entire AST
// and return the result.
Expression Interpreter::evalHelper(Expression exp, Environment & env) {

	if (exp.tail.empty()) {
		return handle_lookup(exp.head, env);
	}
	else if ((exp.head.type == SymbolType) && (exp.head.value.sym_value == "begin")) {
		return handle_begin(exp, env);
	}
	else if ((exp.head.type == SymbolType) && (exp.head.value.sym_value == "define")) {
		return handle_define(exp, env);
	}
	else if ((exp.head.type == SymbolType) && (exp.head.value.sym_value == "if")) {
		return handle_if(exp, env);
	}
    else if ((exp.head.type == SymbolType) && (exp.head.value.sym_value == "draw")) {
        Expression result;
        if(exp.tail.size() == 0) {
            throw InterpreterSemanticError("Error during evaluation: invalid number of arguments for draw.");
        }
        for(int i = 0; i< exp.tail.size(); ++i) {
            result = evalHelper(exp.tail[i], env);
            if(result.head.type == PointType
             || result.head.type == LineType
             || result.head.type == ArcType) {
                draw(result);
            }
            else {
                throw InterpreterSemanticError("Error during evaluation: invalid argument type.");
            }
        }
        return Expression();
    }
	else {
		std::vector<Expression> results;
		for (auto it = exp.tail.begin(); it != exp.tail.end(); ++it) {
			results.push_back(evalHelper(*it, env));
		}
		return apply(exp.head, results, env);
	}
}

//Eval function to be called outside the interpreter
Expression Interpreter::eval() {
	Expression result;
	try {
		result = evalHelper(ast, env);
		env.save();
	}
	catch(const InterpreterSemanticError& ex) {
		env.revert();
		throw;
	}

	return result;
}


