/*
6. Имеется  текстовый  файл  с  некоторым  описанием.   Все
предложения   заканчиваются   точкой.   Требуется   проверить,
является  ли  первая  буква  каждого  предложения   прописной.
Исправить обнаруженные ошибки (6).

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022

Источники:
Сигнатура wmain: https://learn.microsoft.com/en-us/cpp/c-language/using-wmain?view=msvc-170#the-wmain-function-signature
_setmode для изменения кодировки stdout/stdin: https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/reference/setmode?view=msvc-170
Изменение локализации файловых потоков: https://www.codeproject.com/Articles/38242/Reading-UTF-8-with-C-streams
*/

#include <iostream>
#include <windows.h>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>

int wmain( int argc, wchar_t** argv )
{
	// Меняем режим stdout/stdin на UTF-8 без BOM
	_setmode( _fileno( stdout ), _O_U8TEXT );
	_setmode( _fileno( stdin ), _O_U8TEXT );

	// Создаём utf8 локализацию, которую используем как глобальную. Изменение глобальной локализации даёт нам возможность не использовать `imbue` для сменя локализации каждого файлого потока.
	std::locale utf8_locale( std::locale( std::locale::empty( ), new std::codecvt_utf8<wchar_t> ) );
	std::locale::global( utf8_locale );

	// Если мы передали 2 или более 3 аргументов, то это неправильное использование программы
	if ( argc != 1 && argc != 3 )
	{
		std::wcout << L"Ошибка! Введите имена входного и выходного файлов, либо запустите программу без аргументов, чтобы использовать имена по умолчанию (input.txt | output.txt)" << std::endl;
		return 1;
	}

	std::wstring input_file_name( L"input.txt" );
	std::wstring output_file_name( L"output.txt" );

	if ( argc > 1 )
	{
		input_file_name = argv[ 1 ];
		output_file_name = argv[ 2 ];
	}

	std::wifstream input( input_file_name );
	std::wofstream output( output_file_name );

	if ( !input )
	{
		std::wcout << L"Не удалось открыть входной файл" << std::endl;
		return 1;
	}

	if ( !output )
	{
		std::wcout << L"Не удалось открыть выходной файл" << std::endl;
		return 1;
	}

	bool should_next_uppercase = true;
	wchar_t ch;

	while ( input.get( ch ) )
	{
		if ( ch == L'.' )
			should_next_uppercase = true;

		else if ( should_next_uppercase )
		{
			bool is_russian_lowercase = ch >= L'а' && ch <= L'я' || ch == L'ё';
			bool is_russian_uppercase = ch >= L'А' && ch <= L'Я' || ch == L'Ё';
			bool is_english_lowercase = ch >= L'a' && ch <= L'z';
			bool is_english_uppercase = ch >= L'A' && ch <= L'Z';

			if ( is_russian_lowercase || is_russian_uppercase || is_english_lowercase || is_english_uppercase )
			{
				if ( is_russian_lowercase )
					if ( ch == L'ё' )
						ch = L'Ё';
					else
						ch = L'А' + (ch - L'а');

				else if ( is_english_lowercase )
					ch = L'A' + (ch - L'a');

				should_next_uppercase = false;
			}
		}

		output << ch;
	}
}