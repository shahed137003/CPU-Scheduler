#ifndef GANTT_H
#define GANTT_H



class GanttChart{
protected:
    void updateGanttChart(std::queue<char> processes, std::queue<std::vector<float>> timeSlots, bool live);
};
#endif // GANTT_H
