#include "escalonador.hpp"

Escalonador::Escalonador(int quantum)
    : quantum(quantum)
{
}

void Escalonador::adicionarProcesso(const std::shared_ptr<Processo> &processo)
{
    processo->estado = EstadoProcesso::Pronto;
    processo->resetarQuantum();
    filaPronto.push(processo);
}

std::shared_ptr<Processo> Escalonador::proximoProcesso()
{
    if (filaPronto.empty()) {
        return nullptr;
    }

    auto processo = filaPronto.front();
    filaPronto.pop();

    processo->estado = EstadoProcesso::Executando;
    processo->resetarQuantum();

    return processo;
}

bool Escalonador::temProcessoPronto() const
{
    return !filaPronto.empty();
}

int Escalonador::getQuantum() const
{
    return quantum;
}

void Escalonador::setQuantum(int novoQuantum)
{
    if (novoQuantum > 0) {
        quantum = novoQuantum;
    }
}

std::vector<std::shared_ptr<Processo>> Escalonador::getFilaPronto() const
{
    std::queue<std::shared_ptr<Processo>> copia = filaPronto;
    std::vector<std::shared_ptr<Processo>> processos;

    while (!copia.empty()) {
        processos.push_back(copia.front());
        copia.pop();
    }

    return processos;
}

void Escalonador::limpar()
{
    while (!filaPronto.empty()) {
        filaPronto.pop();
    }
}