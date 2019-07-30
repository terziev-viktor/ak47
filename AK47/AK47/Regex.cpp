#include "pch.h"
#include "Regex.h"
#include "AutomataOperator.h"
#include <stack>
#include <iostream>
using std::stack;
using ak47::Regex;
using ak47::NAutomata;
using std::string;
using ak47::AutomataOperators;
using ak47::AutomataOperator;
using ak47::Unit;

Regex::Regex(const std::string& regex)
{
	this->regex = regex;
	this->isInit = false;
}

ak47::Regex::Regex(const std::string& regex, bool Init)
{
	this->regex = regex;
	this->Init();
}

bool ak47::Regex::IsMatch(const std::string& str)
{
	if (!this->isInit)
	{
		return false;
	}
	return this->automata->Matches(str);
}

bool _Init(string::iterator& iter, const string::iterator& end, std::shared_ptr<NAutomata>& out)
{
	stack<Unit> the_stack;
	the_stack.push(Unit{ std::string("(") });
	while (the_stack.size() > 1)
	{
		char c;
		if (iter < end)
		{
			c = *iter;
		}
		else
		{
			c = ')';
		}
		++iter;

		if (c == '(')
		{
			the_stack.push(Unit{ std::string("(") });
		}
		else if (c == ')')
		{
			try
			{
				if (!AutomataOperators::TheInstance().OperateOnStack(&the_stack))
				{
					return false;
				}
			}
			catch (const AutomataOperator::Exception& e)
			{
				std::cout << e.what() << std::endl;
				return false;
			}
		}
		else
		{
			the_stack.push(Unit{ std::string() += c});
		}
	}

	if (the_stack.size() != 1)
	{
		return false;
	}
	out = the_stack.top().automata;
	return true;
}

void ak47::Regex::Init()
{
	string::iterator pos = this->regex.begin();
	string::iterator end = this->regex.end();
	if (!_Init(pos, end, this->automata))
	{
		throw std::exception("Wrong regex expression");
	}
	this->isInit = true;
}
