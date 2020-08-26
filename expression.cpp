#include "expression.hpp"


// system includes
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include <tuple>
#include <iostream>

Expression::Expression(bool tf) {
	head.value.bool_value = tf;
	head.type = BooleanType;
}

Expression::Expression(double num) {
	head.value.num_value = num;
	head.type = NumberType;
}

Expression::Expression(const std::string & sym) {
	head.value.sym_value = sym;
	head.type = SymbolType;
}

Expression::Expression(std::tuple<double,double> value){
	head.value.point_value.x = std::get<0>(value);
	head.value.point_value.y = std::get<1>(value);
	head.type = PointType;
}

Expression::Expression(std::tuple<double,double> start,
		       std::tuple<double,double> end){
	head.value.line_value.first.x = std::get<0>(start);
	head.value.line_value.first.y = std::get<1>(start);
	head.value.line_value.second.x = std::get<0>(end);
	head.value.line_value.second.y = std::get<1>(end);
	head.type = LineType;
}


Expression::Expression(std::tuple<double,double> center,
		       std::tuple<double,double> start,
		       double angle){
	head.value.arc_value.start.x = std::get<0>(start);
	head.value.arc_value.start.y = std::get<1>(start);
	head.value.arc_value.center.x = std::get<0>(center);
	head.value.arc_value.center.y = std::get<1>(center);
	head.value.arc_value.span = angle;
	head.type = ArcType;
}

bool Expression::operator==(const Expression & exp) const noexcept {
	if (exp.head.type != head.type)  return false;
	switch (exp.head.type) {
		case NoneType: return true;
		break;
		case BooleanType: return (exp.head.value.bool_value == head.value.bool_value && exp.tail.size() == tail.size());
		break;
		case NumberType: return (exp.head.value.num_value == head.value.num_value && exp.tail.size() == tail.size());
		break;
		case SymbolType: return (exp.head.value.sym_value == head.value.sym_value && exp.tail.size() == tail.size());
		break;
		case PointType: return (exp.head.value.point_value == head.value.point_value && exp.tail.size() == tail.size());
		break;
		case LineType: return ((exp.head.value.line_value.first == head.value.line_value.first) && exp.tail.size() == tail.size()
							   && (exp.head.value.line_value.second == head.value.line_value.second));
		break;
		case ArcType: return ((exp.head.value.arc_value.center == head.value.arc_value.center) && exp.tail.size() == tail.size()
							  && (exp.head.value.arc_value.start == head.value.arc_value.start)
							  && (exp.head.value.arc_value.span == head.value.arc_value.span));
		break;
		default: return false;
		break;
	}

	return false;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp) {
	out << "(";
	if (exp.tail.size() == 0)
	{
		switch (exp.head.type) {
		case NumberType:  out << exp.head.value.num_value;
			break;
		case SymbolType:  out << exp.head.value.sym_value;
			break;
		case BooleanType: {
			if (exp.head.value.bool_value)
				out << "True";
			else
				out << "False";
		} break;
		case PointType: {
			out << exp.head.value.point_value.x << ",";
			out << exp.head.value.point_value.y;
		} break;
		case LineType: {
			out << "(" << exp.head.value.line_value.first.x << "," << exp.head.value.line_value.first.y << "),";
			out << "(" << exp.head.value.line_value.second.x << "," << exp.head.value.line_value.second.y << ")";
		} break;
		case ArcType: {
			out << "(" << exp.head.value.arc_value.center.x << "," << exp.head.value.arc_value.center.y << "),";
			out << "(" << exp.head.value.arc_value.start.x << "," << exp.head.value.arc_value.start.y << ") ";
			out << exp.head.value.arc_value.span;
		} break;
		default:  out << "None";
			break;//Error try to print NoneType
		}
	}
	else
	{
		for (auto nextExp : exp.tail)
		{
			out << nextExp;
		}
	}
	out << ")";
	return out;
}

//This function checks if two strings are equal regardless
// of their case
bool iequals(std::string ins, std::string b)
{
	std::transform(ins.begin(), ins.end(), ins.begin(), ::toupper);
	std::transform(b.begin(), b.end(), b.begin(), ::toupper);
	if (ins == b) return true;
	return false;

}

//Checks if a string is a valid double
bool isNum(std::string checks) {
    std::istringstream iss(checks);
    double d;
    iss >> d;
    return iss.eof() && !iss.fail();
}

//Will convert a string token into an atom
bool token_to_atom(const std::string & token, Atom & atom) {
	// TODO: implement this function
	// return true if it a token is valid. otherwise, return false.
	double temp;
	std::istringstream iss(token);
    //if(iequals(token, "1abc")) std::cout << token << std::endl;
	if (iequals(token, "true"))
	{
		atom.type = BooleanType;
		atom.value.bool_value = true;
		return true;
	}
	else if (iequals(token, "false"))
	{
		atom.type = BooleanType;
		atom.value.bool_value = false;
		return true;
	}
	else if (iequals(token, "pi"))
	{
		atom.type = NumberType;
		atom.value.num_value = atan2(0, -1);
		return true;
	}
	else if (isNum(token)) {
	    iss >> temp;
	    atom.type = NumberType;
	    atom.value.num_value = temp;
	    return true;
	}
	else
	{
		if (isdigit(token[0]))
		{
			atom.type = NoneType;
			return false;
		}
		else
		{
			atom.type = SymbolType;
			atom.value.sym_value = token;
			return true;
		}
	}
}

