#pragma once
#include <exception>

namespace ak47
{

	template<typename TData>
	class UnionFind
	{
		TData data;
		UnionFind<TData>* parent;
	public:
		class UnionFindException : public std::exception
		{
		public:
			UnionFindException(const char* msg)
				:std::exception(msg)
			{

			}
		};
		UnionFind();
		UnionFind(bool makeSet = true);
		UnionFind(const TData& data);
		~UnionFind();

		// Is in any set?
		operator bool() const;

		void MakeSet();
		bool IsInSameSetAs(const UnionFind* other) const;
		void Union(UnionFind* other);
		const UnionFind* Parent() const;
		const TData& Get() const;
	};

	template<typename TData>
	inline UnionFind<TData>::UnionFind()
		:parent(nullptr)
	{
	}

	template<typename TData>
	inline UnionFind<TData>::UnionFind(bool makeSet)
		: parent(nullptr)
	{
		if (makeSet)
		{
			this->MakeSet();
		}
	}

	template<typename TData>
	inline UnionFind<TData>::UnionFind(const TData& data)
		:parent(nullptr)
	{
		this->data = data;
		this->MakeSet();
	}

	template<typename TData>
	inline UnionFind<TData>::~UnionFind()
	{
	}

	template<typename TData>
	inline UnionFind<TData>::operator bool() const
	{
		return this->parent != nullptr;
	}

	template<typename TData>
	inline void UnionFind<TData>::MakeSet()
	{
		this->parent = this;
	}

	template<typename TData>
	inline bool UnionFind<TData>::IsInSameSetAs(const UnionFind* o) const
	{
		if (!(*this) || !(*o))
		{
			return false;
		}
		const UnionFind* self = this;
		const UnionFind* other = o;

		while (self->Parent() != self) self = self->Parent();
		while (other->Parent() != other) other = other->Parent();

		return self == other;
	}

	template<typename TData>
	inline void UnionFind<TData>::Union(UnionFind* o)
	{
		if (!(*this) || !(*o))
		{
			throw UnionFindException("Can not union not initialized sets");
		}
		UnionFind* self = this;
		UnionFind* other = o;
		size_t a = 0, b = 0;
		while (self->parent != self)
		{
			self = self->parent;
			++a;
		}

		while (other->parent != other)
		{
			other = other->parent;
			++b;
		}

		if (a < b)
		{
			self->parent = other;
		}
		else
		{
			other->parent = self;
		}
	}

	template<typename TData>
	inline const UnionFind<TData>* UnionFind<TData>::Parent() const
	{
		return this->parent;
	}

	template<typename TData>
	inline const TData& UnionFind<TData>::Get() const
	{
		return this->data;
	}
}