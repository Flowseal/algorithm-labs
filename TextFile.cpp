/*
6. ������� ��������� ���� � ��������� ���������. ���
����������� ������������� ������. ��������� ���������,
�������� �� ������ ����� ������� ����������� ���������.
��������� ������������ ������ (6).

<<<<<<< HEAD
Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022

Источники:
Сигнатура wmain: https://learn.microsoft.com/en-us/cpp/c-language/using-wmain?view=msvc-170#the-wmain-function-signature
_setmode для изменения кодировки stdout/stdin: https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/reference/setmode?view=msvc-170
Изменение локализации файловых потоков: https://www.codeproject.com/Articles/38242/Reading-UTF-8-with-C-streams
=======
��������: ����� ���� ��-21
IDE: Visual Studio 2022
>>>>>>> ae39abd (1251 codepage)
*/

#include <iostream>
#include <fstream>
#include <Windows.h>

int main( int argc, char** argv )
{
	// ������ codepage ������/����� ��������� �� Cyrillic Windows (1251)
	SetConsoleOutputCP( 1251 );
	SetConsoleCP( 1251 );

	// ���� �� ������� 2 ��� ����� 3 ����������, �� ��� ������������ ������������� ���������
	if ( argc != 1 && argc != 3 )
	{
		std::cout << "�������������:" << std::endl;
		std::cout << argv[ 0 ] << "(��� ���. ���������� ����� ������������ ����������� ����� ������: input.txt/output.txt)" << std::endl;
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
		std::cout << "�� ������� ������� ������� ����" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::cout << "�� ������� ������� �������� ����" << std::endl;
		return 1;
	}

	bool should_next_uppercase = true;
	char ch;

	while ( input.get( ch ) )
	{
		if ( ch == '.' )
			should_next_uppercase = true;

		else if ( should_next_uppercase )
		{
			bool is_russian_lowercase = ch >= '�' && ch <= '�' || ch == '�';
			bool is_russian_uppercase = ch >= '�' && ch <= '�' || ch == '�';
			bool is_english_lowercase = ch >= 'a' && ch <= 'z';
			bool is_english_uppercase = ch >= 'A' && ch <= 'Z';

			if ( is_russian_lowercase || is_russian_uppercase || is_english_lowercase || is_english_uppercase )
			{
				if ( is_russian_lowercase )
					if ( ch == '�' )
						ch = '�';
					else
						ch = '�' + (ch - '�');

				else if ( is_english_lowercase )
					ch = 'A' + (ch - 'a');

				should_next_uppercase = false;
			}
		}

		output << ch;
	}

	return 0;
}