/*
14. В файле задана  строка  из  целых  чисел  через  пробел.
Реализовать следующие функции:
   1) построение АВЛ-дерева;
   2) добавление нового элемента с перестроением дерева;
   3) изменение значения заданного элемента с перестроением дерева;
   4) выдачу на экран АВЛ-дерева (11).

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022
C++14
*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>

struct Node {
	int value;
	Node* left = nullptr;
	Node* right = nullptr;
	int height = 1;
};

int get_height( Node* node ) {
	return node == nullptr ? 0 : node->height;
}

// Height(right) - Height(left)
int balance_factor( Node* node ) {
	return node == nullptr ? 0 : (get_height( node->right ) - get_height( node->left ));
}

Node* rotate_right_right( Node* z )
{
	Node* y = z->right;
	Node* T2 = y->left;

	y->left = z;
	z->right = T2;

	y->height = max( get_height( y->left ),
		get_height( y->right ) ) + 1;

	z->height = max( get_height( z->left ),
		get_height( z->right ) ) + 1;

	return y;
}


Node* rotate_left_left( Node* z )
{
	Node* y = z->left;
	Node* T3 = y->right;

	y->right = z;
	z->left = T3;

	y->height = max( get_height( y->left ),
		get_height( y->right ) ) + 1;

	z->height = max( get_height( z->left ),
		get_height( z->right ) ) + 1;

	return y;
}

Node* rotate_left_right( Node* z ) {
	z->left = rotate_right_right( z->left );
	return rotate_left_left( z );
}

Node* rotate_right_left( Node* z ) {
	z->right = rotate_left_left( z->right );
	return rotate_right_right( z );
}

Node* insert_into_tree( Node* node, int value ) {
	// Нашли место для вставки
	if ( node == nullptr ) {
		Node* new_node = new Node;
		new_node->value = value;
		// std::cout << "Вставил " << value << std::endl;
		return new_node;
	}

	// Выбираем, куда пойдём:
	if ( value > node->value )
		node->right = insert_into_tree( node->right, value );
	else if ( value < node->value )
		node->left = insert_into_tree( node->left, value );

	// В этой части кода мы оказались только после вставки нового узла

	// Обновляем высоту
	node->height = max( get_height( node->left ), get_height( node->right ) );
	node->height += 1;

	// std::cout << "Значение: " << node->value << ". Баланс фактор: " << balance_factor( node ) << ". ";

	// Балансируем дерево в случае необходимости
	if ( balance_factor( node ) > 1 && value > node->right->value )
	{
		// std::cout << "RR" << std::endl;
		return rotate_right_right( node );
	}
	else if ( balance_factor( node ) > 1 && value < node->right->value )
	{
		// std::cout << "RL" << std::endl;
		return rotate_right_left( node );
	}
	else if ( balance_factor( node ) < -1 && value < node->left->value )
	{
		// std::cout << "LL" << std::endl;
		return rotate_left_left( node );
	}
	else if ( balance_factor( node ) < -1 && value > node->left->value )
	{
		// std::cout << "LR" << std::endl;
		return rotate_left_right( node );
	}
	else {
		// std::cout << "Без баланса" << std::endl;
		return node;
	}
}

void result_tree( Node* node, std::ofstream& output, int dots_count = 0 )
{
	if ( node == nullptr ) return;

	std::cout << std::string( dots_count, '.' ) << node->value << std::endl;
	output << std::string( dots_count, '.' ) << node->value << std::endl;

	result_tree( node->left, output, dots_count + 1 );
	result_tree( node->right, output, dots_count + 1 );
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

	std::vector<int> values {};
	int number;

	while ( input >> number )
	{
		if ( std::find( values.begin( ), values.end( ), number ) != values.end( ) )
		{
			std::cout << "Встретил повтор: " << number << ". Пропускаю." << std::endl;
			continue;
		}

		values.push_back( number );
	}

	std::string input_str;
	while ( input_str != "0" )
	{
		std::cout << "1. Добавить значение" << std::endl;
		std::cout << "2. Изменить значнеие" << std::endl;
		std::cout << "3. Вывести дерево" << std::endl;
		std::cout << "0. Выход" << std::endl;
		std::cout << "> ";
		std::cin >> input_str;

		if ( input_str == "1" ) {
			std::cout << "Введите значение: ";
			
			int new_value;
			std::cin >> new_value;
			
			if ( std::find( values.begin( ), values.end( ), new_value ) != values.end( ) )
			{
				std::cout << "Данное значение уже существует!" << std::endl << std::endl;
				continue;
			}

			values.push_back( new_value );
			std::cout << "Значение " << new_value << " успешно добавлено!" << std::endl;
			output << "Значение " << new_value << " успешно добавлено!" << std::endl;
		}
		else if ( input_str == "2" ) {
			std::cout << "Введите старое значение: ";
			int old_value;
			std::cin >> old_value;

			auto iter = std::find( values.begin( ), values.end( ), old_value );
			if ( iter == values.end( ) )
			{
				std::cout << "Значение не найдено" << std::endl << std::endl;
				continue;
			}

			std::cout << "Введите новое значение: ";
			int new_value;
			std::cin >> new_value;

			if ( std::find( values.begin( ), values.end( ), new_value ) != values.end( ) )
			{
				std::cout << "Значение уже существует!" << std::endl << std::endl;
				continue;
			}

			*iter = new_value;
			std::cout << "Значение " << old_value << " успешно заменено на " << new_value << std::endl;
			output << "Значение " << old_value << " успешно заменено на " << new_value << std::endl;
		}
		else if ( input_str == "3" ) {
			Node* root = nullptr;

			for ( int i = 0; i < values.size(); i++ )
				root = insert_into_tree( root, values.at( i ) );

			result_tree( root, output );
		}

		std::cout << std::endl;
		output << std::endl;
	}

	return 0;
}