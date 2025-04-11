#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
QString algorithmMain;
int numProcessesMain;
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

    ui->comboBox->setCurrentIndex(-1);
    ui->comboBox->setPlaceholderText("Select an option: ");
    ui->comboBox->addItem("First Come First Served");
    ui->comboBox->addItem("Non Preemptive Shortest Job First");
    ui->comboBox->addItem("Preemptive Shortest Job First");
    ui->comboBox->addItem("Non Preemptive Priority Scheduling");
    ui->comboBox->addItem("Preemptive Priority Scheduling");
    ui->comboBox->addItem("Round Robin");
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
    algorithmMain = ui->comboBox->currentText();
    numProcessesMain = ui->spinBox->value();


}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Nextbutton_clicked()
{     numProcessesMain = ui->spinBox->value();     // Get the entered number of processes

    algorithmMain = ui->comboBox->currentText(); // Get the selected algorithm
    hide(); // Hide the main window
    processInfo= new class processInfo(this); // Create processInfo instance
    processInfo->receiveProcessData(algorithmMain, numProcessesMain); // Pass the selected data
    processInfo->show(); // Show the processInfo dialog

}
