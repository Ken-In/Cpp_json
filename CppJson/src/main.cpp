#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


#include "Json/Json.h"
#include "Json/parser.h"
using namespace congb::json;

int main() 
{	
	ifstream fin(".\\src\\test.json");
	stringstream ss;
	ss << fin.rdbuf();
	const string& str = ss.str();
	//cout << str << endl;

	Json v;
	v.parse(str);
	cout << v.str() << endl;
}