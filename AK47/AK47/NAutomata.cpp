#include "pch.h"

ak47::NAutomata::NAutomata()
{
	this->Init("", 1, 0, { });
}

ak47::NAutomata::NAutomata(const Automata& other)
{
	this->A.reserve(other.nQ());
	size_t n = other.nQ();
	for (size_t i = 0; i < n; i++)
	{
		this->A.push_back(std::move(NAutomata::Q()));
	}
	this->SetStart(other.GetStart());
	this->SetFinals(other.GetFinals());
	this->alphabet = other.Alphabet();
	vector<size_t> tmp;
	for (size_t i = 0; i < n; i++)
	{
		for (const auto& q : other.Qs()[i].q)
		{
			tmp.push_back(q.second);
			this->fDelta(i, q.first, tmp, true);
			tmp.clear();
		}
	}
}

ak47::NAutomata::NAutomata(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals)
{
	this->Init(alphabet, nQ, start, Finals);
}

ak47::NAutomata::NAutomata(char c)
{
	this->start = 0;
	this->A.reserve(2);
	this->A.push_back(std::move(NAutomata::Q()));
	this->A.push_back(std::move(NAutomata::Q()));
	this->A.back().isFinal = true;
	this->A[0].q[c].push_back(1);
	this->alphabet = c;
	this->isInit = true;
}

void ak47::NAutomata::Init(const string& alphabet, size_t nQ, size_t start, const vector<size_t>& Finals)
{
	if (this->isInit)
	{
		this->A.clear();
	}
	this->alphabet = alphabet;
	this->SetStart(start);
	this->A.reserve(nQ);
	for (size_t i = 0; i < nQ; i++)
	{
		this->A.push_back(std::move(NAutomata::Q()));
	}
	this->SetFinals(Finals);
	this->isInit = true;
}

void ak47::NAutomata::operator()(size_t from, char with, const vector<size_t>& to)
{
	this->fDelta(from, with, to, false);
}

ak47::NAutomata ak47::NAutomata::operator()(size_t p) const
{
	if (p < this->nQ())
	{
		NAutomata P = *this;
		P.SetStart(p);
		return P;
	}
	else
	{
		throw std::exception("Invalid Parameter for .operator()(size_t p)");
	}
}

void ak47::NAutomata::SetStart(size_t start)
{
	if (this->A.size() > start)
	{
		this->start = start;
	}
}

void ak47::NAutomata::SetFinals(const vector<size_t> Finals)
{
	size_t n = this->A.size();
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

std::vector<size_t> ak47::NAutomata::GetFinals() const
{
	std::vector<size_t> result;
	size_t n = this->Qs().size();
	result.reserve(n);
	for (size_t i = 0; i < n; i++)
	{
		if (this->Qs()[i].isFinal)
		{
			result.push_back(i);
		}
	}

	return result;
}

bool ak47::NAutomata::IsFinal(size_t q) const
{
	if (q < this->nQ())
	{
		return this->A[q].isFinal;
	}
	return false;
}

bool ak47::NAutomata::Matches(const string& str) const
{
	if (str.empty() && A[start].isFinal)
	{
		return true;
	}
	else
	{
		vector<size_t> reachable;
		reachable.push_back(start);
		vector<size_t> next_reachable;

		size_t n = str.size();
		char with;
		for (size_t i = 0; i < n; ++i)
		{
			with = str[i];
			next_reachable.clear();
			for (auto& from : reachable)
			{
				auto iter = this->A[from].q.find(with);
				if (iter != this->A[from].q.end())
				{
					const vector<size_t>& tmp = iter->second;
					for (const auto& k : tmp)
					{
						next_reachable.push_back(k);
					}
				}
			}
			reachable.swap(next_reachable);
		}
		for (auto& i : reachable)
		{
			if (A[i].isFinal)
			{
				return true;
			}
		}
		return false;
	}
}

ak47::NAutomata & ak47::NAutomata::KleeneStar()
{
	size_t n = this->nQ();
	for (size_t i = 0; i < n; ++i) // iterate Qs of A
	{
		if (this->A[i].isFinal && i != this->GetStart()) // every final that is not the start will imitate start
		{
			for (auto& q : this->A[this->GetStart()].q) // iterate map of start of A
			{
				vector<size_t>& rf = this->A[i].q[q.first];
				rf.insert(rf.end(), q.second.begin(), q.second.end());
			}
		}
	}

	this->A.push_back(NAutomata::Q());
	for (auto& q : this->A[this->GetStart()].q)
	{
		vector<size_t>& rf = this->A.back().q[q.first];
		rf.insert(rf.end(), q.second.begin(), q.second.end());
	}
	this->SetStart(this->nQ() - 1);
	this->A.back().isFinal = true;
	return *this;
}

ak47::NAutomata& ak47::NAutomata::ConcatWith(const NAutomata& B)
{
	size_t n = B.Alphabet().size();
	for (size_t i = 0; i < n; i++)
	{
		if (this->alphabet.find(B.Alphabet()[i]) == string::npos) // if there is a new letter that should be added
		{
			this->alphabet += B.Alphabet()[i];
		}
	}
	vector<size_t> finals = B.GetFinals();
	for (size_t i = 0; i < finals.size(); i++)
	{
		finals[i] += this->nQ(); // Qs of B are numbered after those of this
	}
	size_t old_nQ = this->nQ();
	this->A.insert(this->A.end(), B.A.begin(), B.A.end());
	
	// Expanding fdelta with B
	vector<size_t> tmp;
	for (size_t i = 0; i < B.nQ(); ++i)
	{
		for (auto& q : B.Qs()[i].q)
		{
			tmp = q.second;
			for (size_t k = 0; k < tmp.size(); ++k)
			{
				tmp[k] += old_nQ;
			}
			this->fDelta(i + old_nQ, q.first, tmp, true);
		}
	}

	for (size_t i = 0; i < old_nQ; ++i) // expanding the fDelta to link A with B
	{
		if (this->Qs()[i].isFinal) // for each final of A, imitate start of B
		{
			for (auto& j : B.Qs()[B.GetStart()].q) // iterate map of start of B
			{
				tmp = j.second;
				for (size_t k = 0; k < tmp.size(); ++k)
				{
					tmp[k] += old_nQ;
				}
				this->fDelta(i, j.first, tmp, true);
			}
		}
	}
	if (!B.IsFinal(B.GetStart())) // if B start is Final we can match every word that this mathes
	{
		this->SetFinals({});
	}
	for (auto& i : finals)
	{
		this->A[i].isFinal = true;
	}
	return *this;
}

// Union of this and B
ak47::NAutomata& ak47::NAutomata::operator+=(const NAutomata& B)
{
	size_t old_nQ = this->nQ();
	size_t n = B.nQ();
	// Add Qs of B to those of this
	for (size_t i = 0; i < n; i++)
	{
		this->A.push_back(B.Qs()[i]);
		for (auto& q : this->A.back().q)
		{
			for (size_t k = 0; k < q.second.size(); k++)
			{
				q.second[k] += old_nQ;
			}
		}
	}
	this->A.push_back(NAutomata::Q()); // addind the new start
	this->A.back().isFinal = (B.IsFinal(B.GetStart()) || this->IsFinal(this->GetStart()));
	for (auto& i : this->A[this->GetStart()].q)
	{
		this->fDelta(this->nQ() - 1, i.first, i.second, true);
	}
	vector<size_t> tmp;
	for (auto& i : B.Qs()[B.GetStart()].q)
	{
		tmp = i.second;
		size_t n = tmp.size();
		for (size_t k = 0; k < n; k++)
		{
			tmp[k] += old_nQ;
		}
		vector<size_t>& rf = this->A.back().q[i.first];
		rf.insert(rf.end(), tmp.begin(), tmp.end());
	}
	this->SetStart(this->nQ() - 1);

	n = B.Alphabet().size();
	for (size_t i = 0; i < n; i++)
	{
		if (this->alphabet.find(B.Alphabet()[i]) == string::npos) // if there is a new letter that should be added
		{
			this->alphabet += B.Alphabet()[i];
		}
	}

	return *this;
}

ak47::NAutomata ak47::NAutomata::operator*() const
{
	NAutomata A = *this;

	A.KleeneStar();
	
	return A;
}

ak47::NAutomata ak47::NAutomata::operator^(size_t p) const
{
	NAutomata power = *this;
	power ^= p;
	return power;
}

ak47::NAutomata& ak47::NAutomata::operator^=(size_t p)
{
	if (p == 0) // match epsilon only
	{
		this->Init("", 1, 0, { 0 });
	}
	else
	{
		NAutomata cpy = *this;
		for (size_t i = 0; i < p - 1; ++i)
		{
			this->ConcatWith(cpy);
		}
	}
	
	return *this;
}

void ak47::NAutomata::fDelta(size_t from, char with, const vector<size_t>& t, bool force)
{
	if (!force)
	{
		if (alphabet.find(with) == string::npos)
		{
			throw std::exception("Not in the alpthabet");
		}
	}

	for (auto& to : t)
	{
		if (from < A.size() || to < A.size())
		{
			A[from].q[with].push_back(to);
		}
		else
		{
			throw std::exception("Invalid parameters 'from' and/or 'to'");
		}
	}
}

ak47::NAutomata ak47::operator<=(const NAutomata& A, const NAutomata& B)
{
	NAutomata AB = A;
	AB.ConcatWith(B);
	return AB;
}

ak47::NAutomata ak47::operator+(const NAutomata& A, const NAutomata& B)
{
	NAutomata AB = A;
	AB += B;
	return AB;
}
