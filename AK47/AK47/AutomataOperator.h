#pragma once
#include "NAutomata.h"

#include <vector>
#include <memory>
#include <stack>

namespace ak47
{
	struct Unit
	{
		std::string regex;
		std::shared_ptr<NAutomata> automata;
	};

	class AutomataOperator
	{
	public:
		class Exception
			:public std::exception
		{
		public:
			Exception(const char* const message, const char* const thrown_by);

			inline const std::string& ThrownBy() const;
		protected:
			std::string thrown_by;

		};
		AutomataOperator(char c);

		virtual ~AutomataOperator();

		inline bool operator==(const AutomataOperator& b);
		inline bool operator==(char c);

		virtual bool operator()(std::stack<Unit> * the_stack) = 0;

	protected:
		char symbol;
	};

	class AutomataOperators
	{
	public:
		static AutomataOperators& TheInstance();
		std::shared_ptr<AutomataOperator> Get(char c);
		bool OperateOnStack(std::stack<Unit>* stack_ptr);

		bool IsValidOperator(char c);
		void RegisterOperator(std::shared_ptr<AutomataOperator> op);
		
		AutomataOperators(const AutomataOperators&&) = delete;
		AutomataOperators(const AutomataOperators&)  = delete;
		void operator=(const AutomataOperators&)	 = delete;
		void operator=(const AutomataOperators&&)	 = delete;
	private:
		AutomataOperators();

		std::vector<std::shared_ptr<AutomataOperator>> operators;
	};

}