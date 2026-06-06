#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "processos.hpp"
#include "escalonador.hpp"
#include "memoria.hpp"

#include <memory>
#include <vector>
#include <string>

class Simulador
{
private:
    int clock;
    int proximoPid;

    Escalonador escalonador;
    Memoria memoria;

    std::shared_ptr<Processo> processoExecutando;

    std::vector<std::shared_ptr<Processo>> todosProcessos;
    std::vector<std::shared_ptr<Processo>> filaIO;

    std::vector<std::string> logs;

    void adicionarLog(const std::string &mensagem);
    void atualizarIO();
    void escalonarSeNecessario();
    void executarProcessoAtual();

public:
    Simulador();

    std::shared_ptr<Processo> criarProcesso(
        int prioridade,
        int tempoTotal,
        int quantidadePaginas
    );

    void executarClock();

    void enviarProcessoAtualParaIO(int tempoIO);

    void alterarPrioridade(int pid, int novaPrioridade);

    void resetar();

    int getClock() const;
    //int getQuantum() const;
    int getPageFaults() const;

    std::shared_ptr<Processo> getProcessoExecutando() const;

    std::vector<std::shared_ptr<Processo>> getTodosProcessos() const;
    std::vector<std::shared_ptr<Processo>> getFilaPronto() const;
    std::vector<std::shared_ptr<Processo>> getFilaIO() const;

    std::vector<QuadroMemoria> getQuadrosMemoria() const;

    std::vector<std::string> getLogs() const;
};

#endif