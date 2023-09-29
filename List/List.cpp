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

std::map<std::string, int> ram_sizes;

struct Procedure {
	std::string name;
	Procedure* next;

	Procedure( std::string name, Procedure* next ) : name( name ), next( next ) {}
};

struct Stack {
	Procedure* top = nullptr;

	bool is_empty( ) { return top == nullptr; };

	void push( std::string procedure )
	{
		top = new Procedure( procedure, top );
	}

	std::string pop( )
	{
		if ( is_empty( ) )
			return "";

		Procedure* next_top = top->next;
		std::string to_return = top->name;

		delete top;
		top = next_top;
		return to_return;
	}

	void clear( )
	{
		while ( !is_empty( ) )
		{
			pop( );
		}
	}

	bool find( std::string to_search )
	{
		Stack copied_stack = copy_reverse( );
		bool found = false;

		while ( !found && !copied_stack.is_empty( ) )
		{
			found = to_search == copied_stack.pop( );
		}

		copied_stack.clear( );
		return found;
	}

	Stack copy_reverse( )
	{
		Stack new_stack;

		if ( is_empty() )
			return new_stack;

		Procedure* copied_current_top = new Procedure( top->name, top->next );

		while ( copied_current_top != nullptr )
		{
			new_stack.push( copied_current_top->name );
			copied_current_top = copied_current_top->next;
		}

		delete copied_current_top;
		return new_stack;
	}
};

int get_procedure_size( std::string name )
{
	auto it = ram_sizes.find( name );
	if ( it == ram_sizes.end( ) )
		return -1;

	return it->second;
}

void process_trace( Stack stack )
{
	Stack current_trace;
	Stack max_size_trace;

	int max_ram_size = 0;
	int curr_ram_size = 0;

	Procedure* curr = stack.top;

	while ( curr != nullptr )
	{
		if ( current_trace.find( curr->name ) )
		{
			if ( curr->name == current_trace.top->name )
			{
				std::cout << "Wrong traceback: `" << curr->name << "` recursive called" << std::endl;
			}

			curr_ram_size -= get_procedure_size( current_trace.pop( ) );

			if ( curr->name != current_trace.top->name )
			{
				std::cout << "Wrong traceback: `" << current_trace.top->name << "` not returned" << std::endl;
			}
		}
		else
		{
			current_trace.push( curr->name );
			curr_ram_size += get_procedure_size( curr->name );
		}

		curr = curr->next;
		if ( curr_ram_size > max_ram_size )
		{
			max_ram_size = curr_ram_size;
			max_size_trace.clear( );
			max_size_trace = current_trace.copy_reverse();
		}
	}

	std::cout << "Needed ram size: " << max_ram_size << std::endl;
	std::cout << "Maximum traceback:" << std::endl;

	Procedure* max_trace_top = max_size_trace.top;
	while ( max_trace_top != nullptr )
	{
		std::cout << max_trace_top->name << std::endl;
		max_trace_top = max_trace_top->next;
	}

	current_trace.clear( );
	delete max_trace_top, curr;
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

	Stack stack;
	std::string procedure_name;

	while ( input_trace >> procedure_name )
	{
		stack.push( procedure_name );
	}

	process_trace( stack );
}