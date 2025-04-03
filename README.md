# 🖥️ CPU Scheduling Simulator (Live Process Scheduler)

## 📌 Project Overview  
This project is a **GUI-based CPU Scheduling Simulator** that supports multiple scheduling algorithms. It allows users to select a scheduling algorithm, add processes dynamically, and visualize real-time scheduling with a **live Gantt Chart**. The simulator updates the remaining burst time table and calculates key performance metrics like **Average Waiting Time** and **Average Turnaround Time**.

## 🚀 Features  
✔ **Supports multiple scheduling algorithms:**  
   - **FCFS** (First Come, First Served)  
   - **SJF** (Shortest Job First) – Preemptive & Non-Preemptive  
   - **Priority Scheduling** – Preemptive & Non-Preemptive  
   - **Round Robin**  

✔ **Dynamic process addition** while scheduling is running  
✔ **Real-time scheduling visualization (Live Gantt Chart)**  
✔ **Live updating burst time table**  
✔ **Option to run only the existing processes (without live scheduling)**  
✔ **GUI-based Desktop Application**  
✔ **Generates a ready-to-run executable file**  

## 📥 Input Requirements  
- **Scheduler Type** (FCFS, SJF, Priority, Round Robin)  
- **Number of Processes** currently ready to run  
- **Process Information** (based on the scheduler type)  
  - **FCFS:** Arrival Time, Burst Time  
  - **SJF:** Arrival Time, Burst Time  
  - **Priority Scheduling:** Arrival Time, Burst Time, Priority  
  - **Round Robin:** Arrival Time, Burst Time, Time Quantum  

⚠️ **Note:** The program only asks for the necessary information based on the selected scheduler.  

## ⚙️ How It Works  
- Each **1 unit of CPU time = 1 second**  
- Processes execute based on the selected scheduling algorithm  
- The **remaining burst time table updates live**  
- The **Gantt Chart is drawn live** to show process execution order  
- Calculates and displays:  
  - **Average Waiting Time**  
  - **Average Turnaround Time**  

## 🎨 Graphical User Interface (GUI)  
The application provides an interactive **desktop GUI** that includes:  
✅ **Process Table** – Displays all added processes and their details  
✅ **Gantt Chart** – Live visualization of process execution  
✅ **Remaining Burst Time Table** – Updates as time progresses  
✅ **Control Buttons** – Add processes, Start Live Scheduling, Run Static Scheduling  

## 📦 Installation & Setup  
To run the project, follow these steps:  
1️⃣ **Clone the Repository**  
```sh
git clone https://github.com/your-username/cpu-scheduler-simulator.git
cd cpu-scheduler-simulator
