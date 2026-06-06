#include "simulador.hpp"

#include <algorithm>

Simulador::Simulador()
    : clock(0),
      proximoPid(1),
      escalonador(3),
      memoria(8),
      processoExecutando(nullptr)
{
    adicionarLog("Sistema iniciado.");
}

void Simulador::adicionarLog(const std::string &mensagem)
{
    logs.push_back("[Clock " + std::to_string(clock) + "] " + mensagem);
}

std::shared_ptr<Processo> Simulador::criarProcesso(
    int prioridade,
    int tempoTotal,
    int quantidadePaginas
)
{
    int pid = proximoPid++;

    auto processo = std::make_shared<Processo>(
        pid,
        "P" + std::to_string(pid),
        prioridade,
        tempoTotal,
        quantidadePaginas
    );

    todosProcessos.push_back(processo);
    escalonador.adicionarProcesso(processo);

    adicionarLog(
        "Processo " + processo->nome +
        " criado e inserido na fila de pronto."
    );

    return processo;
}

void Simulador::executarClock()
{
    clock++;

    atualizarIO();
    escalonarSeNecessario();
    executarProcessoAtual();
}

void Simulador::atualizarIO()
{
    std::vector<std::shared_ptr<Processo>> novaFilaIO;

    for (auto &processo : filaIO) {
        processo->executarIO();

        if (processo->ioFinalizado()) {
            escalonador.adicionarProcesso(processo);

            adicionarLog(
                "Processo " + processo->nome +
                " concluiu I/O e voltou para a fila de pronto."
            );
        } else {
            novaFilaIO.push_back(processo);
        }
    }

    filaIO = novaFilaIO;
}

void Simulador::escalonarSeNecessario()
{
    if (processoExecutando == nullptr && escalonador.temProcessoPronto()) {
        processoExecutando = escalonador.proximoProcesso();

        adicionarLog(
            "Processo " + processoExecutando->nome +
            " entrou em execução."
        );
    }
}

void Simulador::executarProcessoAtual()
{
    if (processoExecutando == nullptr) {
        adicionarLog("CPU ociosa.");
        return;
    }

    int indicePagina = 0;

    if (!processoExecutando->paginas.empty()) {
        int progresso = processoExecutando->tempoTotal - processoExecutando->tempoRestante;
        indicePagina = progresso % processoExecutando->paginas.size();
    }

    bool paginaJaEstavaNaMemoria = memoria.acessarPagina(
        processoExecutando->pid,
        indicePagina
    );

    if (paginaJaEstavaNaMemoria) {
        adicionarLog(
            "Processo " + processoExecutando->nome +
            " acessou a página " + std::to_string(indicePagina) +
            " na memória."
        );
    } else {
        adicionarLog(
            "Page fault no processo " + processoExecutando->nome +
            " ao acessar a página " + std::to_string(indicePagina) + "."
        );
    }

    processoExecutando->executarUmClock();

    adicionarLog(
        "Processo " + processoExecutando->nome +
        " executou 1 unidade de tempo."
    );

    if (processoExecutando->terminou()) {
        adicionarLog(
            "Processo " + processoExecutando->nome +
            " foi finalizado."
        );

        memoria.liberarProcesso(processoExecutando->pid);
        processoExecutando = nullptr;
        return;
    }

    // FIFO/FCFS:
    // Não existe preempção por quantum.
    // O processo continua executando até terminar ou ser enviado para I/O.
}

/*
void Simulador::executarProcessoAtual()
{
    if (processoExecutando == nullptr) {
        adicionarLog("CPU ociosa.");
        return;
    }

    int indicePagina = 0;

    if (!processoExecutando->paginas.empty()) {
        int progresso = processoExecutando->tempoTotal - processoExecutando->tempoRestante;
        indicePagina = progresso % processoExecutando->paginas.size();
    }

    bool paginaJaEstavaNaMemoria = memoria.acessarPagina(
        processoExecutando->pid,
        indicePagina
    );

    if (paginaJaEstavaNaMemoria) {
        adicionarLog(
            "Processo " + processoExecutando->nome +
            " acessou a página " + std::to_string(indicePagina) +
            " na memória."
        );
    } else {
        adicionarLog(
            "Page fault no processo " + processoExecutando->nome +
            " ao acessar a página " + std::to_string(indicePagina) + "."
        );
    }

    processoExecutando->executarUmClock();

    adicionarLog(
        "Processo " + processoExecutando->nome +
        " executou 1 unidade de tempo."
    );

    if (processoExecutando->terminou()) {
        adicionarLog(
            "Processo " + processoExecutando->nome +
            " foi finalizado."
        );

        memoria.liberarProcesso(processoExecutando->pid);
        processoExecutando = nullptr;
        return;
    }

    if (processoExecutando->quantumUsado >= escalonador.getQuantum()) {
        adicionarLog(
            "Quantum do processo " + processoExecutando->nome +
            " terminou. Processo voltou para a fila de pronto."
        );

        escalonador.adicionarProcesso(processoExecutando);
        processoExecutando = nullptr;
    } 
} */

void Simulador::enviarProcessoAtualParaIO(int tempoIO)
{
    if (processoExecutando == nullptr) {
        adicionarLog("Nenhum processo em execução para enviar para I/O.");
        return;
    }

    processoExecutando->enviarParaIO(tempoIO);
    filaIO.push_back(processoExecutando);

    adicionarLog(
        "Processo " + processoExecutando->nome +
        " foi enviado para I/O por " +
        std::to_string(tempoIO) + " clocks."
    );

    processoExecutando = nullptr;
}

void Simulador::alterarPrioridade(int pid, int novaPrioridade)
{
    for (auto &processo : todosProcessos) {
        if (processo->pid == pid) {
            processo->prioridade = novaPrioridade;

            adicionarLog(
                "Prioridade do processo " + processo->nome +
                " alterada para " + std::to_string(novaPrioridade) + "."
            );

            return;
        }
    }

    adicionarLog("Processo com PID " + std::to_string(pid) + " não encontrado.");
}

void Simulador::resetar()
{
    clock = 0;
    proximoPid = 1;

    processoExecutando = nullptr;

    todosProcessos.clear();
    filaIO.clear();
    logs.clear();

    escalonador.limpar();
    memoria.limpar();

    adicionarLog("Sistema resetado.");
}

int Simulador::getClock() const
{
    return clock;
}

/*
int Simulador::getQuantum() const
{
    return escalonador.getQuantum();
}*/

int Simulador::getPageFaults() const
{
    return memoria.getPageFaults();
}

std::shared_ptr<Processo> Simulador::getProcessoExecutando() const
{
    return processoExecutando;
}

std::vector<std::shared_ptr<Processo>> Simulador::getTodosProcessos() const
{
    return todosProcessos;
}

std::vector<std::shared_ptr<Processo>> Simulador::getFilaPronto() const
{
    return escalonador.getFilaPronto();
}

std::vector<std::shared_ptr<Processo>> Simulador::getFilaIO() const
{
    return filaIO;
}

std::vector<QuadroMemoria> Simulador::getQuadrosMemoria() const
{
    return memoria.getQuadros();
}

std::vector<std::string> Simulador::getLogs() const
{
    return logs;
}