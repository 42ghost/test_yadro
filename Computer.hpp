#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <vector>
#include <iostream>

enum Status {
    Waiting,
    Busy
};

class Computer
{
private:
    Status status;
    int payHours;
    std::pair<int, int> startTime;
    std::pair<int, int> busytime;
public:
    Computer();
    ~Computer();

    Status getStatus();
    std::pair<int, int> getTime();
    int getProfit(int);
    void setStatus(Status);
    void setStartTime(std::pair<int, int> );

    std::pair<int, int> sumTime(std::pair<int, int>, std::pair<int, int>);
    std::pair<int, int> diffTime(std::pair<int, int> , std::pair<int, int>);

    void shutdown(std::pair<int, int>);
};


#endif