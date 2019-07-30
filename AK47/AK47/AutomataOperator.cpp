#include "pch.h"
#include "AutomataOperator.h"

using namespace ak47;

AutomataOperator::AutomataOperator(char c)
{
	this->symbol = c;
	AutomataOperators::TheInstance().RegisterOperator(std::move(std::shared_ptr<AutomataOperator>(this)));
}

AutomataOperator::~AutomataOperator()
{
}

inline bool AutomataOperator::operator==(const AutomataOperator& b)
{
	return this->symbol == b.symbol;
}

inline bool ak47::AutomataOperator::operator==(char c)
{
	return this->symbol == c;
}

AutomataOperators& ak47::AutomataOperators::TheInstance()
{
	static AutomataOperators the_instance;
	return the_instance;
}

ak47::AutomataOperators::AutomataOperators()
{

}

std::shared_ptr<AutomataOperator> ak47::AutomataOperators::Get(char c)
{
	for (auto& i : AutomataOperators::TheInstance().operators)
	{
		if (*i == c)
		{
			return i;
		}
	}
	return nullptr;
}

bool ak47::AutomataOperators::OperateOnStack(std::stack<Unit>* stack_ptr)
{
	for (auto& op : AutomataOperators::TheInstance().operators)
	{
		if ((*op)(stack_ptr))
		{
			return true; // operated
		}
	}
	// did not operate, something's wrong
	return false;
}

bool ak47::AutomataOperators::IsValidOperator(char c)
{
	for (auto& i : AutomataOperators::TheInstance().operators)
	{
		if (*i == c)
		{
			return true;
		}
	}
	return false;
}

void ak47::AutomataOperators::RegisterOperator(std::shared_ptr<AutomataOperator> op)
{
	this->operators.push_back(std::move(op));
}

ak47::AutomataOperator::Exception::Exception(const char* const message, const char* const thrown_by)
	:std::exception(message)
{
	this->thrown_by = thrown_by;
}

inline const std::string& ak47::AutomataOperator::Exception::ThrownBy() const
{
	 return this->thrown_by; 
}
