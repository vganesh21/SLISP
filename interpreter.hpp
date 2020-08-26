#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <string>
#include <istream>
#include <vector>


// module includes
#include "expression.hpp"
#include "environment.hpp"
#include "tokenize.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter{
public:
  bool parse(std::istream & expression) noexcept;
  Expression eval();
protected:
  Environment env;
  Expression ast;
  std::vector<Atom> graphics;
  void draw(Expression exp);
private:
	Expression evalHelper(Expression exp, Environment & env);
	Expression parseRead(const TokenSequenceType &tokens) noexcept;
	Expression handle_if(Expression exp, Environment & env);
	Expression handle_lookup(const Atom & head, const Environment & env);
	Expression handle_begin(Expression exp, Environment & env);
	Expression handle_define(Expression exp, Environment & env);
	Expression apply(const Atom & op, const std::vector<Expression> & args, const Environment & env);
};


#endif
