#pragma once
#include "AutomataOperator.h"

namespace ak47
{
	class StarOperator
		:public AutomataOperator
	{
		StarOperator();

		bool operator()(std::stack<Unit> * the_stack) override;
	};

}