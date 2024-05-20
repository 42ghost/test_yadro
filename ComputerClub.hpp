#ifndef COMPUTERCLUB_HPP
#define COMPUTERCLUB_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <regex>
#include "Computer.hpp"

enum ClientStatus {
    ClientArrived = 1,
    ClientSatDown,
    ClientWaiting,
    ClientLeave,
};

enum ClubEvent {
    ClubLeave = 11,
    ClubSatDown,
    ClubError,
};

struct Client {
    std::string name;
    ClientStatus status;
    int compID;
};


class ComputerClub {
    private:
        std::vector<Computer> computers;
        std::map<std::string, Client> clients;
        std::queue<std::string> waitingQueue;
        std::pair<int, int> openTime;
        std::pair<int, int> closeTime;
        int price;
        std::vector<std::string> loglist;
    public:
        ComputerClub();
        ComputerClub(int, int, std::pair<int, int>, std::pair<int, int>);
        ~ComputerClub();

        void run(std::ifstream&);
        void close();
        
        bool clientExist(std::string);
        bool isOpen(std::pair<int, int>);
        bool isClosed(std::pair<int, int>);
        bool placeIsBusy(int);
        bool placeIsWaiting(int);
        bool hasFreeTable();

        int compareTime(std::pair<int, int>, std::pair<int, int>);
        void printTime(std::pair<int, int>, char='\0');
        void printEvent(std::pair<int, int>, int, std::string, int=-1);

        // обработчики событий
        void clientArrived(std::string, std::pair<int, int>);
        void clientLeave(std::string, std::pair<int, int>, bool=0);
        void clientSatDown(std::string, std::pair<int, int>, int);
};

#endif