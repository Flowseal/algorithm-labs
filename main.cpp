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
#include <string>
#include <vector>
#include <sstream>

struct Top;
struct Link;

struct Top {
	std::string name;
	std::vector<Link*> links;
	int index;
};

struct Link {
	Top* top = nullptr;
	int time;
};

struct DuoLink {
	Top* top_from = nullptr;
	Top* top_to = nullptr;
	int time;
};

Top* get_top_by_name( std::string name, std::vector<Top*>& vec )
{
	// Ищем существующую вершину
	for ( int i = 0; i < vec.size( ); i++ )
		if ( vec.at( i )->name == name )
			return vec.at( i );

	// Если её нет - создаём новую
	static int next_index = 0;

	Top* top = new Top;
	top->name = name;
	top->index = next_index;

	vec.push_back( top );
	next_index++;
	return top;
}

std::vector<std::string> split_string( std::string str, char delimetr )
{
	std::stringstream stream;
	stream << str;
	std::string segment;
	std::vector<std::string> string_parts;

	while ( std::getline( stream, segment, delimetr ) )
	{
		string_parts.push_back( segment );
	}

	return string_parts;
}

bool topological_sort_visit( int top_index, bool visited [], std::vector<Top*> graph, std::vector<Top*>& sorted_graph, std::vector<int>& history )
{
	bool no_cycles = true;
	visited[ top_index ] = true;
	history.push_back( top_index );

	for ( int i = 0; i < graph.at( top_index )->links.size( ); i++ )
	{
		// Индекс вершины следующей связи
		int link_top_index = graph.at( top_index )->links.at( i )->top->index;

		// Проверяем, посещали ли мы эту вершину за всё время
		if ( !visited[ link_top_index ] )
			no_cycles = topological_sort_visit( link_top_index, visited, graph, sorted_graph, history ) && no_cycles;

		// Проверяем на цикличность: появлялась ли эта вершина в истории текущей рекурсии
		else if ( std::find( history.begin( ), history.end( ), link_top_index ) != history.end( ) )
		{
			std::cout << "Найден цикл: " << graph.at( top_index )->name << " -> " << graph.at( link_top_index )->name << std::endl;
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

	while ( getline( input, raw_string ) )
	{
		std::vector<std::string> string_parts = split_string( raw_string, '|' );

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

		Top* top1 = get_top_by_name( top1_name, graph );
		Top* top2 = get_top_by_name( top2_name, graph );

		Link* link = new Link;
		link->top = top2;
		link->time = link_time;

		top1->links.push_back( link );
	}

	return graph.size( ) > 0;
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

	// Создаём список для вершин и ещё один такой же для отсортированных
	std::vector<Top*> graph;
	std::vector<Top*> sorted_graph;

	// Читаем input.txt и заполняем список вершинами
	if ( !init_graph( graph, input ) )
		return 1;

	// Топологическая сортировка
	if ( !topological_sort( graph, sorted_graph ) )
		return 1;

	std::cout << "Отсортированный список вершин:" << std::endl;
	output << "Отсортированный список вершин:" << std::endl;

	for ( int i = 0; i < sorted_graph.size( ); i++ )
	{
		std::cout << sorted_graph.at( i )->name << std::endl;
		output << sorted_graph.at( i )->name << std::endl;
	}

	// Ищем максимальный путь:

	// 1. Создаём список дуг, где i-элемент - путь наибольшей трудоемкости в эту вершину
	DuoLink** max_paths = new DuoLink *[ sorted_graph.size( ) ];
	for ( int i = 0; i < sorted_graph.size( ); i++ )
	{
		max_paths[ i ] = new DuoLink;
		max_paths[ i ]->time = 0;
	}

	// 2. Заполняем список
	for ( int i = 0; i < sorted_graph.size( ); i++ )
	{
		int curr_index = sorted_graph.at( i )->index;

		for ( Link* link : sorted_graph.at( i )->links )
			if ( max_paths[ curr_index ]->time + link->time > max_paths[ link->top->index ]->time )
			{
				max_paths[ link->top->index ]->time = max_paths[ curr_index ]->time + link->time;
				max_paths[ link->top->index ]->top_from = sorted_graph.at( i );
				max_paths[ link->top->index ]->top_to = link->top;
			}
	}

	// 3. Ищем вершину, путь в которую наиболее трудоёмкий
	DuoLink* max_path = new DuoLink;
	max_path->time = -1;

	for ( int i = 0; i < sorted_graph.size( ); i++ )
		if ( max_paths[ i ]->time > max_path->time )
			max_path = max_paths[ i ];

	std::cout << "\nПуть наибольшей трудоемкости: " << max_path->time << " (";
	output << "\nПуть наибольшей трудоемкости: " << max_path->time << " (";

	std::vector<std::string> path;
	while ( max_path->top_to != nullptr )
	{
		std::string top_from = max_path->top_from->name;
		std::string top_to = max_path->top_to->name;

		path.push_back( top_to );
		max_path = max_paths[max_path->top_from->index];

		if ( max_path->top_to == nullptr )
			path.push_back( top_from );
	}

	std::reverse( path.begin(), path.end() );

	for ( int i = 0; i < path.size( ); i++ )
	{
		std::cout << path.at(i);
		output << path.at( i );

		if ( i + 1 != path.size( ) )
		{
			std::cout << " -> ";
			output << " -> ";
		}
	}

	std::cout << ")" << std::endl;
	output << ")" << std::endl;
}