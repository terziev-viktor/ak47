#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Automata.h"

namespace ak47
{
	using std::vector;
	using std::unordered_map;
	using std::string;

	// Non deterministic Automata
	class NAutomata
	{
	public:
		struct Q
		{
			unordered_map<char, vector<size_t>> q;
			bool isFinal;
			Q() :isFinal(false) {}
		};

		NAutomata();

		NAutomata(const Automata& other);

		NAutomata(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals);

		NAutomata(char c);

		void operator()(size_t from, char with, const vector<size_t> & to);

		NAutomata operator()(size_t p) const;

		void SetStart(size_t start);

		inline size_t GetStart() const { return this->start; }

		void SetFinals(const vector<size_t> Finals);

		vector<size_t> GetFinals() const;

		bool IsFinal(size_t q) const;

		inline const string& Alphabet() const { return this->alphabet; }

		bool Matches(const string& str) const;

		inline const vector<NAutomata::Q>& Qs() const { return this->A; }

		inline size_t nQ() const { return this->A.size(); }

		NAutomata& KleeneStar();

		NAutomata& ConcatWith(const NAutomata& B);

		NAutomata& operator+=(const NAutomata& B);

		NAutomata operator*() const;

		NAutomata operator^(size_t p) const;

		NAutomata& operator^=(size_t p);

	protected:
		vector<Q> A;
		size_t start;
		string alphabet;
		bool isInit;
		void fDelta(size_t from, char with, const vector<size_t> & to, bool force);

	private:
		void Init(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals);

	};
	// Concatination of A and B
	NAutomata operator<=(const NAutomata& A, const NAutomata& B);

	// Union of A and B
	NAutomata operator+(const NAutomata& A, const NAutomata& B);
}
