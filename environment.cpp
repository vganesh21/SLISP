#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "interpreter_semantic_error.hpp"

const double PI = atan2(0, -1);
//The following group of functions are defining how
// the procedures implemented should behave given
// proper and improper arguments

Expression add(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.empty()) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}

	for (auto & a : args)
	{
		if (a.head.type == NumberType)
			result += a.head.value.num_value;
		else
			throw InterpreterSemanticError("Error: Invalid argument type");
	}
	return Expression(result);
}

Expression mult(const std::vector<Expression> & args)
{
	double result = 1.0;
	if (args.empty()) {
		throw InterpreterSemanticError("Error: Invalid number of arguments");
	}

	for (auto & a : args)
	{
		if (a.head.type == NumberType)
			result *= a.head.value.num_value;
		else
			throw InterpreterSemanticError("Error: Invalid argument type");
	}
	return Expression(result);
}

Expression subneg(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() == 1)
	{
		if (args[0].head.type == NumberType) {
			result = -args[0].head.value.num_value;
			return Expression(result);
		}
		else {
			throw InterpreterSemanticError("Error: Invalid argument type");
		}
	}
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = args[0].head.value.num_value - args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression divide(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = args[0].head.value.num_value / args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression power(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = pow(args[0].head.value.num_value, args[1].head.value.num_value);
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression logs(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() != 1) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType)
	{
		result = log10(args[0].head.value.num_value);
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression isNot(const std::vector<Expression> & args)
{
	if (args.size() != 1) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == BooleanType)
	{
		return Expression(!args[0].head.value.bool_value);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression isAnd(const std::vector<Expression> & args)
{
	bool result = true;
	if (args.empty()) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	for (auto & a : args)
	{
		if (a.head.type == BooleanType)
			result = result & a.head.value.bool_value;
		else
			throw InterpreterSemanticError("Error: Invalid argument type");
	}
	return Expression(result);
}

Expression isOr(const std::vector<Expression> & args)
{
	bool result = false;
	if (args.empty()) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	for (auto & a : args)
	{
		if (a.head.type == BooleanType)
			result = result | a.head.value.bool_value;
		else
			throw InterpreterSemanticError("Error: Invalid argument type");
	}
	return Expression(result);
}

Expression less(const std::vector<Expression> & args)
{
	bool result = false;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = args[0].head.value.num_value < args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression lesseq(const std::vector<Expression> & args)
{
	bool result = false;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = args[0].head.value.num_value <= args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression greater(const std::vector<Expression> & args)
{
	bool result = false;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = args[0].head.value.num_value > args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression greatereq(const std::vector<Expression> & args)
{
	bool result = false;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = args[0].head.value.num_value >= args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression equals(const std::vector<Expression> & args)
{
	bool result = false;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
        result = (fabs(args[0].head.value.num_value - args[1].head.value.num_value) < std::numeric_limits<double>::epsilon());
		//result = args[0].head.value.num_value == args[1].head.value.num_value;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression mySin(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() != 1) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");

	}
	if (args[0].head.type == NumberType)
	{
		result = std::sin(args[0].head.value.num_value);
		if(args[0].head.value.num_value == PI) result = 0;
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression myCos(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() != 1) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType)
	{
		result = std::cos(args[0].head.value.num_value);
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression myarcTan(const std::vector<Expression> & args)
{
	double result = 0.0;
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		result = std::atan2(args[0].head.value.num_value, args[1].head.value.num_value);
		return Expression(result);
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression makePoint(const std::vector<Expression> & args)
{
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == NumberType && args[1].head.type == NumberType)
	{
		Expression result;
		result.head.type = PointType;
		result.head.value.point_value.x = args[0].head.value.num_value;
		result.head.value.point_value.y = args[1].head.value.num_value;
		return result;
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression makeLine(const std::vector<Expression> & args)
{
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == PointType && args[1].head.type == PointType)
	{
		Expression result;
		result.head.type = LineType;
		result.head.value.line_value.first = args[0].head.value.point_value;
		result.head.value.line_value.second = args[1].head.value.point_value;
		return result;
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

Expression makeArc(const std::vector<Expression> & args)
{
	if (args.size() != 3) {
		throw InterpreterSemanticError("Error: Invalid number or arguments");
	}
	if (args[0].head.type == PointType && args[1].head.type == PointType && args[2].head.type == NumberType)
	{
		Expression result;
		result.head.type = ArcType;
		result.head.value.arc_value.center = args[0].head.value.point_value;
		result.head.value.arc_value.start = args[1].head.value.point_value;
		result.head.value.arc_value.span = args[2].head.value.num_value;
		return result;
	}
	throw InterpreterSemanticError("Error: Invalid argument type");
}

//This checks if an atom exists in the map
bool Environment::is_known(const Atom & pAtom) const {
	if (pAtom.type != SymbolType) return false;

	return ((envmap.find(pAtom.value.sym_value) != envmap.end()));
}

//This checks if an atom is an expression in the
// environment or not
bool Environment::is_exp(const Atom & pAtom) const {
	if (pAtom.type != SymbolType) return false;
	auto result = envmap.find(pAtom.value.sym_value);
	//printMap();
	return (result != envmap.end()) && (result->second.type == ExpressionType);
}

//This checks prints the environment map
// *was used mainly for debugging
void Environment::printMap() const {
	for (auto it : envmap) {
		std::cout << it.first << std::endl;
	}
}

//This will get the expression for a given atom
// from the environment map
Expression Environment::get_exp(const Atom & pAtom) const {
	Expression exp;
	if (pAtom.type == SymbolType) {
		auto result = envmap.find(pAtom.value.sym_value);
		if ((result != envmap.end()) && (result->second.type == ExpressionType)) {
			exp = result->second.exp;
		}
	}
	else if (pAtom.type == NumberType && pAtom.value.num_value == std::atan2(0, -1)) {
		auto result = envmap.find("pi");
		if ((result != envmap.end()) && (result->second.type == ExpressionType)) {
			exp = result->second.exp;
		}
	}
	return exp;
}

//This will add an atom and expression into
// the environment map (used to handle define)
void Environment::add_exp(const Atom & pAtom, const Expression & exp) {
	if (!(pAtom.type == SymbolType)) {
		throw InterpreterSemanticError("Error: Attempt to add non-symbol to environment.");
	}
	if (envmap.find(pAtom.value.sym_value) != envmap.end()) {
		throw InterpreterSemanticError("Error: Attempt to overwrite symbol in environment.");
	}
	envmap.emplace(pAtom.value.sym_value, EnvResult(ExpressionType, exp));
	//std::cout << envmap.find(pAtom.value.sym_value)->first << " " <<  envmap.find(pAtom.value.sym_value)->second.exp.head.value.num_value <<std::endl;
}

//This checks if an atom is a procedure defined in the
// environment or not
bool Environment::is_proc(const Atom & pAtom) const {
	if (pAtom.type != SymbolType) return false;

	auto result = envmap.find(pAtom.value.sym_value);
	return (result != envmap.end()) && (result->second.type == ProcedureType);
}

//This will get the procedure for a given atom
// from the environment map
Procedure Environment::get_proc(const Atom & pAtom) const {

	if (pAtom.type == SymbolType) {
		auto result = envmap.find(pAtom.value.sym_value);
		if ((result != envmap.end()) && (result->second.type == ProcedureType)) {
			return result->second.proc;
		}
	}

	throw InterpreterSemanticError("Error: invalid procedure");
}

//This will reset the environment when necessary
void Environment::reset() {
	envmap.clear();
	envmap.emplace("not", EnvResult(ProcedureType, isNot));//Inverse of expression
	envmap.emplace("and", EnvResult(ProcedureType, isAnd));//And all arguments
	envmap.emplace("or", EnvResult(ProcedureType, isOr));//Or all arguments
	envmap.emplace("<", EnvResult(ProcedureType, less));//True if first arg is less than second
	envmap.emplace("<=", EnvResult(ProcedureType, lesseq));//True if first arg is less than or equal to second
	envmap.emplace(">", EnvResult(ProcedureType, greater));//True if first arg is greater than second
	envmap.emplace(">=", EnvResult(ProcedureType, greatereq));//True if first arg is greater than or equal to second
	envmap.emplace("=", EnvResult(ProcedureType, equals));
	envmap.emplace("+", EnvResult(ProcedureType, add));//adds set of numbers together
	envmap.emplace("-", EnvResult(ProcedureType, subneg));//handles subtracting two numbers and negating numbers
	envmap.emplace("*", EnvResult(ProcedureType, mult));//mulitply a set of numbers
	envmap.emplace("/", EnvResult(ProcedureType, divide));//divide a two numbers
	envmap.emplace("log10", EnvResult(ProcedureType, logs));// finds logarithm of a value
	envmap.emplace("pow", EnvResult(ProcedureType, power));// power of two numbers
	envmap.emplace("pi", EnvResult(ExpressionType, Expression(PI)));// store value of pi
	envmap.emplace("sin", EnvResult(ProcedureType, mySin));// finds sin of value
	envmap.emplace("cos", EnvResult(ProcedureType, myCos));// finds cos of value
	envmap.emplace("arctan", EnvResult(ProcedureType, myarcTan));// finds inverse tan of values
	envmap.emplace("point", EnvResult(ProcedureType, makePoint));// Creates a Point
	envmap.emplace("line", EnvResult(ProcedureType, makeLine));// Creates a Line
	envmap.emplace("arc", EnvResult(ProcedureType, makeArc));// Creates an Arc
}
void Environment::save() {
	revmap = envmap;
}
void Environment::revert() {
	envmap = revmap;
}



//Constructor for environment
Environment::Environment() {
	reset();

}
