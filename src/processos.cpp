#include "processos.hpp"

Processo::Processo(
    int pid,
    const std::string &nome,
    int prioridade,
    int tempoTotal,
    int quantidadePaginas
)
    : pid(pid),
      nome(nome),
      prioridade(prioridade),
      tempoTotal(tempoTotal),
      tempoRestante(tempoTotal),
      quantumUsado(0),
      tempoIO(0),
      estado(EstadoProcesso::Pronto)
{
    for (int i = 0; i < quantidadePaginas; i++) {
        paginas.push_back(i);
    }
}

void Processo::executarUmClock()
{
    if (tempoRestante > 0) {
        tempoRestante--;
        quantumUsado++;
    }

    if (tempoRestante == 0) {
        estado = EstadoProcesso::Finalizado;
    }
}


void Processo::resetarQuantum()
{
    quantumUsado = 0;
}

void Processo::enviarParaIO(int tempo)
{
    tempoIO = tempo;
    estado = EstadoProcesso::Bloqueado;
    resetarQuantum();
}

void Processo::executarIO()
{
    if (tempoIO > 0) {
        tempoIO--;
    }

    if (tempoIO == 0 && estado == EstadoProcesso::Bloqueado) {
        estado = EstadoProcesso::Pronto;
    }
}

bool Processo::terminou() const
{
    return estado == EstadoProcesso::Finalizado;
}

bool Processo::ioFinalizado() const
{
    return tempoIO == 0;
}

std::string Processo::estadoComoTexto() const
{
    switch (estado) {
        case EstadoProcesso::Novo:
            return "Novo";

        case EstadoProcesso::Pronto:
            return "Pronto";

        case EstadoProcesso::Executando:
            return "Executando";

        case EstadoProcesso::Bloqueado:
            return "Bloqueado";

        case EstadoProcesso::Finalizado:
            return "Finalizado";

        default:
            return "Desconhecido";
    }
}