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
        std::cout << "�� ������� ������� ���� input_sizes.txt" << std::endl;
        return 1;
    }

    if ( !input_stack )
    {
        std::cout << "�� ������� ������� ���� input_stack.txt" << std::endl;
        return 1;
    }

    if ( !output )
    {
        std::cout << "�� ������� ������� ���� output.txt" << std::endl;
        return 1;
    }

    std::string word;
    while ( input_stack >> word )
    {
        std::cout << word << std::endl;
    }
}