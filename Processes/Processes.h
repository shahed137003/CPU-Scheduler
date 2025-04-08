#pragma once

#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <windows.h> 

using namespace std;

class Processes {
public:
    Processes();  // Constructor
    Processes(char nam, float arrival, float burst);
    Processes(char nam, float arrival, float burst, int prior);
    ~Processes(); // Destructor

    void setName(char nam);
    void setBurst(float burst);
    void setArrival(float arrival);
    void setTurnaround(float turnaround);
    void setResponse(float response);
    void setWaiting(float waiting);
    void setRemaining(float remaining);
    void setLasttime (float last);
    void setPriority(int prior);

    char getName() const;
    float getBurst() const;
    float getArrival() const;
    float getTurnaround() const;
    float getResponse() const;
    float getWaiting() const;
    float getRemaining() const;
    float getLasttime() const;
    int getPriority() const;

private:
    char name;
    float burst_time;
    float arrival_time;
    float turnaround_time;
    float response_time;
    float waiting_time;
    float remaining_time;
    float last_time_in_readyqueue;
    int priority;
};

void wait();
void wait(int n);
void wait(int n, char print);
void printGantt(queue<char>operate, queue<vector<float>>Time_slots, bool live);
void printNumbers(queue<vector<float>>Time_slots);
bool compareByArrival(const Processes& a, const Processes& b);
bool compareByPriority(const Processes& a, const Processes& b);
float calcTotal_response_time(queue<Processes>processes);
float calcAvg_response_time(queue<Processes>processes);
float calcTotal_turn_time(queue<Processes>processes);
float calcAvg_turn_time(queue<Processes>processes);
float calcTotal_wait_time(queue<Processes>processes);
float calcAvg_wait_time(queue<Processes>processes);