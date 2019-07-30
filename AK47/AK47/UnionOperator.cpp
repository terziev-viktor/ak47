#include "pch.h"
#include "UnionOperator.h"

ak47::UnionOperator::UnionOperator()
	:AutomataOperator('+')
{
	
}

ak47::UnionOperator::~UnionOperator()
{
}

bool ak47::UnionOperator::operator()(std::stack<Unit>* the_stack)
{
	Unit rightArg = the_stack->top();
	the_stack->pop();
	Unit op = the_stack->top();
	if (op.regex != "+")
	{
		the_stack->push(rightArg);
		return false;
	}
	the_stack->pop();
	Unit leftArg = the_stack->top();
	the_stack->pop();
	(*leftArg.automata) += *rightArg.automata;
	leftArg.regex = leftArg.regex + op.regex + rightArg.regex;
	the_stack->push(leftArg);
	return true;
}

static ak47::UnionOperator the_operator;