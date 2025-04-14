#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <string>

using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time = -1;
    int finish_time = -1;

    Process(int pid, int arrival, int burst)
        : pid(pid), arrival_time(arrival), burst_time(burst), remaining_time(burst) {
    }
};

// Priority: Shortest remaining time first
auto cmp = [](const Process* a, const Process* b) {
    if (a->remaining_time == b->remaining_time)
        return a->arrival_time > b->arrival_time;
    return a->remaining_time > b->remaining_time;
    };

// Shared resources for dynamic input
mutex mtx;
vector<Process> dynamic_processes;
atomic<bool> stop_flag(false);

void inputListener() {
    cout << "Input listener ready. Enter new processes as: pid arrival_time burst_time" << endl;
    cout << "Enter 'stop' to end scheduling early" << endl;

    while (true) {
        string input;
        getline(cin, input);

        if (input == "stop") {
            stop_flag = true;
            break;
        }

        int pid, arrival, burst;
        if (sscanf_s(input.c_str(), "%d %d %d", &pid, &arrival, &burst) == 3) {
            lock_guard<mutex> lock(mtx);
            dynamic_processes.emplace_back(pid, arrival, burst);
            cout << "New process " << pid << " will be added at time " << arrival << endl;
        }
        else {
            cout << "Invalid format. Use: pid arrival_time burst_time" << endl;
        }
    }
}

void calculateAverages(const vector<Process>& processes) {
    double total_waiting = 0;
    double total_turnaround = 0;

    for (const auto& p : processes) {
        int turnaround = p.finish_time - p.arrival_time;
        int waiting = turnaround - p.burst_time;

        total_turnaround += turnaround;
        total_waiting += waiting;
    }

    int count = processes.size();
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << (total_waiting / count) << endl;
    cout << "Average Turnaround Time: " << (total_turnaround / count) << endl;
}

void scheduleSRJF(vector<Process>& processes) {
    int time = 0;
    priority_queue<Process*, vector<Process*>, decltype(cmp)> ready_queue(cmp);
    Process* current = nullptr;
    vector<Process*> all_processes; // To keep track for average calc

    while (true) {
        // Add initial processes at correct time
        for (auto& p : processes) {
            if (p.arrival_time == time) {
                ready_queue.push(&p);
                all_processes.push_back(&p);
            }
        }

        // Add new processes from input
        {
            lock_guard<mutex> lock(mtx);
            for (auto it = dynamic_processes.begin(); it != dynamic_processes.end(); ) {
                if (it->arrival_time <= time) {
                    processes.push_back(*it); // copy to main list
                    ready_queue.push(&processes.back());
                    all_processes.push_back(&processes.back());
                    it = dynamic_processes.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        // Check if current process finished
        if (current && current->remaining_time == 0) {
            current->finish_time = time;
            current = nullptr;
        }

        // Preempt if necessary
        if (!current || (!ready_queue.empty() && ready_queue.top()->remaining_time < current->remaining_time)) {
            if (current && current->remaining_time > 0) {
                ready_queue.push(current);
            }
            if (!ready_queue.empty()) {
                current = ready_queue.top();
                ready_queue.pop();
                if (current->start_time == -1)
                    current->start_time = time;
            }
            else {
                current = nullptr;
            }
        }

        // Run current process
        if (current) {
            current->remaining_time--;
        }

        // Check termination
        if (stop_flag && ready_queue.empty() && (!current || current->remaining_time == 0) && dynamic_processes.empty()) {
           // cout << "\nAll processes completed at time " << time << endl;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
        time++;
    }

    // Final process list for averages
    vector<Process> finished;
    for (auto* p : all_processes) {
        if (p->finish_time != -1)
            finished.push_back(*p);
    }

    calculateAverages(finished);
}

int main() {
    int n;
    cout << "Enter number of initial processes: ";
    cin >> n;
    cin.ignore();

    vector<Process> processes;
    for (int i = 0; i < n; ++i) {
        int pid, at, bt;
        cout << "Enter Process " << i + 1 << " (pid arrival_time burst_time): ";
        cin >> pid >> at >> bt;
        processes.emplace_back(pid, at, bt);
    }
    cin.ignore();

    thread listener(inputListener);
    scheduleSRJF(processes);
    listener.join();

    return 0;
}
