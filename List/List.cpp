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

struct Node {
    std::string node_name;
    Node* g;
};

int main()
{
    SetConsoleCP( 1251 );
    SetConsoleOutputCP( 1251 );

    std::ifstream input_sizes( "input_sizes.txt" );
    std::ifstream input_stack( "input_stack.txt" );
    std::ofstream output( "output.txt" );

    if ( !input_sizes )
    {
        std::cout << "Не удалось открыть файл input_sizes.txt" << std::endl;
        return 1;
    }

    if ( !input_stack )
    {
        std::cout << "Не удалось открыть файл input_stack.txt" << std::endl;
        return 1;
    }

    if ( !output )
    {
        std::cout << "Не удалось открыть файл output.txt" << std::endl;
        return 1;
    }

    std::string word;
    while ( input_stack >> word )
    {
        std::cout << word << std::endl;
    }
}