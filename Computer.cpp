#include "Computer.hpp"

Computer::Computer() {
    status = Waiting;
    payHours = 0;
    busytime = {0, 0};
}

Computer::~Computer() {
}

Status Computer::getStatus(){
    return status;
}

std::pair<int, int> Computer::getTime(){
    return busytime;
}

int Computer::getProfit(int price){
    return payHours * price;
}

void Computer::setStatus(Status status){
    this->status = status;
}

void Computer::setStartTime(std::pair<int, int> time){
    if (startTime.first != 0 || startTime.second != 0) {
        //busytime = sumTime(busytime, diffTime(time, startTime));
    }
    startTime = time;
}


std::pair<int, int> Computer::sumTime(std::pair<int, int> t1, std::pair<int, int> t2){
    std::pair<int, int> tmp;

    tmp.first = t1.first + t2.first;
    tmp.second = t1.second + t2.second;

    tmp.first += tmp.second / 60;
    tmp.second = tmp.second % 60;

    return tmp;
}

std::pair<int, int> Computer::diffTime(std::pair<int, int> t1, std::pair<int, int> t2){
    std::pair<int, int> tmp;

    tmp.first = t1.first - t2.first;
    if (t1.second < t2.second){
        tmp.first -= 1;
        tmp.second = t1.second - t2.second + 60;
    } else {
        tmp.second = t1.second - t2.second;
    }

    return tmp;
}

void Computer::shutdown(std::pair<int, int> time){
    if (status == Busy) {
        std::pair<int, int> tmp = diffTime(time, startTime);
        payHours += tmp.first + (tmp.second > 0 ? 1 : 0);
        if (tmp.first == 0 && tmp.second == 0){
            payHours += 1;
        }
        busytime = sumTime(busytime, tmp);
    }
}