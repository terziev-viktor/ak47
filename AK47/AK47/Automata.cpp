#include "pch.h"
#include "Automata.h"

ak47::Automata::Automata()
{
	this->start = 0;
	this->A.push_back(std::move(Automata::Q()));
	this->isInit = true;
}

ak47::Automata::Automata(const string& alphabet, size_t nQ, size_t start, const vector<size_t> & Finals)
{
	this->Init(alphabet, nQ, start, Finals);
}

ak47::Automata::Automata(char c)
{
	this->start = 0;
	this->A.reserve(2);
	this->A.push_back(std::move(Automata::Q()));
	this->A.push_back(std::move(Automata::Q()));
	this->A.back().isFinal = true;
	this->A[0].q[c] = 1;
	this->alphabet = c;
}

void ak47::Automata::Init(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals)
{
	if (!isInit)
	{
		this->alphabet = alphabet;
		this->start = start;
		A.reserve(nQ);
		size_t n = alphabet.size();
		for (size_t i = 0; i < nQ; i++)
		{
			A.push_back(Automata::Q());
			A[i].q.reserve(n);
		}
		this->SetFinals(Finals);
		this->isInit = true;
	}
}

void ak47::Automata::operator()(size_t from, char with, size_t to)
{
	this->fDelta(from, with, to, false);
}

void ak47::Automata::SetStart(size_t start)
{
	if (start < A.size())
	{
		this->start = start;
	}
}

void ak47::Automata::SetFinals(const vector<size_t> Finals)
{
	size_t n = A.size();
	for (auto& i : A)
	{
		i.isFinal = false;
	}
	for (auto& i : Finals)
	{
		if (i < n)
		{
			A[i].isFinal = true;
		}
	}
}

std::vector<size_t> ak47::Automata::GetFinals() const
{
	std::vector<size_t> finals;
	size_t n = this->A.size();
	for (size_t i = 0; i < n; i++)
	{
		if (this->A[i].isFinal)
		{
			finals.push_back(i);
		}
	}
	return finals;
}

bool ak47::Automata::IsFinal(size_t q) const
{
	if (q < this->A.size())
	{
		return this->A[q].isFinal;
	}
	return false;
}

bool ak47::Automata::Matches(const string& str) const
{
	size_t index = this->start;
	size_t n = str.size();
	char ch;
	for (size_t i = 0; i < n; ++i)
	{
		ch = str[i];
		auto next = A[index].q.find(ch);
		if (next == A[index].q.end())
		{
			return false;
		}
		index = next->second;
	}
	return A[index].isFinal;
}

bool ak47::Automata::IsTotal() const
{
	size_t n = alphabet.size();
	for (auto& i : A)
	{
		if (i.q.size() != n)
		{
			return false;
		}
	}
	return true;

}

void ak47::Automata::fDelta(size_t from, char with, size_t to, bool force)
{
	if (!force)
	{
		if (alphabet.find(with) == string::npos)
		{
			throw std::exception("Not in the alpthabet");
		}
	}

	if (from < A.size() || to < A.size())
	{
		A[from].q[with] = to;
	}
	else
	{
		throw std::exception("Invalid parameters 'from' and/or 'to'");
	}
}
