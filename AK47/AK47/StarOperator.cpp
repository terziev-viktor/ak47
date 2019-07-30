#include "pch.h"
#include "StarOperator.h"

ak47::StarOperator::StarOperator()
	:AutomataOperator('*')
{

}

bool ak47::StarOperator::operator()(std::stack<Unit>* the_stack)
{
	Unit u = the_stack->top();
	if (u.regex == "*")
	{
		the_stack->pop();
		Unit arg = the_stack->top();
		the_stack->pop();
		arg.automata->KleeneStar();
		arg.regex += u.regex;
		return true;
	}
	return false;
}

static ak47::StarOperator the_operator;