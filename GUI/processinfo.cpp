#include "processinfo.h"
#include "build/Desktop_Qt_6_8_1_MinGW_64_bit-Debug/ui_processinfo.h"
#include "ui_processinfo.h"
bool needsPriority=false;
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
    needsPriority = algorithm.contains("Priority", Qt::CaseInsensitive);
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
int i = 0;
bool first = false;
float quantum = 0.0;
vector<Processes> processes;
vector<Processes> process;
void processInfo::on_pushButton_clicked()
{
    if(i>processNum)
    {
        sort(processes.begin(),processes.end(),compareByArrival);
        ui->pushButton->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->lineEdit_4->hide();
        ui->label_6->show();
        ui->label_6->setText("Processes added successfully");
        Dynamically *dynamically = new class Dynamically(nullptr); // Create Dynamically instance
        dynamically->callAlgo(processes,process,quantum,comboIndex); // Pass the selected algo
        dynamically->show(); // Show the Dynamically dialog*/
    }
    if(i>1)
        ui->label_4->hide();
    //ui->label_5->setText("Process "+ QString::number(i));
    Processes p;
    char name = 'A'+i; //nshof 7l!
    float arrival=(ui->lineEdit->text()).toFloat();
    p.setName(name);
    p.setArrival(arrival);
    p.setBurst((ui->lineEdit_2->text()).toFloat());
    p.setPriority((ui->lineEdit_3->text()).toInt());
    QString str = QString::number(arrival, 'f', 2);
    Processes p1(i, arrival, (ui->lineEdit_2->text()).toFloat(), true);
    process.push_back(p);
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
    QString output;
    for (const auto& p : processes) {
        output += QString("%1\t%2\t%3\n")
        .arg(p.getName())
            .arg(p.getArrival())
            .arg(p.getBurst());
    }
    // Update table headers dynamically
    QStringList headers = {"Process Name", "Arrival Time", "Burst Time"};
    int columnCount = 3;

    if (needsPriority) {
        headers << "Priority";
        columnCount++;
    }

    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);   // Disable selection

    // Insert new row
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // Column 0: Process Name
    QTableWidgetItem *nameItem = new QTableWidgetItem(QString(p.getName()));
    nameItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 0, nameItem);

    // Column 1: Arrival Time
    QTableWidgetItem *arrivalItem = new QTableWidgetItem(QString::number(p.getArrival()));
    arrivalItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, arrivalItem);

    // Column 2: Burst Time
    QTableWidgetItem *burstItem = new QTableWidgetItem(QString::number(p.getBurst()));
    burstItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, burstItem);

    // Column 3: Priority (only if needed)
    if (needsPriority) {
        QTableWidgetItem *priorityItem = new QTableWidgetItem(QString::number(p.getPriority()));
        priorityItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 3, priorityItem); // Must match the correct index
    }

    // Style the table
    ui->tableWidget->setStyleSheet(R"(
    QTableWidget {
        background-color: #f9f9f9;
        alternate-background-color: #e0e0e0;
        gridline-color: #ccc;
        font: 14px 'Segoe UI';
    }
    QHeaderView::section {
        background-color: #3498db;
        color: white;
        font-weight: bold;
        padding: 5px;
        border: 1px solid #ccc;
    }
    QTableWidget::item {
        color: black;
    }
    QTableWidget::item:selected {
        background-color: #a0d8ef;
        color: black;
    }
)");





}
