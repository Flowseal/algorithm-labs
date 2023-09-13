/*
6. Имеется текстовый файл с некоторым описанием. Все
предложения заканчиваются точкой. Требуется проверить,
является ли первая буква каждого предложения прописной.
Исправить обнаруженные ошибки (6).

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022
C++20
*/

#include <iostream>
#include <fstream>
#include <Windows.h>

int main( int argc, char** argv )
{
	// Меняем codepage вывода/ввода терминала на Cyrillic Windows (1251)
	SetConsoleOutputCP( 1251 );
	SetConsoleCP( 1251 );

	// Если мы передаём 2 или более 3 аргументов, то это неправильное использование программы
	if ( argc != 1 && argc != 3 )
	{
		std::cout << "Использование:" << std::endl;
		std::cout << argv[ 0 ] << "(без доп. аргументов будут использованы стандартные имена файлов: input.txt/output.txt)" << std::endl;
		std::cout << argv[ 0 ] << " input_file.txt output_file.txt" << std::endl;
		return 1;
	}

	std::string input_file_name( "input.txt" );
	std::string output_file_name( "output.txt" );

	if ( argc > 1 )
	{
		input_file_name = argv[ 1 ];
		output_file_name = argv[ 2 ];
	}

	std::ifstream input( input_file_name );
	std::ofstream output( output_file_name );

	if ( !input )
	{
		std::cout << "Не удалось открыть входной файл" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::cout << "Не удалось открыть выходной файл" << std::endl;
		return 1;
	}

	bool should_next_uppercase = true;
	int errors_found = 0;
	char ch;

	while ( input.get( ch ) )
	{
		if ( ch == '.' )
			should_next_uppercase = true;

		else if ( should_next_uppercase )
		{
			bool is_russian_lowercase = ch >= 'а' && ch <= 'я' || ch == 'ё';
			bool is_russian_uppercase = ch >= 'А' && ch <= 'Я' || ch == 'Ё';
			bool is_english_lowercase = ch >= 'a' && ch <= 'z';
			bool is_english_uppercase = ch >= 'A' && ch <= 'Z';

			if ( is_russian_lowercase || is_russian_uppercase || is_english_lowercase || is_english_uppercase )
			{
				if ( is_russian_lowercase || is_english_lowercase )
					errors_found += 1;

				if ( is_russian_lowercase )
					if ( ch == 'ё' )
						ch = 'Ё';
					else
						ch = 'А' + (ch - 'а');

				else if ( is_english_lowercase )
					ch = 'A' + (ch - 'a');

				should_next_uppercase = false;
			}
		}

		output << ch;
	}

	std::cout << "Ошибок во входном файле: " << errors_found << std::endl;
	return 0;
}