/*
19. Проект научно-технической  программы  задан  с  помощью
ориентированного графа. Вершина графа соответствует отдельному
исследованию,  а  дуги   показывают   очередность   выполнения
исследований  (каждое  исследование  может  начаться  не ранее
окончания  предшествующих   исследований).   Продолжительность
каждого исследования известна. Требуется:
   1) проверить граф на отсутствие циклов;
   2) используя топологическую сортировку, найти путь наибольшей
	  трудоемкости (10).
   Путем считается  последовательность  работ,  которые должны
выполняться друг за  другом.  Трудоемкость  пути  -  суммарная
продолжительность работ на этом пути (10).

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
#include <sstream>

std::map<int, std::string> indices;
struct Top;
struct Link;

struct Top {
	std::string name;
	std::vector<Link*> links; // связи, которые выходят из узла
};

struct Link {
	Top* top;
	int time;
};

bool is_study_exist( std::string study )
{
	for ( auto it = indices.begin( ); it != indices.end( ); ++it )
		if ( it->second == study )
			return true;
	return false;
}

int study_to_index( std::string study )
{
	static int next_index = 0;

	for ( auto it = indices.begin( ); it != indices.end( ); ++it )
		if ( it->second == study )
			return it->first;

	indices.try_emplace( next_index, study );
	next_index += 1;

	return next_index - 1;
}

std::vector<std::string> split_string( std::string str, std::string delimetr )
{
	std::string word = "";
	std::vector<std::string> parts;

	while ( str.compare( word ) != 0 )
	{
		auto index = str.find_first_of( delimetr );
		word = str.substr( 0, index );

		str = str.substr( index + 1, str.length( ) );

		if ( word.length( ) == 0 ) {
			continue;
		}

		parts.push_back( word );
	}

	return parts;
}

bool topological_sort_visit( int top_index, bool visited [], std::vector<Top*> graph, std::vector<Top*>& sorted_graph, std::vector<int>& recurse_history )
{
	bool no_cycles = true;
	visited[ top_index ] = true;
	recurse_history.push_back( top_index );

	for ( int i = 0; i < graph.at( top_index )->links.size( ); i++ )
	{
		int link_top_index = study_to_index( graph.at( top_index )->links.at(i)->top->name );

		if ( !visited[ link_top_index ] )
			no_cycles = no_cycles && topological_sort_visit( link_top_index, visited, graph, sorted_graph, recurse_history );

		else if ( std::find( recurse_history.begin(), recurse_history.end(), link_top_index ) != recurse_history.end() )
		{
			std::cout << "Найден цикл: " << graph.at( top_index )->name << " -> " << graph.at(link_top_index)->name << std::endl;
			return false;
		}
	}

	sorted_graph.push_back( graph.at( top_index ) );
	return no_cycles;
}

bool topological_sort( std::vector<Top*> graph, std::vector<Top*>& sorted_graph )
{
	// Создаём массив посещённых вершин
	bool* visited = new bool[ graph.size( ) ];
	for ( int i = 0; i < graph.size( ); i++ )
		visited[ i ] = false;

	// Для каждой непосещённой вершины проходимся по ней и её связям
	for ( int i = 0; i < graph.size( ); i++ )
		if ( !visited[ i ] )
		{
			// Для выявления циклов будем хранить историю текущей рекурсии
			std::vector<int> recurse_history;

			if ( !topological_sort_visit( i, visited, graph, sorted_graph, recurse_history ) )
				return false;
		}

	std::reverse( sorted_graph.begin( ), sorted_graph.end( ) );
	return true;
}

bool init_graph( std::vector<Top*>& graph, std::ifstream& input )
{
	std::string raw_string;

	while ( getline(input, raw_string) )
	{
		std::vector<std::string> string_parts = split_string(raw_string, "|");

		if ( string_parts.size( ) != 3 )
		{
			std::cout << "Ошибка в строке: " << raw_string << ". Найдено более или менее двух делиметров '|'." << std::endl;
			return false;
		}

		int link_time = 0;
		std::string raw_link_time = string_parts.at( 1 );
		raw_link_time.erase( std::remove_if( raw_link_time.begin( ), raw_link_time.end( ), isspace ), raw_link_time.end( ) );

		try {
			link_time = std::stoi( raw_link_time );
		}
		catch ( ... ) {
			std::cout << "Ошибка при конвертации " << string_parts.at( 1 ) << " в число!" << std::endl;
			return false;
		}

		std::string top1_name = string_parts.at( 0 );
		std::string top2_name = string_parts.at( 2 );

		if ( !is_study_exist( top1_name ) )
		{
			Top* top1 = new Top;
			top1->name = top1_name;
			graph.push_back( top1 );
		}

		if ( !is_study_exist( top2_name ) )
		{
			Top* top2 = new Top;
			top2->name = top2_name;
			graph.push_back( top2 );
		}

		int top1_index = study_to_index( top1_name );
		int top2_index = study_to_index( top2_name );

		Link* link = new Link;
		link->top = graph.at( top2_index );
		link->time = link_time;

		graph.at( top1_index )->links.push_back( link );
	}
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

	std::vector<Top*> graph;
	std::vector<Top*> sorted_graph;

	if ( !init_graph( graph, input ) )
	{
		return 1;
	}

	if ( !topological_sort( graph, sorted_graph ) )
	{
		return 1;
	}

	for ( int i = 0; i < sorted_graph.size( ); i++ )
	{
		std::cout << sorted_graph.at( i )->name << std::endl;
	}
}