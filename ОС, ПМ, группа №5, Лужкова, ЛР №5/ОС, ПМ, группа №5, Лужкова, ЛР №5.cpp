// Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <string>

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main(int argc, char* argv[]) {
    int count_of_emp = std::stoi(argv[1]);

    HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
    if (hStartEvent == nullptr)
    {
        std::cout << "Open event failed. \nInput any char to exit.\n";
        std::cin.get();
        return GetLastError();
    }

    SetEvent(hStartEvent);
    HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\pipe_name", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Creation of the named pipe failed.\n The last error code: " << GetLastError() << "\n";
        std::cout << "Press anything to finish server: ";
        _getch();
        return (0);
    }

    while (true) {
        int chosen_option = 0;
        std::cout << "\nChoose option:\n 1 Modify data\n 2 Read data\n 3 Exit\nChoose: ";
        std::cin >> chosen_option;
        if (chosen_option == 3) {
            break;
        }
        else if (chosen_option == 1) {
            DWORD dwBytesWritten;
            DWORD dwBytesReaden;
            int ID;
            std::cout << "\n\nInput an ID of employee:";
            std::cin >> ID;
            if (ID > count_of_emp)
            {
                std::cout << "\nID not find\n";
                continue;
            }

            int message_to_send = ID * 10 + chosen_option;
            bool checker = WriteFile(
                hPipe, // дескриптор канала
                &message_to_send, // адрес буфера для вывода данных
                sizeof(message_to_send), // число записываемых байтов
                &dwBytesWritten, // число записанных байтов
                NULL);// передача данных синхронная

            if (checker) {
                std::cout << "\nMessage was sent.\n";
            }
            else {
                std::cout << "\nMessage wasn't sent.\n";
            }

            employee* emp = new employee();
            ReadFile(
                hPipe, // дескриптор канала
                emp, // адрес буфера для ввода данных
                sizeof(employee), // число читаемых байтов
                &dwBytesReaden, // число прочитанных байтов
                NULL); // передача данных синхронная

            std::cout << "\nID of employee: " << emp->num << "\nName of employee: " << emp->name << "\nHours of employee: " << emp->hours << "\n\n";
            std::cout << "Input new Name: ";
            std::cin >> emp->name;
            std::cout << "Input new Hours: ";
            std::cin >> emp->hours;

            checker = WriteFile(
                hPipe, // дескриптор канала
                emp, // адрес буфера для вывода данных
                sizeof(employee), // число записываемых байтов
                &dwBytesWritten, // число записанных байтов
                NULL);// передача данных синхронная

            if (checker) {
                std::cout << "\nMessage was sent.\n";
            }
            else {
                std::cout << "\nMessage wasn't sent.\n";
            }


            std::cout << "Press any key to confirm the option...\n";
            _getch();

            message_to_send = 1;
            WriteFile(hPipe, &message_to_send, sizeof(message_to_send), &dwBytesWritten, NULL);
            delete[] emp;
        }
        else if (chosen_option == 2) {
            DWORD dwBytesWritten;
            DWORD dwBytesReaden;
            int ID;
            std::cout << "\nInput an ID of employee:";
            std::cin >> ID;
            if (ID > count_of_emp)
            {
                std::cout << "\nID not find\n";
                continue;
            }

            int message_to_send = ID * 10 + chosen_option;
            bool checker = WriteFile(
                hPipe, // дескриптор канала
                &message_to_send, // адрес буфера для вывода данных
                sizeof(message_to_send), // число записываемых байтов
                &dwBytesWritten, // число записанных байтов
                NULL);// передача данных синхронная

            if (checker) {
                std::cout << "Message was sent.\n";
            }
            else {
                std::cout << "Message wasn't sent.\n";
            }
            employee* emp = new employee();
            ReadFile(
                hPipe, // дескриптор канала
                emp, // адрес буфера для ввода данных
                sizeof(employee), // число читаемых байтов
                &dwBytesReaden, // число прочитанных байтов
                NULL); // передача данных синхронная

            std::cout << "\nID of employee: " << emp->num << "\nName of employee: " << emp->name << "\nHours of employee: " << emp->hours << "\n";

            std::cout << "Press any key to confirm the option...\n";
            _getch();
            message_to_send = 1;
            WriteFile(hPipe, &message_to_send, sizeof(message_to_send), &dwBytesWritten, NULL);
        }

    }

    return 0;
}
