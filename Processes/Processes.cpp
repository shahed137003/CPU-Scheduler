#include "Processes.h"

// Constructors
Processes::Processes() {
    name = '\0';
    burst_time = 0.0;
    arrival_time = 0.0;
    turnaround_time = -1;
    response_time = -1;
    waiting_time = -1;
    remaining_time = 0.0;
    last_time_in_readyqueue = -1;
    priority= 0;
}
Processes::Processes(char nam, float arrival, float burst) {
    name = nam;
    burst_time = burst;
    arrival_time = arrival;
    turnaround_time = -1;
    response_time = -1;
    waiting_time = -1;
    remaining_time = burst;
    last_time_in_readyqueue = -1;
    priority= 0;
}
Processes::Processes(char nam, float arrival, float burst, int prior) {
    name = nam;
    burst_time = burst;
    arrival_time = arrival;
    turnaround_time = -1;
    response_time = -1;
    waiting_time = -1;
    remaining_time = burst;
    last_time_in_readyqueue = -1;
    priority= prior;
}

// Destructor
Processes::~Processes() {}

// Setters
void Processes::setName(char nam) {
    name = nam;
}
void Processes::setBurst(float burst) {
    burst_time = burst;
}
void Processes::setArrival(float arrival) {
    arrival_time = arrival;
}
void Processes::setTurnaround(float turnaround) {
    turnaround_time = turnaround;
}
void Processes::setResponse(float response) {
    response_time = response;
}
void Processes::setWaiting(float waiting) {
    waiting_time = waiting;
}
void Processes::setRemaining(float remaining) {

    remaining_time = remaining;
}
void Processes::setLasttime(float last) {
    last_time_in_readyqueue = last;

}
void Processes::setPriority(int prior) {
    priority= prior;
}

// Getters
char Processes::getName() const {
    return name;
}
float Processes::getBurst() const {
    return burst_time;
}
float Processes::getArrival() const {
    return arrival_time;
}
float Processes::getTurnaround() const {
    return turnaround_time;
}
float Processes::getResponse() const {
    return response_time;
}
float Processes::getWaiting() const {
    return waiting_time;
}
float Processes::getRemaining() const {
    return remaining_time;
}
float Processes::getLasttime() const {
    return last_time_in_readyqueue;
}
int Processes::getPriority() const {
    return priority;
}
bool compareByArrival(const Processes& a, const Processes& b) {
    return a.getArrival() < b.getArrival();
}
bool compareByPriority(const Processes& a, const Processes& b) {
    return a.getPriority() < b.getPriority();
}
float calcTotal_response_time(queue<Processes>processes) {
    float result = 0.0;
    int  y = processes.size();
    for (int i = 0;i < y;i++) {
        result += processes.front().getResponse();
        processes.pop();
    }
    return result;
}
float calcAvg_response_time(queue<Processes>processes) {
    return calcTotal_response_time(processes) / processes.size();
}
float calcTotal_turn_time(queue<Processes>processes) {
    float result = 0.0;
    int  y = processes.size();
    for (int i = 0; i < y; i++) {
        result += processes.front().getTurnaround();
        processes.pop();
    }
    return result;
}
float calcAvg_turn_time(queue<Processes>processes) {
    return calcTotal_turn_time(processes) / processes.size();
}
float calcTotal_wait_time(queue<Processes>processes) {
    float result = 0.0;
    int  y = processes.size();
    for (int i = 0;i < y;i++) {
        result += processes.front().getWaiting();
        processes.pop();
    }
    return result;
}
float calcAvg_wait_time(queue<Processes>processes) {
    return calcTotal_wait_time(processes) / processes.size();
}
void wait() {
    // Waiting for 1 second
    Sleep(1000);
}
void wait(int n) {
    while (n > 0) {
        wait();
        n--;
    }
}
void wait(int n, char print) {
    int n3times = 3 * n;
    int middle = n3times / 2;
    while (n3times > 0) {
        if (n3times == middle ) {
            std::cout << '_';
            std::cout << print;
            std::cout << '_';
        }
        else {
            std::cout << '_';
            std::cout << '_';
            std::cout << '_';
        }
        wait();
        n3times -=3;
    }
}
void printGantt(int time, bool live, char name) {
    if (live) {
        wait(time, name);
    }
    else { 
        int threeTimes = (3 * time);
        int middle = (3 * time) / 2;
        while (threeTimes > middle + 1) {
            std::cout << '_';
            threeTimes--;
        }
        std ::cout << name;
        threeTimes--;
        while (threeTimes > 0) {
            std::cout << '_';
            threeTimes--;
        }
    }
}
