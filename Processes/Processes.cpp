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
void printGantt(queue<char>operate, queue<vector<float>>time_slots, bool live) {
    vector<float> time_interval;
    float last = 0;
    if (time_slots.front()[0] != 0) {
        int n = time_slots.size();
        time_slots.push({ 0,time_slots.front()[0] });
        for (int i = 0;i < n;i++) {
            time_interval = time_slots.front();
            time_slots.pop();
            time_slots.push(time_interval);
        }
        n = operate.size();
        operate.push('#');
        char temp;
        for (int i = 0;i < n;i++) {
            temp = operate.front();
            operate.pop();
            operate.push(temp);
        }
    }
    for (int i = 0;i < time_slots.size();i++) {
        time_interval = time_slots.front();
        time_slots.pop();
        if (i == time_slots.size() - 1) {
            last = time_slots.front()[1];
        }
        time_slots.push(time_interval);
    }
    cout << "\n";
    last = 3 * last + time_slots.size() + 1;
    while (last--)cout << '_';
    cout << "\n";
    while (!operate.empty()) {
        cout << "|";
        time_interval = time_slots.front();
        float time = time_interval[1] - time_interval[0];
        int threeTimes = (3 * time);
        int middle = (3 * time) / 2;
        while (threeTimes > middle + 1) {
            if (operate.front() == '#') cout << '#';
            else std::cout << '_';
            threeTimes--;
            if (live && threeTimes % 3 == 0) wait();
        }
        std::cout << operate.front();
        threeTimes--;
        if (live && threeTimes % 3 == 0) wait();
        while (threeTimes > 0) {
            if (operate.front() == '#') cout << '#';
            else std::cout << '_';
            threeTimes--;
            if (live && threeTimes % 3 == 0) wait();
        }
        operate.pop();
        time_slots.pop();
        time_slots.push(time_interval);
        if (!operate.empty() && time_interval[1] != time_slots.front()[0]) {
            cout << "|";
            time = time_slots.front()[0] - time_interval[1];
            for (int i = 0; i < 3 * time;i++) {
                cout << '#';
                if (live && i % 3 == 0) wait();
            }
        }
    }
    cout << "|\n";
    printNumbers(time_slots);
}

void printNumbers(queue<vector<float>>time_slots) {
    while (!time_slots.empty()) {
        int great = 0; // 0 in case of two digit number, 1 in case 3 digit number....soon
        float number = time_slots.front()[0];
        cout << number;
        while (number / 10 >= 1.0) { 
            great++;
            number /= 10;
        }
        int diff = time_slots.front()[1] - time_slots.front()[0];
        for (int i = 0; i < 3 * diff - great;i++) {
            cout << ' ';
        }

        number = time_slots.front()[1];
        if (time_slots.size() == 1) cout << number;
        time_slots.pop();
        if (!time_slots.empty() && number != time_slots.front()[0]) {
            cout << number;
            diff = time_slots.front()[0] - number;
            for (int i = 0; i < 3 * diff - great;i++) {
                cout << ' ';
            }
        }
    }
}
