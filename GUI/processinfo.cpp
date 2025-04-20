
#include "processinfo.h"
#include "ui_processinfo.h"
#include <QThread>
#include <QDebug>

bool needsPriority = false;
bool needsQuantum;
bool isLive = false;

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
//std::mutex allMutex ;

processInfo::processInfo(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::processInfo)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #000000; color: white; font-family: Arial; border: black");

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
    ui->pushButton_2->setText("Add Extra process");
    ui->pushButton_2->setStyleSheet("QPushButton {"
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
    ui->pushButton_2->hide();
    ui->timeLabel->setStyleSheet(R"(
    font-size: 18px;
    font-weight: bold;
    color: white;
    background-color: transparent;)");
    ui->timeLabel->setAlignment(Qt::AlignRight);
    ui->timeLabel->setText("Current Time: 0.0");
    ui->timeLabel->setFixedWidth(200);  // Adjust width as needed
    // Set up validators for line edits
    QDoubleValidator* arrivalValidator = new QDoubleValidator(0.0, 9999.0, 1, this);
    arrivalValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit->setValidator(arrivalValidator);

    QDoubleValidator* burstValidator = new QDoubleValidator(0.1, 9999.0, 1, this);
    burstValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_2->setValidator(burstValidator);

    QIntValidator* priorityValidator = new QIntValidator(0, 9999, this);
    ui->lineEdit_3->setValidator(priorityValidator);

    QDoubleValidator* quantumValidator = new QDoubleValidator(0.1, 9999.0, 1, this);
    quantumValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_4->setValidator(quantumValidator);
}

processInfo::~processInfo()
{
    delete ui;
}

int comboIndex;
void processInfo::receiveProcessData(QString algorithm, int numProcesses, int index, bool live) {
    // Set Algorithm and Process Number Labels
    processInfo::AlgorithmNum = algorithm;
    processInfo::processNum = numProcesses;
    isLive = live;

    QString algorithmText = AlgorithmNum;
    ui->Algorithm_label->setText(algorithmText);

    QString processText = "Selected Number of Processes: " + QString::number(processNum);
    needsPriority = algorithm.contains("Priority", Qt::CaseInsensitive);
    bool needsQuantum = algorithm.contains("Round Robin", Qt::CaseInsensitive);
    // Create input fields dynamically based on the number of processes
    if (!needsPriority)
    {
        ui->lineEdit_3->hide();
        ui->label_3->hide();
    }
    if (!needsQuantum)
    {
        ui->lineEdit_4->hide();
        ui->label_4->hide();
    }
    if (!isLive) {
        ui->tableWidget->hide();
    }
    ui->pushButton_3->setText("Add process");
    ui->pushButton_3->setStyleSheet("QPushButton {"
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
    ui->pushButton_3->hide();
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
std::vector<Processes> processes;
std::queue<SRJF::Process> process;
std::queue<Processes> processesQ;
float overall_time = 0;
void processInfo::on_pushButton_clicked()
{
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() ||
        (needsPriority && ui->lineEdit_3->text().isEmpty()) ||
        (needsQuantum && first == false && ui->lineEdit_4->text().isEmpty())) {
        QString warningMsg = "Please fill in all required fields:\n";
        if (ui->lineEdit->text().isEmpty()) warningMsg += "- Arrival Time\n";
        if (ui->lineEdit_2->text().isEmpty()) warningMsg += "- Burst Time\n";
        if (needsPriority && ui->lineEdit_3->text().isEmpty()) warningMsg += "- Priority\n";
        if (needsQuantum && first == false && ui->lineEdit_4->text().isEmpty()) warningMsg += "- Quantum Time\n";
        showWarning(warningMsg);
        return;
    }
    bool ok;
    float arrival = ui->lineEdit->text().toFloat(&ok);
    if (!ok || arrival < 0) {
        showWarning("Arrival time must be a number ≥ 0");
        return;
    }

    float burst = ui->lineEdit_2->text().toFloat(&ok);
    if (!ok || burst <= 0) {
        showWarning("Burst time must be a number > 0");
        return;
    }

    if (needsPriority) {
        int priority = ui->lineEdit_3->text().toInt(&ok);
        if (!ok || priority < 0) {
            showWarning("Priority must be an integer ≥ 0");
            return;
        }
    }

    if (!first && needsQuantum) {
        float quantum = ui->lineEdit_4->text().toFloat(&ok);
        if (!ok || quantum <= 0) {
            showWarning("Quantum time must be a number > 0");
            return;
        }
    }
    Processes p;
    char name = 'A' + i;
    p.setName(name);
    p.setArrival(arrival);
    p.setBurst((ui->lineEdit_2->text()).toFloat());
    p.setRemaining((ui->lineEdit_2->text()).toFloat());
    p.setPriority((ui->lineEdit_3->text()).toInt());
    qDebug() << "Process data:" << p.getName() << p.getArrival() << p.getBurst() << p.getPriority();

    SRJF::Process p1(i, arrival, (ui->lineEdit_2->text()).toFloat(), true, name);
    {
        std::lock_guard<std::mutex> lock(allMutex);
        process.push(p1);
        processes.push_back(p);
        processesQ.push(p);
    }
    if (!first) {
        quantum = ((ui->lineEdit_4->text()).toFloat());
        qDebug() << "Quantum = " << quantum;
        ui->lineEdit_4->hide();
        ui->label_4->hide();
        first = true;
    }

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();

    // Update table
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
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(QString(p.getName()));
    nameItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 0, nameItem);

    QTableWidgetItem *arrivalItem = new QTableWidgetItem(QString::number(p.getArrival()));
    arrivalItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, arrivalItem);

    QTableWidgetItem *burstItem = new QTableWidgetItem(QString::number(p.getBurst()));
    burstItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, burstItem);

    if (needsPriority) {
        QTableWidgetItem *priorityItem = new QTableWidgetItem(QString::number(p.getPriority()));
        priorityItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 3, priorityItem);
    }

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

    if (i >= processNum - 1)
    {
        std::sort(processes.begin(), processes.end(), compareByArrival);
        sort_queue(processesQ);
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
        ui->tableWidget->show();
        ui->pushButton_2->show();

        // Create worker and thread
        QThread* thread = new QThread;
        Worker* worker = new Worker(&processes, &processesQ, &process, &overall_time, quantum, comboIndex, isLive, &allMutex);
        worker->moveToThread(thread);

        // Connect signals and slots
        connect(thread, &QThread::started, worker, &Worker::run);
        connect(worker, &Worker::finished, thread, &QThread::quit);
        connect(worker, &Worker::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        // Start the thread
        thread->start();
        worker->startTimer(); // Start the timer when worker starts

        // Connect the time update signal
        connect(worker, &Worker::timeUpdated, this, &processInfo::updateTimeDisplay);

        // Don't forget to stop the timer when done
        connect(worker, &Worker::finished, worker, &Worker::stopTimer);
        return; // Stop execution to prevent adding extra row
    }
    i++;
}

void processInfo::on_pushButton_2_clicked()
{
    if (needsPriority)
    {
        ui->lineEdit_3->show();
        ui->label_3->show();
    }
    if (needsQuantum)
    {
        ui->lineEdit_4->show();
        ui->label_4->show();
    }
    if (!isLive) {
        ui->tableWidget->hide();
    }
    ui->pushButton_3->show();
    ui->label->show();
    ui->label_2->show();
    ui->lineEdit->show();
    ui->lineEdit_2->show();
    ui->label_6->hide();
    processNum++;
    i++;
    ui->tableWidget->show();
    ui->pushButton_2->hide();
}

void processInfo::on_pushButton_3_clicked()
{
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() ||
        (needsPriority && ui->lineEdit_3->text().isEmpty())) {
        QString warningMsg = "Please fill in all required fields:\n";
        if (ui->lineEdit->text().isEmpty()) warningMsg += "- Arrival Time\n";
        if (ui->lineEdit_2->text().isEmpty()) warningMsg += "- Burst Time\n";
        if (needsPriority && ui->lineEdit_3->text().isEmpty()) warningMsg += "- Priority\n";
        showWarning(warningMsg);
        return;
    }

    bool ok;
    float arrival = ui->lineEdit->text().toFloat(&ok);
    if (!ok || arrival < 0) {
        showWarning("Arrival time must be a number ≥ 0");
        return;
    }

    float burst = ui->lineEdit_2->text().toFloat(&ok);
    if (!ok || burst <= 0) {
        showWarning("Burst time must be a number > 0");
        return;
    }

    if (needsPriority) {
        int priority = ui->lineEdit_3->text().toInt(&ok);
        if (!ok || priority < 0) {
            showWarning("Priority must be an integer ≥ 0");
            return;
        }
    }
    bool msg = false;
    Processes p;
    char name = 'A' + i;
    QString savedMessage;
    p.setName(name);
    if(arrival < overall_time){
        p.setArrival(overall_time);
        msg = true;
    }
    else p.setArrival(arrival);
    p.setBurst((ui->lineEdit_2->text()).toFloat());
    p.setRemaining((ui->lineEdit_2->text()).toFloat());
    p.setPriority((ui->lineEdit_3->text()).toInt());
    qDebug() << "Process data:" << p.getName() << p.getArrival() << p.getBurst() << p.getPriority();

    SRJF::Process p1(i, arrival, (ui->lineEdit_2->text()).toFloat(), true, name);
    {
        std::lock_guard<std::mutex> lock(allMutex);
        process.push(p1);
        std::sort(processes.begin(), processes.end(), compareByArrival);
        processes.push_back(p);
        std::cout << "size before Added process: " << processesQ.size() << "\n";
        processesQ.push(p);
        std::cout << "size after Added process: " << processesQ.size() << "\n";
        sort_queue(processesQ);
        std::cout << "Added process: " << p.getName() << "\n";
    }
    ui->pushButton->hide();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    ui->pushButton_3->hide();
    ui->label_6->show();
    ui->label_6->setText("Processes added successfully");
    ui->tableWidget->show();
    ui->pushButton_2->show();

    if(msg){
        savedMessage = QString("You chose an arrival time less than the running time now.\n"
                        "So, we will set arrival time of process %1 to %2").arg(name).arg(overall_time);
        // Create and style QMessageBox
        QMessageBox information;
        information.setWindowTitle("Info");
        information.setText(savedMessage);
        information.setStandardButtons(QMessageBox::Ok);
        information.setModal(false) ;

        // Apply stylesheet to QMessageBox
        information.setStyleSheet(R"(
            QMessageBox {
                background-color: #000000;
                color: white;
                font-family: Arial;
                font-size: 16px;
            }
            QMessageBox QLabel {
                color: white;
                font: bold 16px Arial;
            }
            QMessageBox QPushButton {
                background-color: #3498db;
                color: white;
                font: bold 14px Arial;
                padding: 8px 16px;
                border-radius: 8px;
                border: none;
            }
            QMessageBox QPushButton:hover {
                background-color: #2980b9;
            }
            QMessageBox QPushButton:pressed {
                background-color: #1f6391;
                transform: scale(0.95);
            }
        )");

        information.exec();
    }

    // Update table
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
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(QString(p.getName()));
    nameItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 0, nameItem);

    QTableWidgetItem *arrivalItem = new QTableWidgetItem(QString::number(p.getArrival()));
    arrivalItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, arrivalItem);

    QTableWidgetItem *burstItem = new QTableWidgetItem(QString::number(p.getBurst()));
    burstItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, burstItem);

    if (needsPriority) {
        QTableWidgetItem *priorityItem = new QTableWidgetItem(QString::number(p.getPriority()));
        priorityItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 3, priorityItem);
    }

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
void processInfo::updateTimeDisplay(float newTime) {
    ui->timeLabel->setText(QString("Current Time: %1").arg(newTime,0,'f',1));
    ui->timeLabel->adjustSize();
}
void processInfo::showWarning(const QString& message)
{
    QMessageBox warning;
    warning.setWindowTitle("Invalid Input");
    warning.setText(message);
    warning.setIcon(QMessageBox::Warning);
    warning.setStandardButtons(QMessageBox::Ok);

    warning.setStyleSheet(R"(
        QMessageBox {
            background-color: #000000;
            color: white;
            font-family: Arial;
            font-size: 16px;
        }
        QMessageBox QLabel {
            color: white;
            font: bold 16px Arial;
        }
        QMessageBox QPushButton {
            background-color: #3498db;
            color: white;
            font: bold 14px Arial;
            padding: 8px 16px;
            border-radius: 8px;
            border: none;
        }
        QMessageBox QPushButton:hover {
            background-color: #2980b9;
        }
        QMessageBox QPushButton:pressed {
            background-color: #1f6391;
            transform: scale(0.95);
        }
    )");

    warning.exec();
}
