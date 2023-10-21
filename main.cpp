/*
16. В  листьях  бинарного  дерева  указаны   идентификаторы
переменных,  в других вершинах - знаки арифметических операций
или функции SIN,  COS, TG, CTG, LOG, EXP. Возможны одноместные
операции типа '+' или '-'. В этом случае требуется только один
операнд.    Значения    переменных     известны.     Проверить
синтаксическую  правильность идентификаторов.  Выдать на экран
выражение в инфиксной форме со  скобками. Запросить в  диалоге
значения переменных и определить результат вычисления выражения. (11).

((-(((a1*bar)+c)))-((sin(dors))-e))

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022
C++20
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <map>
#include <string>
#include <cmath>

std::map<std::string, double> variables;

struct TreeNode {
	std::string name = "";
	double value = 0.f;
	int level = 0;

	TreeNode* father = nullptr;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
};

std::string str_to_lower( std::string str )
{
	std::transform( str.begin( ), str.end( ), str.begin( ),
		[]( unsigned char c ) { return std::tolower( c ); } );

	return str;
}

bool is_unary( std::string name )
{
	static const std::string unaries [] = { "sin", "cos", "tg", "ctg", "log", "exp" };

	// lowercase
	name = str_to_lower( name );

	for ( int i = 0; i < sizeof( unaries ) / sizeof( unaries[ 0 ] ); i++ )
		if ( unaries[ i ] == name )
			return true;

	return false;
}

bool is_variable( std::string name )
{
	static const std::string not_vars[] = { "sin", "cos", "tg", "ctg", "log", "exp", "*", "/", "-", "+" };

	// lowercase
	name = str_to_lower( name );

	for ( int i = 0; i < sizeof( not_vars ) / sizeof( not_vars[0]); i++ )
		if ( not_vars[i] == name )
			return false;

	return true;
}

double get_or_create_variable( std::string name )
{
	auto it = variables.find( name );
	if ( it == variables.end( ) ) // переменной не существует - создаём
	{
		double value = 0.F;

		std::cout << "Введите значение переменной " << name << ": ";
		std::cin >> value;
		variables.emplace( name, value );

		return value;
	}

	return it->second;
}

bool init_tree( TreeNode* tree, std::ifstream& input )
{
	std::string str;
	TreeNode* prev_tree = tree;

	while ( input >> str )
	{
		int level = std::count( str.begin(), str.end(), '.' );
		str.erase( 0, level );

		// Самый верх дерева
		if ( level == 0 )
		{
			tree->name = str;
			tree->level = 0;
			continue;
		}

		TreeNode* new_tree = new TreeNode;
		new_tree->level = level;
		new_tree->name = str;
		new_tree->father = prev_tree;

		// Новый ребёнок
		if ( level > prev_tree->level )
		{
			new_tree->father = prev_tree;
			
			if ( prev_tree->left == nullptr )
			{
				prev_tree->left = new_tree;
			}
			else if ( prev_tree->right == nullptr )
			{
				prev_tree->right = new_tree;
			}
			else
			{
				std::cout << "Ошибка! Найдено более 2 детей." << std::endl;
				return false;
			}
		}
		else
		{
			if ( level < prev_tree->level )	// Возвращаемся назад для поиска нужного узла
			{ 
				while ( level <= prev_tree->level )
					prev_tree = prev_tree->father;
			
			}
			else // Подряд заполняем второго ребенка
			{
				new_tree->father = prev_tree->father;
			}

			if ( new_tree->father->right != nullptr )
			{
				std::cout << "Ошибка! Найдено более 2 детей." << std::endl;
				return false;
			}

			new_tree->father->right = new_tree;
		}

		prev_tree = new_tree;

		// Еслифункция является унарной, но у нас несколько детей
		if ( is_unary( new_tree->father->name ) && new_tree->father->right )
		{
			std::cout << "Ошибка! У унарной операции " << prev_tree->name << " несколько операндов" << std::endl;
			return false;
		}

		// Если это переменная - то спрашиваем у юзера её значение
		if ( is_variable( str ) )
			new_tree->value = get_or_create_variable( str );
	}

	return true;
}

double calculate( std::string name, double left, double right )
{
	name = str_to_lower( name );

	if ( name == "sin" )
		return sin( right );
	else if ( name == "cos" )
		return cos( right );
	else if ( name == "tg" )
		return tan( right );
	else if ( name == "ctg" )
		return 1 / tan( right );
	else if ( name == "log" )
		return log( right );
	else if ( name == "exp" )
		return exp( right );
	else if ( name == "*" )
		return left * right;
	else if ( name == "/" )
		return left / right;
	else if ( name == "-" )
		return left - right;
	else if ( name == "+" )
		return left + right;
}

double result_tree( TreeNode* tree, std::string& preview )
{
	if ( !tree )
		return 0;

	// Если узел кончается не переменной
	if ( tree->left == nullptr && tree->right == nullptr && !is_variable( tree->name ) )
	{
		std::cout << "Ошибка! Неправильный синтаксис" << std::endl;
		return -1;
	}

	bool is_unary_operation = !is_variable( tree->name ) && tree->right == nullptr;
	double left_value = 0.F;
	double right_value = 0.F;

	// Меняем местами узлы детей для унарных операций
	if ( is_unary_operation )
	{
		tree->right = tree->left;
		tree->left = new TreeNode;
	}

	{
		if ( !is_variable( tree->name ) )
			preview += "(";

		// Обрабатываем левую ветку
		{
			left_value = result_tree( tree->left, preview );
		}

		// Выводим текущий узел
		{
			preview += tree->name;
		}

		// Обрабатываем правую ветку
		{
			if ( is_unary_operation )
				preview += "(";
	
			right_value = result_tree( tree->right, preview );

			if ( is_unary_operation )
				preview += ")";
		}

		if ( !is_variable( tree->name ) )
			preview += ")";
	}

	// Возвращаем значение текущего узла
	if ( is_variable( tree->name ) )
		return tree->value;
	else
		return calculate( tree->name, left_value, right_value );
}

int main( )
{
	SetConsoleCP( 1251 );
	SetConsoleOutputCP( 1251 );

	std::ifstream input( "input.txt" );
	std::ofstream output( "output.txt" );

	if ( !input )
	{
		std::cout << "Не удалось открыть файл input.txt" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::cout << "Не удалось открыть файл output.txt" << std::endl;
		return 1;
	}

	TreeNode* tree = new TreeNode;

	if (init_tree( tree, input ))
	{
		std::string preview = "";
		double result = result_tree( tree, preview );

		std::cout << preview << std::endl;
		output << preview << std::endl;

		std::cout << "Результат вычислений: " << result << std::endl;
		output << "Результат вычислений: " << result << std::endl;
	}
}