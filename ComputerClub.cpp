#include "ComputerClub.hpp"

ComputerClub::ComputerClub() {
}

ComputerClub::ComputerClub(int n, int p, std::pair<int, int> t1, std::pair<int, int> t2) {
    computers = std::vector<Computer>(n+1, Computer());
    openTime = t1;
    closeTime = t2;
    price = p;
}

ComputerClub::~ComputerClub() {
}

void ComputerClub::run(std::ifstream &file){
    printTime(openTime);
    std::cout << std::endl;
    std::string s;
    std::regex pattern134(R"((2[0-3]|[01]?[0-9]):([0-5][0-9]) (1|2|3|4) ([a-z0-9_-]+))");
    std::regex pattern2(R"((2[0-3]|[01]?[0-9]):([0-5][0-9]) (1|2|3|4) ([a-z0-9_-]+) ([0-9]+))");
    std::smatch match;
    while (getline(file, s)) {
        if (std::regex_search(s, match, pattern134) && match[0] == s) {
            std::pair<int, int> time = {std::stoi(match[1]), std::stoi(match[2])};
            int eventID = std::stoi(match[3]);
            std::string name = match[4];
            
            printEvent(time, eventID, name);

            if (eventID == ClientArrived) { // Клиент пришёл
                if (isClosed(time)) {
                    printEvent(time, ClubError, "NotOpenYet");
                } else if (clientExist(name)){
                    printEvent(time, ClubError, "YouShallNotPass!");
                } else {
                    clientArrived(name, time);
                }
            } else if (eventID == ClientWaiting) {
                if (hasFreeTable()) {
                    printEvent(time, ClubError, "ICanWaitNoLonger!");
                } else if (!clientExist(name)) { // Не по ТЗ
                    printEvent(time, ClubError, "ClientUnknown");
                } else if (waitingQueue.size() > computers.size()) {
                    clientLeave(name, time);    
                }
            } else if (eventID == ClientLeave) {
                if (!clientExist(name)) {
                    printEvent(time, ClubError, "ClientUnknown");
                } else {
                    clientLeave(name, time);
                }
            }
        } else if (std::regex_search(s, match, pattern2) && match[0] == s) {
            std::pair<int, int> time = {std::stoi(match[1]), std::stoi(match[2])};
            int eventID = std::stoi(match[3]);
            std::string name = match[4];
            int compID = std::stoi(match[5]);

            printEvent(time, eventID, name, compID);

            if (eventID == ClientSatDown) { 
                if (placeIsBusy(compID)) {
                    printEvent(time, ClubError, "PlaceIsBusy");
                } else if (!clientExist(name)) {
                    printEvent(time, ClubError, "ClientUnknown");
                } else {
                    clientSatDown(name, time, compID);
                }
            }
        } else {
            std::cout << s << std::endl;
            exit(1);
        }
    }
}

void ComputerClub::close(){
    std::vector<std::string> tmp;
    std::sort(tmp.begin(), tmp.end());
    for (auto cl = clients.begin(); cl != clients.end(); ++cl){
        tmp.push_back(cl->first);
    }

    for (auto cl : tmp){
        if (clients[cl].status == ClientWaiting || clients[cl].status == ClientSatDown){
            printEvent(closeTime, ClubLeave, cl);
            clientLeave(cl, closeTime);
        }
    }
    printTime(closeTime, '\n');

    for (int i = 1; i < computers.size(); ++i) {
        computers[i].shutdown(closeTime);
        
        std::cout << i << ' ' << computers[i].getProfit(price) << ' ';

        printTime(computers[i].getTime(), '\n');
        
    }
}

bool ComputerClub::isOpen(std::pair<int, int> t){
    if (compareTime(openTime, t) != 1 && compareTime(closeTime, t) == 1) {
        return true;
    }
    return false;    
}

bool ComputerClub::isClosed(std::pair<int, int> t){
    return !isOpen(t);   
}

int ComputerClub::compareTime(std::pair<int, int> t1, std::pair<int, int> t2){
    if (t1.first > t2.first) {
        return 1;
    } else if (t1.first < t2.first) {
        return -1;
    }

    if (t1.second > t2.second) {
        return 1;
    } else if (t1.second < t2.second) {
        return -1;
    }

    return 0;
}

void ComputerClub::printTime(std::pair<int, int> time, char lastch){
    if (time.first <= 9){
        std::cout << 0;
    }
    std::cout << time.first << ':';

    if (time.second <= 9){
        std::cout << 0;
    }
    std::cout << time.second << lastch;
}

void ComputerClub::printEvent(std::pair<int, int> time, int eventID, std::string body, int compID){
    printTime(time, ' ');
    std::cout << eventID << ' ' << body;
    if (compID > 0){
        std::cout << ' ' << compID << std::endl;
    } else {
        std::cout << std::endl;
    }
}

bool ComputerClub::clientExist(std::string name){
    return (clients.find(name) != clients.end());
}

bool ComputerClub::placeIsBusy(int id){
    return (computers[id].getStatus() == Busy);
}

bool ComputerClub::placeIsWaiting(int id){
    return (computers[id].getStatus() == Waiting);
}

bool ComputerClub::hasFreeTable(){
    for (int i = 1; i < computers.size(); ++i){
        if (placeIsWaiting(i)){
            return true;
        }
    }
    return false;
}

void ComputerClub::clientArrived(std::string name, std::pair<int, int> time){
    clients[name] = Client{name, ClientWaiting, -1};
    waitingQueue.push(name);
}

void ComputerClub::clientLeave(std::string name, std::pair<int, int> time){
    if (clients[name].status == ClientWaiting) {
        std::queue<std::string> tmp;
        while (!waitingQueue.empty()) {
            if (waitingQueue.front() != name){
                tmp.push(waitingQueue.front());
            }
            waitingQueue.pop();
        }
        waitingQueue = std::move(tmp);
    } else if (clients[name].status == ClientSatDown) {
        computers[clients[name].compID].shutdown(time);
        if (!waitingQueue.empty()){
            printEvent(time, ClubSatDown, waitingQueue.front(), clients[name].compID);
            clientSatDown(waitingQueue.front(), time, clients[name].compID);
        } else {
            computers[clients[name].compID].setStatus(Waiting);
        }
    }
    clients.erase(name);
}

void ComputerClub::clientSatDown(std::string name, std::pair<int, int> time, int compID){
    clients[name].compID = compID;
    clients[name].status = ClientSatDown;
    computers[compID].setStatus(Busy);
    
    computers[compID].setStartTime(time);
    
    if (waitingQueue.front() != name){
        std::queue<std::string> tmp;
        while (!waitingQueue.empty()) {
                if (waitingQueue.front() != name){
                    tmp.push(waitingQueue.front());
                }
                waitingQueue.pop();
        }
        waitingQueue = std::move(tmp);
    } else {
        waitingQueue.pop();
    }
}