#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace ak47
{
	using std::unordered_map;
	using std::vector;
	using std::string;

	class Automata
	{
	public:
		struct Q
		{
			unordered_map<char, size_t> q;
			bool isFinal;
			Q() :isFinal(false){}
		};

		Automata();

		Automata(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals);

		Automata(char c);

		void operator()(size_t from, char with, size_t to);

		void SetStart(size_t start);

		void SetFinals(const vector<size_t> Finals);

		vector<size_t> GetFinals() const;

		bool IsFinal(size_t q) const;

		inline const string& Alphabet() const { return this->alphabet; }

		inline size_t nQ() const { return this->A.size(); }

		inline size_t GetStart() const { return this->start; }

		bool Matches(const string& str) const;

		bool IsTotal() const;

		inline const vector<Automata::Q> & Qs() const { return this->A; }

	protected:
		vector<Q> A;
		size_t start;
		string alphabet;
		bool isInit;
		void fDelta(size_t from, char with, size_t to, bool force);
	private:
		void Init(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals);
	};

	// Concatination of A and B
}
