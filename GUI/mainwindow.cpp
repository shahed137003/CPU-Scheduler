#include "mainwindow.h"
#include "build/Desktop_Qt_6_8_1_MinGW_64_bit-Debug/ui_mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
QString algorithmMain;
int numProcessesMain;
int cbindex;
bool live;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: 	#000000; color: white; font-family: Arial; border: black");
    QString text ="Welcome to the CPU Scheduler 🖥️";
    ui->label->setText(text);
    ui->label->setAlignment(Qt::AlignCenter);
    int currentIndex = 0;

    // Timer that updates the label text character by character
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (currentIndex < text.length()) {
            ui->label->setText(text.left(currentIndex + 1));
            currentIndex++;
        } else {
            timer->stop(); // Stop the timer once the full text is displayed
        }
    });

    timer->start(50);

    timer->start(50); // Update every 100 milliseconds
    ui->label->setStyleSheet(R"(
    font-size: 33px;  /* Larger font size */
    font-weight: bold;  /* Bold title */
    color: white;  /* Title text color set to white */
    background-color: #3498db;
    )");
    ui->label->setText(text);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_4->setText("Live Scheduling");
    ui->label_4->setStyleSheet("QLabel {"
                               "    font: bold 23px 'Arial';"        // Font style and size
                               "    color: #FFFFFF;"                 // Text color

                               "}");
    ui->checkBox->setText("");
    ui->checkBox->setStyleSheet("QCheckBox {"
                                "    background-color: #ecf0f1;"  // Light gray background
                                "    color: #2c3e50;"             // Dark text color
                                "    font: bold 14px Arial;"      // Bold font, 14px size
                                "    border: 2px solid #2980b9;"  // Blue border
                                "    border-radius: 5px;"         // Rounded corners
                                "    padding: 5px;"               // Padding inside the combobox
                                "} "


                                );
    ui->comboBox->setCurrentIndex(-1);
    ui->comboBox->setPlaceholderText("Select an option: ");
    ui->comboBox->addItem("First Come First Served");  // 0 FCFS
    ui->comboBox->addItem("Non Preemptive Shortest Job First"); // 1 SJF
    ui->comboBox->addItem("Preemptive Shortest Job First"); // 2 non SJF
    ui->comboBox->addItem("Non Preemptive Priority Scheduling");    // 3 nonPriority
    ui->comboBox->addItem("Preemptive Priority Scheduling");        // 4 priority
    ui->comboBox->addItem("Round Robin");       //5 rr
    ui->comboBox->setStyleSheet("QComboBox {"
                                "    background-color: #ecf0f1;"  // Light gray background
                                "    color: #2c3e50;"             // Dark text color
                                "    font: bold 14px Arial;"      // Bold font, 14px size
                                "    border: 2px solid #2980b9;"  // Blue border
                                "    border-radius: 5px;"         // Rounded corners
                                "    padding: 5px;"               // Padding inside the combobox
                                "} "
                                "QComboBox QAbstractItemView {"
                                "    background-color: white;"   // White background for the dropdown list
                                "    color: black;"              // Black text for visibility
                                "    selection-background-color: #2980b9;" // Highlight color when an option is selected
                                "    selection-color: white;"    // Text color when an option is selected
                                "    border: 1px solid #2980b9;" // Border around the dropdown
                                "} "

                                "QComboBox:hover {"
                                "    border: 2px solid #3498db;"  // Border color changes on hover
                                "} "


                                );
    ui->label_2->setText("Choose a scheduling algorithm ");
    ui->label_2->setStyleSheet("QLabel {"
                               "    font: bold 23px 'Arial';"        // Font style and size
                               "    color: #FFFFFF;"                 // Text color
                               "}");
    ui->label_3->setText("Enter number of processes ");
    ui->label_3->setStyleSheet("QLabel {"
                               "    font: bold 23px 'Arial';"        // Font style and size
                               "    color: #FFFFFF;"                 // Text color

                               "}");
    ui->spinBox->setStyleSheet("QSpinBox {"
                               "    background-color: #ecf0f1;"  // Light gray background
                               "    color: #2c3e50;"             // Dark text color
                               "    font: bold 14px Arial;"      // Bold font, 14px size
                               "    border: 2px solid #2980b9;"  // Blue border
                               "    border-radius: 5px;"         // Rounded corners
                               "} "

                               /* Style for up and down buttons */
                               "QSpinBox::up-button, QSpinBox::down-button {"

                               "    width: 25px; height: 15px;"  // Button size
                               "} "

                               );

    ui->Nextbutton->setStyleSheet("QPushButton {"
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
                                  "    background-color: #1c6f94;"    // Even darker blue when pressed
                                  "    transform: scale(0.95);"        // Slightly shrinks the button on press
                                  "} "
                                  "QPushButton:disabled {"
                                  "    background-color: #bdc3c7;"    // Grey background for disabled state
                                  "    color: #7f8c8d;"                // Darker text color for disabled state
                                  "}");




}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Nextbutton_clicked()
{
    if (ui->comboBox->currentIndex() == -1 && ui->spinBox->value() == 0) {
        showWarning("Please select a scheduling algorithm and Number of processes");
        return;
    }
    // Validate algorithm selection
    if (ui->comboBox->currentIndex() == -1) {
        showWarning("Please select a scheduling algorithm");
        return;
    }

    // Validate number of processes
    if (ui->spinBox->value() <= 0) {
        showWarning("Number of processes must be at least 1");
        return;
    }
    cbindex= ui->comboBox->currentIndex();
    numProcessesMain = ui->spinBox->value();     // Get the entered number of processes
    algorithmMain = ui->comboBox->currentText(); // Get the selected algorithm
    live = ui->checkBox->isChecked();
    hide(); // Hide the main window
    processInfo= new class processInfo(this); // Create processInfo instance
    processInfo->receiveProcessData(algorithmMain, numProcessesMain,cbindex,live); // Pass the selected data
    processInfo->show(); // Show the processInfo dialog

}
void MainWindow::showWarning(const QString& message)
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
