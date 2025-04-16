#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iomanip>
#include <atomic>
#include <string>
#include <cstdio>

using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    bool is_initial;

    Process(int id, int arrival, int burst, bool initial)
        : pid(id), arrival_time(arrival), burst_time(burst),
        remaining_time(burst), start_time(-1), finish_time(-1),
        is_initial(initial) {
    }
};

// Global variables
mutex mtx;
queue<Process> new_processes;
atomic<bool> stop_flag(false);
atomic<bool> new_processes_added(false);

//QVector<QPair<QString::QString, int>>& PRocess;
// Comparison function for priority queue (SRJF)
auto cmp = [](const Process* left, const Process* right) {
    return left->remaining_time > right->remaining_time;
    };

void calculateAverages(const vector<Process>& processes) {
    double total_waiting = 0;
    double total_turnaround = 0;
    int count = 0;

    for (const auto& p : processes) {
        if (p.finish_time != -1) {
            int turnaround = p.finish_time - p.arrival_time;
            int waiting = turnaround - p.burst_time;
            total_waiting += waiting;
            total_turnaround += turnaround;
            count++;
        }
    }

    if (count > 0) {
        cout << fixed << setprecision(2);
        cout << "\nAverage Waiting Time: " << (total_waiting / count) << endl;
        cout << "Average Turnaround Time: " << (total_turnaround / count) << endl;
    }
    else {
        cout << "No processes completed." << endl;
    }
}

void scheduleSRJF(vector<Process>& processes) {
    priority_queue<Process*, vector<Process*>, decltype(cmp)> ready_queue(cmp);
    int current_time = 0;
    Process* current_process = nullptr;
    bool initial_only = true;

    while (true) {
        // Check for new processes
        {
            lock_guard<mutex> lock(mtx);
            while (!new_processes.empty() && new_processes.front().arrival_time <= current_time) {
                Process& p = new_processes.front();
                processes.push_back(p);
                ready_queue.push(&processes.back());
                new_processes.pop();
                if (!p.is_initial) {
                    new_processes_added = true;
                    initial_only = false;
                }
            }
        }

        // Check for arriving processes
        for (auto& p : processes) {
            if (p.arrival_time == current_time && p.remaining_time == p.burst_time) {
                ready_queue.push(&p);
            }
        }

        // Process completion
        if (current_process && current_process->remaining_time == 0) {
            current_process->finish_time = current_time;
            current_process = nullptr;
        }

        // Get next process if none running
        if (!current_process && !ready_queue.empty()) {
            current_process = ready_queue.top();
            ready_queue.pop();
            if (current_process->start_time == -1) {
                current_process->start_time = current_time;
            }
        }

        // Execute current process
        if (current_process) {
            current_process->remaining_time--;

            // Check for preemption
            if (!ready_queue.empty() && ready_queue.top()->remaining_time < current_process->remaining_time) {
                ready_queue.push(current_process);
                current_process = ready_queue.top();
                ready_queue.pop();
            }
        }

        // Check termination conditions
        bool should_stop = false;
        {
            lock_guard<mutex> lock(mtx);
            should_stop = stop_flag ||
                (initial_only && ready_queue.empty() && (!current_process || current_process->remaining_time == 0));
        }

        if (should_stop) {
            if (!stop_flag && initial_only) {
                cout << "\nAll initial processes completed at time " << current_time << endl;
            }
            break;
        }

        current_time++;
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    calculateAverages(processes);
}

void inputListener() {
    cout << "Input listener ready. Enter new processes as: pid arrival_time burst_time" << endl;
    cout << "Enter 'stop' to end scheduling early" << endl;

    while (true) {
        string input;
        getline(cin, input);

        if (input == "stop") {
            lock_guard<mutex> lock(mtx);
            stop_flag = true;
            break;
        }

        int pid, arrival, burst;
        if (sscanf_s(input.c_str(), "%d %d %d", &pid, &arrival, &burst) == 3) {
            lock_guard<mutex> lock(mtx);
            new_processes.push(Process(pid, arrival, burst, false));
            cout << "New process " << pid << " will be added at time " << arrival << endl;
        }
        else {
            cout << "Invalid format. Use: pid arrival_time burst_time" << endl;
        }
    }
}
/*
int main() {
    vector<Process> processes;
    int initial_count;

    cout << "Enter number of initial processes: ";
    cin >> initial_count;
    cin.ignore();

    for (int i = 0; i < initial_count; i++) {
        int pid, arrival, burst;
        cout << "Enter Process " << i + 1 << " (pid arrival_time burst_time): ";
        cin >> pid >> arrival >> burst;
        cin.ignore();
        processes.emplace_back(pid, arrival, burst, true);
    }

    thread listener(inputListener);
    thread scheduler(scheduleSRJF, ref(processes));

    listener.join();
    scheduler.join();

    return 0;
}*/
