#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

class Environment {
public:
	Environment();
	bool is_known(const Atom & pAtom) const;
	bool is_exp(const Atom & pAtom) const;
	Expression get_exp(const Atom &pAtom) const;
	void add_exp(const Atom &pAtom, const Expression &exp);
	bool is_proc(const Atom &pAtom) const;
	Procedure get_proc(const Atom &pAtom) const;
	void reset();
	void save();
	void revert();

private:
	void printMap() const;

	// Environment is a mapping from symbols to expressions or procedures
	enum EnvResultType { ExpressionType, ProcedureType };
	struct EnvResult {
		EnvResultType type;
		Expression exp;
		Procedure proc;

		EnvResult() {};
		EnvResult(EnvResultType t, Expression e) : type(t), exp(e) {};
		EnvResult(EnvResultType t, Procedure p) : type(t), proc(p) {};

	};

	std::map<Symbol, EnvResult> envmap;
	std::map<Symbol, EnvResult> revmap;
};

#endif
