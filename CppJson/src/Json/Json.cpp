#include "Json.h"
#include "parser.h"
#include <sstream>

using namespace congb::json;


Json::Json() : m_type(json_null){}

Json::Json(bool value) : m_type(json_bool)
{
	m_value.m_bool = value;
}

Json::Json(int value) : m_type(json_int)
{
	m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double)
{
	m_value.m_double = value;
}

Json::Json(const char* value) : m_type(json_string)
{
	m_value.m_string = new std::string(value);
}

Json::Json(const std::string& value) : m_type(json_string)
{
	m_value.m_string = new std::string(value);
}

Json::Json(Type type) : m_type(type)
{
	switch (m_type) {
		case json_null:
			break;
		case json_bool:
			m_value.m_bool = false;
			break;
		case json_int:
			m_value.m_int = 0;
			break;
		case json_double:
			m_value.m_double = 0.0;
			break;
		case json_string:
			m_value.m_string = new std::string("");
			break;
		case json_array:
			m_value.m_array = new std::vector<Json>();
			break;
		case json_object:
			m_value.m_object = new std::map<std::string, Json>();
			break;
	}
}

Json::Json(const Json& other) : m_type(other.m_type)
{
	m_value = other.m_value;
}

Json::operator bool()
{
	if (m_type != json_bool)
	{
		throw new std::logic_error("type error, not bool value");
	}
	return m_value.m_bool;
}

Json::operator int()
{
	if (m_type != json_int)
	{
		throw new std::logic_error("type error, not int value");
	}
	return m_value.m_int;
}

Json::operator double()
{
	if (m_type != json_double)
	{
		throw new std::logic_error("type error, not double value");
	}
	return m_value.m_double;
}

Json::operator std::string()
{
	if (m_type != json_string)
	{
		throw new std::logic_error("type error, not string value");
	}
	return *(m_value.m_string);
}

// Json& Json::operator [](int index) 
// {
// 	if(m_type != json_array)
// 	{
// 		m_type = json_array;
// 		m_value.m_array = new std::vector<Json>();
// 	}
// 
// 	if (index < 0)
// 	{
// 		throw new std::logic_error("invalid index");
// 	}
// 
// 	int m_size = m_value.m_array->size();
// 	if (index >= m_size) {
// 		m_value.m_array->reserve(index + 1);
// 	}
// 	return m_value.m_array->at(index);
// }

Json& Json::operator [](int index)
{
	if (m_type != json_array)
	{
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}

	int m_size = m_value.m_array->size();
	if (index < 0 || index >= m_size)
	{
		throw new std::logic_error("invalid index");
	}

	return m_value.m_array->at(index);
}

void Json::append(const Json& other) 
{
	if (m_type != json_array) {
		clear();
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}
	m_value.m_array->push_back(other);
}

void Json::operator = (const Json& other)
{
	clear();
	m_type = other.m_type;
	m_value = other.m_value;
}

bool Json::operator == (const Json& other)
{
	if (m_type != other.m_type) return false;
	switch (m_type) {
	case json_null:
		return true;
	case json_bool:
		return m_value.m_bool == other.m_value.m_bool;
	case json_int:
		return m_value.m_int == other.m_value.m_int;
	case json_double:
		return m_value.m_double == other.m_value.m_double;
	case json_string:
		return *(m_value.m_string) == *(other.m_value.m_string);
	case json_array:
		return m_value.m_array == other.m_value.m_array;
	case json_object:
		return m_value.m_object == other.m_value.m_object;
	}
	return false;
}

bool Json::operator != (const Json& other)
{
	return !((*this) == other);
}

void Json::clear()
{
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		{
			delete m_value.m_string;
			break;
		}
	case json_array:
		{
			for (auto it : *(m_value.m_array))
				it.clear();
			delete m_value.m_array;
			break;
		}
	case json_object:
		{
			for (auto it : *(m_value.m_object))
				(it.second).clear();
			delete m_value.m_object;
			break;
		}
	default:
		break;
	}
	m_type = json_null;
}


std::string Json::str() const
{
	std::stringstream ss;

	switch (m_type) {
	case json_null:
		ss << "null";
		break;
	case json_bool:
		ss << m_value.m_bool ? "true" : "false";
		break;
	case json_int:
		ss << m_value.m_int;
		break;
	case json_double:
		ss << m_value.m_double;
		break;
	case json_string:
		ss << '\"' << *(m_value.m_string) << '\"';
		break;
	case json_array:
		{
			ss << '[';
			for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); it++)
			{
				if (it != m_value.m_array->begin())
					ss << ',';
				ss << it->str();
			}
			ss << ']';
			break;
		}
	case json_object:
		{
			ss << '{';
			bool flag = false;
			for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); it++)
			{
				if (it != m_value.m_object->begin())
					ss << ',';
				ss << '\"' << it->first << '\"' << ':' << it->second.str();
			}
			ss << '}';
			break;
		}
	default:
		break;
	}

	return ss.str();
}

Json& Json::operator [] (const char* key)
{
	std::string name(key);
	return (*this)[name];
}

Json& Json::operator [] (const std::string& key) {
	
	if (m_type != json_object)
	{
		clear();
		m_type = json_object;
		m_value.m_object = new std::map<std::string, Json>();
	}
	return (*(m_value.m_object))[key];
}

bool Json::asBool() const
{
	if (m_type != json_bool) 
	{
		throw new std::logic_error("type error, not bool value");
	}
	return m_value.m_bool;
}

int Json::asInt() const
{
	if (m_type != json_int)
	{
		throw new std::logic_error("type error, not int value");
	}
	return m_value.m_int;
}

double Json::asDouble() const
{
	if (m_type != json_double)
	{
		throw new std::logic_error("type error, not double value");
	}
	return m_value.m_double;
}

const std::string& Json::asString() const
{
	if (m_type != json_string)
	{
		throw new std::logic_error("type error, not string value");
	}
	return *(m_value.m_string);
}

bool Json::has(int index)
{
	if (m_type != json_array)
		return false;
	int m_size = m_value.m_array->size();
	if (index >= m_size || index < 0)
		return false;
	return true;
}

bool Json::has(const char* key)
{
	if (m_type != json_object)
		return false;
	if (m_value.m_object->count(key))
		return true;
	return false;
}

bool Json::has(const std::string& key)
{
	return has(key);
}

void Json::remove(int index)
{
	if (m_type != json_array)
		return;
	int m_size = m_value.m_array->size();
	if (index < 0 || index >= m_size)
		return;
	m_value.m_array->at(index).clear();
	m_value.m_array->erase(m_value.m_array->begin() + index);
}

void Json::remove(const char* key)
{
	std::string name(key);
	remove(name);
}

void Json::remove(const std::string& key)
{
	if (m_type != json_object)
		return;
	auto it = m_value.m_object->find(key);
	if (it == m_value.m_object->end())
		return;
	it->second.clear();
	m_value.m_object->erase(it);
}

void Json::parse(const std::string& str)
{
	Parser p;
	p.load(str);
	*this = p.parse();
}
