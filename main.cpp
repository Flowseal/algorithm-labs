/*
19. ������ ������-�����������  ���������  �����  �  �������
���������������� �����. ������� ����� ������������� ����������
������������,  �  ����   ����������   �����������   ����������
������������  (������  ������������  �����  ��������  �� �����
���������  ��������������   ������������).   �����������������
������� ������������ ��������. ���������:
   1) ��������� ���� �� ���������� ������;
   2) ��������� �������������� ����������, ����� ���� ����������
	  ������������ (10).
   ����� ���������  ������������������  �����,  ������� ������
����������� ���� ��  ������.  ������������  ����  -  ���������
����������������� ����� �� ���� ���� (10).

��������: ����� ���� ��-21
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
	// ���� ������������ �������
	for ( int i = 0; i < vec.size( ); i++ )
		if ( vec.at( i )->name == name )
			return vec.at( i );

	// ���� � ��� - ������ �����
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
		// ������ ������� ��������� �����
		int link_top_index = graph.at( top_index )->links.at( i )->top->index;

		// ���������, �������� �� �� ��� ������� �� �� �����
		if ( !visited[ link_top_index ] )
			no_cycles = topological_sort_visit( link_top_index, visited, graph, sorted_graph, history ) && no_cycles;

		// ��������� �� �����������: ���������� �� ��� ������� � ������� ������� ��������
		else if ( std::find( history.begin( ), history.end( ), link_top_index ) != history.end( ) )
		{
			std::cout << "������ ����: " << graph.at( top_index )->name << " -> " << graph.at( link_top_index )->name << std::endl;
			return false;
		}
	}

	sorted_graph.push_back( graph.at( top_index ) );
	return no_cycles;
}

bool topological_sort( std::vector<Top*> graph, std::vector<Top*>& sorted_graph )
{
	// ������ ������ ���������� ������
	bool* visited = new bool[ graph.size( ) ];
	for ( int i = 0; i < graph.size( ); i++ )
		visited[ i ] = false;

	// ��� ������ ������������ ������� ���������� �� ��� � � ������
	for ( int i = 0; i < graph.size( ); i++ )
		if ( !visited[ i ] )
		{
			// ��� ��������� ������ ����� ������� ������� ������� ��������
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
			std::cout << "������ � ������: " << raw_string << ". ������� ����� ��� ����� ���� ���������� '|'." << std::endl;
			return false;
		}

		int link_time = 0;
		std::string raw_link_time = string_parts.at( 1 );
		raw_link_time.erase( std::remove_if( raw_link_time.begin( ), raw_link_time.end( ), isspace ), raw_link_time.end( ) );

		try {
			link_time = std::stoi( raw_link_time );
		}
		catch ( ... ) {
			std::cout << "������ ��� ����������� " << string_parts.at( 1 ) << " � �����!" << std::endl;
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
		std::cout << "�� ������� ������� ���� input.txt" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::cout << "�� ������� ������� ���� output.txt" << std::endl;
		return 1;
	}

	// ������ ������ ��� ������ � ��� ���� ����� �� ��� ���������������
	std::vector<Top*> graph;
	std::vector<Top*> sorted_graph;

	// ������ input.txt � ��������� ������ ���������
	if ( !init_graph( graph, input ) )
		return 1;

	// �������������� ����������
	if ( !topological_sort( graph, sorted_graph ) )
		return 1;

	std::cout << "��������������� ������ ������:" << std::endl;
	output << "��������������� ������ ������:" << std::endl;

	for ( int i = 0; i < sorted_graph.size( ); i++ )
	{
		std::cout << sorted_graph.at( i )->name << std::endl;
		output << sorted_graph.at( i )->name << std::endl;
	}

	// ���� ������������ ����:

	// 1. ������ ������ ���, ��� i-������� - ���� ���������� ������������ � ��� �������
	DuoLink** max_paths = new DuoLink *[ sorted_graph.size( ) ];
	for ( int i = 0; i < sorted_graph.size( ); i++ )
	{
		max_paths[ i ] = new DuoLink;
		max_paths[ i ]->time = 0;
	}

	// 2. ��������� ������
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

	// 3. ���� �������, ���� � ������� �������� ���������
	DuoLink* max_path = new DuoLink;
	max_path->time = -1;

	for ( int i = 0; i < sorted_graph.size( ); i++ )
		if ( max_paths[ i ]->time > max_path->time )
			max_path = max_paths[ i ];

	std::cout << "\n���� ���������� ������������: " << max_path->time << " (";
	output << "\n���� ���������� ������������: " << max_path->time << " (";

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