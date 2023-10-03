/*
15. Трассировка  программы,   не   содержащей   рекурсивных
вызовов  и  повторяющихся  имен  процедур,  распечатана в виде
списка выполняемых процедур. Процедура попадает в список, если
к ней произошло обращение из вызывающей процедуры либо возврат
управления из  вызванной  ей  процедуры.  Структура  программы
такова,  что  каждая вызываемая процедура вложена в вызывающую
ее процедуру.  Известен объем памяти,  который  требуется  для
загрузки каждой процедуры.  При выходе из процедуры занимаемая
ей   память   освобождается.   Выяснить,   соответствует    ли
трассировка  правильной  работе  программы.  Определить размер
памяти,  необходимый для работы программы,  и цепочку вызовов,
требующую максимальной памяти (8).

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022
C++20
*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <map>
#include <string>
#include <vector>

std::map<std::string, std::string> nested_procedures;
std::map<std::string, int> ram_sizes;

std::string get_nested_procedure( std::string father, std::string name )
{
	auto it = nested_procedures.find( name );
	if ( it == nested_procedures.end( ) )
	{
		nested_procedures.try_emplace( name, father );
		return father;
	}

	return it->second;
}

int get_procedure_size( std::string name )
{
	auto it = ram_sizes.find( name );
	if ( it == ram_sizes.end( ) )
		return -1;

	return it->second;
}

bool init_ram_sizes( std::ifstream& stream )
{
	std::string procedure_name;
	std::string procedure_size;

	while ( stream >> procedure_name )
	{
		if ( !(stream >> procedure_size) )
			return false;

		if ( get_procedure_size( procedure_name ) != -1 )
			return false;

		ram_sizes.try_emplace( procedure_name, std::stoi( procedure_size ) );
	}

	return true;
}

struct TreeNode {
	std::string name = "";
	TreeNode* father = nullptr;
	std::vector<TreeNode*> children;
};

bool is_procedure_exist( TreeNode* tree, std::string name )
{
	TreeNode* father_tree = tree->father;
	while ( father_tree != nullptr )
	{
		if ( father_tree->name == name )
			return true;

		father_tree = father_tree->father;
	}

	return false;
}

void process_trace( std::ifstream& stream, std::ofstream& output )
{
	std::string procedure_name = "";
	TreeNode* prev_tree = nullptr;
	TreeNode* max_ram_tree = nullptr;
	int current_ram_size = 0;
	int max_ram_size = 0;

	while ( stream >> procedure_name )
	{
		TreeNode* tree_node = new TreeNode;
		tree_node->name = procedure_name;
		tree_node->father = prev_tree;

		// Возвращаемся на предыдущий уровень
		if ( is_procedure_exist( tree_node, procedure_name ) )
		{
			if ( tree_node->father->name == procedure_name )
			{
				std::cout << "Wrong trace: `" << tree_node->name << "` recursivly called" << std::endl;
				return;
			}

			if ( tree_node->father->father->name != procedure_name )
			{
				std::cout << "Wrong trace: `" << tree_node->name << "` not returned" << std::endl;
				return;
			}

			int procedure_size = get_procedure_size( tree_node->father->name );
			if ( procedure_size == -1 )
			{
				std::cout << "Не найден размер для процедуры: `" << tree_node->father->name << "`" << std::endl;
				return;
			}

			current_ram_size -= procedure_size;
			prev_tree = tree_node->father->father;
		}

		// Идём на следующий уровень
		else
		{
			if ( tree_node->father != nullptr && get_nested_procedure( tree_node->father->name, tree_node->name ) != tree_node->father->name )
			{
				std::cout << "Wrong trace: `" << tree_node->name << "` nested from other procedure" << std::endl;
				return;
			}

			// Добавляем текущий узел в список детей отцовского узла, если это не первый уровень
			if ( tree_node->father != nullptr )
				tree_node->father->children.push_back( tree_node );

			int procedure_size = get_procedure_size( tree_node->name );
			if ( procedure_size == -1 )
			{
				std::cout << "Не найден размер для процедуры: `" << tree_node->name << "`" << std::endl;
				return;
			}

			current_ram_size += procedure_size;

			if ( current_ram_size > max_ram_size )
			{
				max_ram_size = current_ram_size;
				max_ram_tree = tree_node;
			}

			prev_tree = tree_node;
		}
	}

	if ( prev_tree == nullptr )
	{
		std::cout << "На вход подан пустой Trace" << std::endl;
		return;
	}

	if ( prev_tree->father != nullptr )
	{
		std::cout << "Wrong trace: `" << prev_tree->father->name << "` not returned" << std::endl;
		return;
	}

	std::vector<std::string> max_ram_trace;
	while ( max_ram_tree != nullptr )
	{
		max_ram_trace.push_back( max_ram_tree->name );
		max_ram_tree = max_ram_tree->father;
	}

	std::cout << "Необходимо памяти: " << max_ram_size << std::endl;
	output << "Необходимо памяти: " << max_ram_size << std::endl;
	std::cout << "Тrace, который займёт больше всего памяти:" << std::endl;
	output << "Тrace, который займёт больше всего памяти:" << std::endl;

	for ( int i = max_ram_trace.size( ) - 1; i >= 0; i-- )
	{
		std::cout << max_ram_trace.at( i ) << std::endl;
		output << max_ram_trace.at( i ) << std::endl;
	}
}

int main( )
{
	SetConsoleCP( 1251 );
	SetConsoleOutputCP( 1251 );

	std::ifstream input_sizes( "input_sizes.txt" );
	std::ifstream input_trace( "input_trace.txt" );
	std::ofstream output( "output.txt" );

	if ( !input_sizes )
	{
		std::cout << "Не удалось открыть файл input_sizes.txt" << std::endl;
		return 1;
	}

	if ( !input_trace )
	{
		std::cout << "Не удалось открыть файл input_trace.txt" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::cout << "Не удалось открыть файл output.txt" << std::endl;
		return 1;
	}

	if ( !init_ram_sizes( input_sizes ) )
	{
		std::cout << "Ошибка при прочтении файла input_sizes.txt" << std::endl;
		return 1;
	}

	process_trace( input_trace, output );
}