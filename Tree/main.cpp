/*
15. �����������  ���������,   ��   ����������   �����������
�������  �  �������������  ����  ��������,  ����������� � ����
������ ����������� ��������. ��������� �������� � ������, ����
� ��� ��������� ��������� �� ���������� ��������� ���� �������
���������� ��  ���������  ��  ���������.  ���������  ���������
������,  ���  ������ ���������� ��������� ������� � ����������
�� ���������.  �������� ����� ������,  �������  ���������  ���
�������� ������ ���������.  ��� ������ �� ��������� ����������
��   ������   �������������.   ��������,   �������������    ��
�����������  ����������  ������  ���������.  ���������� ������
������,  ����������� ��� ������ ���������,  � ������� �������,
��������� ������������ ������ (8).

��������: ����� ���� ��-21
IDE: Visual Studio 2022
C++20
*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <map>
#include <string>
#include <vector>

std::map<std::string, int> ram_sizes;

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

void process_trace( std::ifstream& stream )
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

		// ������������ �� ���������� �������
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

			current_ram_size -= get_procedure_size( tree_node->name );
			current_ram_size -= get_procedure_size( tree_node->father->name );
			prev_tree = tree_node->father->father;
		}

		// ��� �� ��������� �������
		else
		{
			// ��������� ������� ���� � ������ ����� ���������� ����, ���� ��� �� ������ �������
			if ( tree_node->father != nullptr )
				tree_node->father->children.push_back( tree_node );

			current_ram_size += get_procedure_size( tree_node->name );
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
		std::cout << "�� ���� ����� ������ Trace" << std::endl;
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

	std::cout << "���������� ������: " << max_ram_size << std::endl;
	std::cout << "�race, ������� ����� ������ ����� ������:" << std::endl;

	for (int i = max_ram_trace.size() - 1; i >= 0; i--)
		std::cout << max_ram_trace.at( i ) << std::endl;
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
		std::cout << "�� ������� ������� ���� input_sizes.txt" << std::endl;
		return 1;
	}

	if ( !input_trace )
	{
		std::cout << "�� ������� ������� ���� input_trace.txt" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::cout << "�� ������� ������� ���� output.txt" << std::endl;
		return 1;
	}

	if ( !init_ram_sizes( input_sizes ) )
	{
		std::cout << "������ ��� ��������� ����� input_sizes.txt" << std::endl;
		return 1;
	}

	process_trace( input_trace );
}