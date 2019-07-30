#pragma once
#include "AutomataOperator.h"

namespace ak47
{
	class UnionOperator :
		public AutomataOperator
	{
	public:
		UnionOperator();

		virtual ~UnionOperator();

		bool operator()(std::stack<Unit> * the_stack) override;
	};
}



