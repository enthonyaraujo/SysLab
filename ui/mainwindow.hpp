#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "simulador.hpp"

class QTimer;
class QLabel;
class QPushButton;
class QTableWidget;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    Simulador simulador;

    QLabel *titleLabel;
    QLabel *clockLabel;
    // QLabel *quantumLabel;
    QLabel *pageFaultLabel;

    QPushButton *createProcessButton;
    QPushButton *tickButton;
    QPushButton *startAutoButton;
    QPushButton *pauseAutoButton;
    QPushButton *sendIoButton;
    QPushButton *resetButton;

    QTableWidget *readyTable;
    QTableWidget *runningTable;
    QTableWidget *ioTable;
    QTableWidget *memoryTable;

    QTimer *autoTimer;

    QTextEdit *logText;

    void setupUi();
    void setupTables();

    void updateInterface();
    void updateReadyTable();
    void updateRunningTable();
    void updateIoTable();
    void updateMemoryTable();
    void updateLog();

    void onCreateProcess();
    void onTick();
    void onStartAuto();
    void onPauseAuto();
    void onSendToIo();
    void onReset();
};

#endif