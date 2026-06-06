#include "mainwindow.hpp"

#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QString>
#include <QTimer>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    updateInterface();
}

void MainWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    titleLabel = new QLabel("SysLab - Simulador de Sistemas Operacionais", this);
    clockLabel = new QLabel(this);
    //quantumLabel = new QLabel(this);
    pageFaultLabel = new QLabel(this);

    createProcessButton = new QPushButton("Criar processo", this);
    tickButton = new QPushButton("Executar clock", this);
    startAutoButton = new QPushButton("Executar automático", this);
    pauseAutoButton = new QPushButton("Pausar", this);
    sendIoButton = new QPushButton("Enviar para I/O", this);
    resetButton = new QPushButton("Resetar", this);

    autoTimer = new QTimer(this);
    autoTimer->setInterval(1000);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(clockLabel);
    //topLayout->addWidget(quantumLabel);
    topLayout->addWidget(pageFaultLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(createProcessButton);
    buttonLayout->addWidget(tickButton);
    buttonLayout->addWidget(startAutoButton);
    buttonLayout->addWidget(pauseAutoButton);
    buttonLayout->addWidget(sendIoButton);
    buttonLayout->addWidget(resetButton);

    readyTable = new QTableWidget(this);
    runningTable = new QTableWidget(this);
    ioTable = new QTableWidget(this);
    memoryTable = new QTableWidget(this);
    logText = new QTextEdit(this);

    setupTables();

    logText->setReadOnly(true);

    QGridLayout *tablesLayout = new QGridLayout();

    tablesLayout->addWidget(new QLabel("Fila de pronto", this), 0, 0);
    tablesLayout->addWidget(new QLabel("Processo em execução", this), 0, 1);

    tablesLayout->addWidget(readyTable, 1, 0);
    tablesLayout->addWidget(runningTable, 1, 1);

    tablesLayout->addWidget(new QLabel("Fila de I/O", this), 2, 0);
    tablesLayout->addWidget(new QLabel("Memória", this), 2, 1);

    tablesLayout->addWidget(ioTable, 3, 0);
    tablesLayout->addWidget(memoryTable, 3, 1);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(tablesLayout);
    mainLayout->addWidget(new QLabel("Log de eventos", this));
    mainLayout->addWidget(logText);

    setCentralWidget(central);
    setWindowTitle("SysLab");
    resize(1100, 750);

    connect(createProcessButton, &QPushButton::clicked, this, &MainWindow::onCreateProcess);
    connect(tickButton, &QPushButton::clicked, this, &MainWindow::onTick);
    connect(startAutoButton, &QPushButton::clicked, this, &MainWindow::onStartAuto);
    connect(pauseAutoButton, &QPushButton::clicked, this, &MainWindow::onPauseAuto);
    connect(sendIoButton, &QPushButton::clicked, this, &MainWindow::onSendToIo);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);

    connect(autoTimer, &QTimer::timeout, this, &MainWindow::onTick);
}

void MainWindow::setupTables()
{
    readyTable->setColumnCount(5);
    readyTable->setHorizontalHeaderLabels({"PID", "Nome", "Prioridade", "Tempo restante", "Estado"});

    runningTable->setColumnCount(4);
    runningTable->setHorizontalHeaderLabels({"PID", "Nome", "Tempo restante", "Estado"});

    ioTable->setColumnCount(4);
    ioTable->setHorizontalHeaderLabels({"PID", "Nome", "Tempo I/O", "Estado"});

    memoryTable->setColumnCount(4);
    memoryTable->setHorizontalHeaderLabels({"Quadro", "PID", "Página", "Estado"});

    readyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    runningTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ioTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    readyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    runningTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ioTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updateInterface()
{
    clockLabel->setText("Clock: " + QString::number(simulador.getClock()));
    //quantumLabel->setText("Quantum: " + QString::number(simulador.getQuantum()));
    pageFaultLabel->setText("Page faults: " + QString::number(simulador.getPageFaults()));

    updateReadyTable();
    updateRunningTable();
    updateIoTable();
    updateMemoryTable();
    updateLog();
}

void MainWindow::updateReadyTable()
{
    auto filaPronto = simulador.getFilaPronto();

    readyTable->setRowCount(0);

    for (const auto &processo : filaPronto) {
        int row = readyTable->rowCount();
        readyTable->insertRow(row);

        readyTable->setItem(row, 0, new QTableWidgetItem(QString::number(processo->pid)));
        readyTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(processo->nome)));
        readyTable->setItem(row, 2, new QTableWidgetItem(QString::number(processo->prioridade)));
        readyTable->setItem(row, 3, new QTableWidgetItem(QString::number(processo->tempoRestante)));
        readyTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(processo->estadoComoTexto())));
    }
}

void MainWindow::updateRunningTable()
{
    runningTable->setRowCount(0);

    auto processo = simulador.getProcessoExecutando();

    if (processo == nullptr) {
        return;
    }

    runningTable->insertRow(0);

    runningTable->setItem(0, 0, new QTableWidgetItem(QString::number(processo->pid)));
    runningTable->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(processo->nome)));
    runningTable->setItem(0, 2, new QTableWidgetItem(QString::number(processo->tempoRestante)));
    runningTable->setItem(0, 3, new QTableWidgetItem(QString::fromStdString(processo->estadoComoTexto())));
}

void MainWindow::updateIoTable()
{
    auto filaIO = simulador.getFilaIO();

    ioTable->setRowCount(0);

    for (const auto &processo : filaIO) {
        int row = ioTable->rowCount();
        ioTable->insertRow(row);

        ioTable->setItem(row, 0, new QTableWidgetItem(QString::number(processo->pid)));
        ioTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(processo->nome)));
        ioTable->setItem(row, 2, new QTableWidgetItem(QString::number(processo->tempoIO)));
        ioTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(processo->estadoComoTexto())));
    }
}

void MainWindow::updateMemoryTable()
{
    auto quadros = simulador.getQuadrosMemoria();

    memoryTable->setRowCount(0);

    for (int i = 0; i < static_cast<int>(quadros.size()); i++) {
        int row = memoryTable->rowCount();
        memoryTable->insertRow(row);

        memoryTable->setItem(row, 0, new QTableWidgetItem(QString::number(i)));

        if (quadros[i].livre) {
            memoryTable->setItem(row, 1, new QTableWidgetItem("-"));
            memoryTable->setItem(row, 2, new QTableWidgetItem("-"));
            memoryTable->setItem(row, 3, new QTableWidgetItem("Livre"));
        } else {
            memoryTable->setItem(row, 1, new QTableWidgetItem(QString::number(quadros[i].pid)));
            memoryTable->setItem(row, 2, new QTableWidgetItem(QString::number(quadros[i].pagina)));
            memoryTable->setItem(row, 3, new QTableWidgetItem("Ocupado"));
        }
    }
}

void MainWindow::updateLog()
{
    auto logs = simulador.getLogs();

    logText->clear();

    for (const auto &log : logs) {
        logText->append(QString::fromStdString(log));
    }
}

void MainWindow::onCreateProcess()
{
    bool okPrioridade = false;
    int prioridade = QInputDialog::getInt(
        this,
        "Criar processo",
        "Prioridade:",
        1,      // valor padrão
        1,      // valor mínimo
        10,     // valor máximo
        1,      // passo
        &okPrioridade
    );

    if (!okPrioridade) {
        return;
    }

    bool okTempo = false;
    int tempoTotal = QInputDialog::getInt(
        this,
        "Criar processo",
        "Tempo total de CPU:",
        10,     // valor padrão
        1,      // valor mínimo
        100,    // valor máximo
        1,      // passo
        &okTempo
    );

    if (!okTempo) {
        return;
    }

    bool okPaginas = false;
    int quantidadePaginas = QInputDialog::getInt(
        this,
        "Criar processo",
        "Quantidade de páginas:",
        4,      // valor padrão
        1,      // valor mínimo
        20,     // valor máximo
        1,      // passo
        &okPaginas
    );

    if (!okPaginas) {
        return;
    }

    simulador.criarProcesso(
        prioridade,
        tempoTotal,
        quantidadePaginas
    );

    updateInterface();
}

void MainWindow::onTick()
{
    simulador.executarClock();
    updateInterface();
}

void MainWindow::onStartAuto()
{
    autoTimer->start();
}

void MainWindow::onPauseAuto()
{
    autoTimer->stop();
}

void MainWindow::onSendToIo()
{
    simulador.enviarProcessoAtualParaIO(3);
    updateInterface();
}

void MainWindow::onReset()
{
    autoTimer->stop();
    simulador.resetar();
    updateInterface();
}