#include "processinfo.h"
#include "ui_processinfo.h"
bool needsPriority;
bool needsQuantum;
struct ProcessInputs {
    QLabel* label;             // Process label
    QLabel* arrivalLabel;      // Arrival time label
    QSpinBox* arrivalInput;
    QLabel* burstLabel;        // Burst time label
    QSpinBox* burstInput;
    QLabel* priorityLabel;     // Priority label (optional)
    QSpinBox* priorityInput;   // Priority input (optional)
    QSpinBox* quantumInput;    // Quantum input (optional)
    QLabel* quantumLabel;      // Quantum label (optional)
};
QVector<ProcessInputs> processInputs;

processInfo::processInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::processInfo)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: 	#000000; color: white; font-family: Arial; border: black");

    QString text = "Process Information 📝";
    ui->title->setStyleSheet(R"(
    font-size: 33px;  /* Larger font size */
    font-weight: bold;  /* Bold title */
    color: white;  /* Title text color set to white */
    background-color: #3498db;
)");

    ui->title->setText(text);
    ui->title->setAlignment(Qt::AlignCenter);

    // Setting Algorithm label style
    ui->Algorithm_label->setStyleSheet(R"(
    font-size: 23px;  /* Size of the font */
    font-weight: bold;  /* Bold font */
    font-family: 'Segoe UI', Tahoma, Arial, sans-serif;  /* Clean and modern font */
    color: #FFFFFF;
    padding-bottom: 0px;  /* Space below the label */
)");


    ui->label->setStyleSheet(R"(
    background-color: transparent;   /* No background */
    color: white;                    /* White text color */
    font: bold 24px Arial;           /* Bold font, 14px size */
    border: none;                    /* No border */
)");
    ui->label_2->setStyleSheet(R"(
    background-color: transparent;   /* No background */
    color: white;                    /* White text color */
    font: bold 24px Arial;           /* Bold font, 14px size */
    border: none;                    /* No border */
)");
    ui->label_3->setStyleSheet(R"(
    background-color: transparent;   /* No background */
    color: white;                    /* White text color */
    font: bold 24px Arial;           /* Bold font, 14px size */
    border: none;                    /* No border */
)");
    ui->label_4->setStyleSheet(R"(
    background-color: transparent;   /* No background */
    color: white;                    /* White text color */
    font: bold 24px Arial;           /* Bold font, 14px size */
    border: none;                    /* No border */
)");
    ui->label_5->setStyleSheet(R"(
    background-color: transparent;   /* No background */
    color: white;                    /* White text color */
    font: bold 24px Arial;           /* Bold font, 14px size */
    border: none;                    /* No border */
)");
    ui->label_6->setStyleSheet(R"(
    font-size: 33px;  /* Larger font size */
    font-weight: bold;  /* Bold title */
    color: white;  /* Title text color set to white */
    )");
    ui->label_6->setAlignment(Qt::AlignCenter);
    ui->label_6->hide();


    QString lineEditStyle = R"(
    QLineEdit {
        background-color: white;
        color: black;  /* Set text color to black */
    }
)";

    ui->lineEdit->setStyleSheet(lineEditStyle);
    ui->lineEdit_2->setStyleSheet(lineEditStyle);
    ui->lineEdit_3->setStyleSheet(lineEditStyle);
    ui->lineEdit_4->setStyleSheet(lineEditStyle);


}

processInfo::~processInfo()
{
    delete ui;
}
int comboIndex;
void processInfo::receiveProcessData(QString algorithm, int numProcesses, int index) {
    // Set Algorithm and Process Number Labels
    processInfo::AlgorithmNum = algorithm;
    processInfo::processNum = numProcesses;

    QString algorithmText =  AlgorithmNum;
    ui->Algorithm_label->setText(algorithmText);

    QString processText = "Selected Number of Processes: " + QString::number(processNum);
    bool needsPriority = algorithm.contains("Priority", Qt::CaseInsensitive);
    bool needsQuantum = algorithm.contains("Round Robin", Qt::CaseInsensitive);
    // Create input fields dynamically based on the number of processes
    if(!needsPriority)
    {
        ui->lineEdit_3->hide();
        ui->label_3->hide();
    }
    if(!needsQuantum)
    {
        ui->lineEdit_4->hide();
        ui->label_4->hide();
    }
    ui->label_5->setText("Process 1");
    ui->label_5->setAlignment(Qt::AlignLeft);
    ui->label->setText("Arrival Time: ");
    ui->label_2->setText("Burst Time: ");
    ui->label_3->setText("Priority: ");
    ui->label_4->setText("Quantum Time: ");
    ui->pushButton->setText("Add process");
    ui->pushButton->setStyleSheet("QPushButton {"
                                  "    background-color: #3498db;"    // Blue background
                                  "    color: white;"                 // White text color
                                  "    font: bold 23px Arial;"        // Bold font with Arial style
                                  "    padding: 10px 20px;"           // Padding for a spacious button
                                  "    border-radius: 10px;"          // Rounded corners
                                  "    border: none;"                 // No border
                                  "    transition: background-color 0.3s ease, transform 0.2s ease;" // Smooth transitions
                                  "} "
                                  "QPushButton:hover {"
                                  "    background-color: #2980b9;"    // Darker blue when hovered
                                  "    transform: scale(1.05);"        // Slightly enlarges the button on hover
                                  "} "
                                  "QPushButton:pressed {"
                                  "    background-color: #2980b9;"    // Even darker blue when pressed
                                  "    transform: scale(0.95);"        // Slightly shrinks the button on press
                                  "} "
                                  "QPushButton:disabled {"
                                  "    background-color: #2980b9;"    // Grey background for disabled state
                                  "    color: #7f8c8d;"                // Darker text color for disabled state
                                  "}");
    comboIndex = index;
}
int i = 2;//Why 2 ??
bool first = false;
float quantum = 0.0;
vector<Processes> processes;
vector<Process> process;
void processInfo::on_pushButton_clicked()
{
    if(i>processNum)
    {
        switch(comboIndex){
        case 0:
            FCFS(processes);
            break;
        case 1:
            SJF_Non(processes);
            break;
        case 2:
            scheduleSRJF(process);
            break;
        case 3:
            PriorityNon(processes);
            break;
        case 4:
            PriorityPre(processes);
            break;
        case 5:
            queue<Processes>processesQ;
            for (const auto& proc : processes) {
                processesQ.push(proc);
            }
            bool live = true;
            roundRobin(processesQ, quantum, live);
            break;
        }
        ui->pushButton->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->lineEdit_4->hide();
        ui->label_6->show();
        ui->label_6->setText("Processes added successfully");
        Dynamically *dynamically = new class Dynamically(nullptr); // Create Dynamically instance
        //processInfo->receiveProcessData(algorithmMain, numProcessesMain); // Pass the selected data
        dynamically->show(); // Show the processInfo dialog*/
    }
    ui->label_5->setText("Process "+ QString::number(i));
    Processes p;
    char name = 'A' +i;
    float arrival=(ui->lineEdit->text()).toFloat();
    p.setName(name);
    p.setArrival(arrival);
    p.setBurst((ui->lineEdit_2->text()).toFloat());
    p.setPriority((ui->lineEdit_3->text()).toInt());
    QString str = QString::number(arrival, 'f', 2);
    Process p1(i, arrival, (ui->lineEdit_2->text()).toFloat(), true);
    process.push_back(p1);
    processes.push_back(p);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    first = true;
    if(first){
        quantum = ((ui->lineEdit_4->text()).toFloat());
        ui->lineEdit_4->hide();
        ui->label_4->hide();
    }

    i++;

}
