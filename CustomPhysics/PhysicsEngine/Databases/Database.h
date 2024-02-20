#pragma once

#ifndef Database_H
#define Database_H

#include <unordered_map>

template <class T>
class Database
{
protected:
	std::unordered_map<uint32_t, T> m_database;

public:
	virtual bool Init() = 0;

	struct QueryResult
	{
		bool success;

		// Idea of const to avoid people modifying database entries (force to deep copy if needed)
		const T* data;
	};

	QueryResult GetEntry(uint32_t key) const
	{
		QueryResult result;

		auto it = m_database.find(key);

		if (it == m_database.end())
		{
			result.success = false;
			result.data = nullptr;
		}
		else
		{
			result.success = true;
			result.data = &(it->second);
		}

		return result;
	}

	/*
	* TODO: Think if we want to have such exposed API for every database
	QueryResult AddEntry(uint32_t key, T data)
	{
		QueryResult result;

		auto it = m_database.find(key);

		// There is identical entry
		if (it != m_database.end())
		{
			result.success = false;
			result.data = nullptr;
			return result;
		}

		m_database[key] = data;
		result.success = true;
		result.data = &m_database[key];
		return result;
	}

	QueryResult ReplaceEntry(uint32_t key, T data)
	{
		QueryResult result;

		m_database[key] = data;
		result.success = true;
		result.data = &m_database[key];

		return result;
	}
	*/
};

#endif