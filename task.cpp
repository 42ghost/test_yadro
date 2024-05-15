#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

#include "ComputerClub.hpp"


int main(int argc, const char * argv[]){
    if (argc != 2){
        std::cout << "incorrect number of arguments" << std::endl;
        exit(1);
    }

    std::string s;
    std::ifstream file(argv[1]);

    int n, price;
    std::string time;

    // Чтение количества компьютеров
    if(getline(file, s)){
        n = std::stoi(s);
        if (n <= 0) {
            std::cout << n << std::endl;
            return 1;
        }
    }
    
    // Чтение времени работы
    std::smatch match;
    std::regex pattern(R"((2[0-3]|[01]?[0-9]):([0-5][0-9]) (2[0-3]|[01]?[0-9]):([0-5][0-9]))");
    std::pair<int, int> time1, time2; 
    if (getline(file, time)){
        if (std::regex_search(time, match, pattern) && match[0] == time) {
            time1 = {std::stoi(match[1]), std::stoi(match[2])};
            time2 = {std::stoi(match[3]), std::stoi(match[4])};
        } else {
            std::cout << time << std::endl;
            return 1;
        }
    } else {
        std::cout << "incorrect test file" << std::endl;
        exit(1);
    }
    
    // Чтение цены
    if (getline(file, s)){
        price = std::stoi(s);
        if (price <= 0) {
            std::cout << price << std::endl;
            return 1;
        }
    } else {
        std::cout << "incorrect test file" << std::endl;
        exit(1);
    }

    // Создание класса обработчика КК
    ComputerClub club = ComputerClub(n, price, time1, time2);
    club.run(file);
    file.close();

    club.close();
    
    return 0;
}