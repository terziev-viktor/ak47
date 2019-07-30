#pragma once
#include "NAutomata.h"
#include <memory>

namespace ak47
{
	using std::string;

	class Regex
	{
	public:
		Regex(const std::string& regex);
		Regex(const std::string& regex, bool Init);

		bool IsMatch(const std::string& str);

		void Init();

	protected:
		std::string regex;
		std::shared_ptr<ak47::NAutomata> automata;
		bool isInit;
	};

}