#ifndef PROCESSOS_HPP
#define PROCESSOS_HPP

#include <string>
#include <vector>

enum class EstadoProcesso
{
    Novo,
    Pronto,
    Executando,
    Bloqueado,
    Finalizado
};

class Processo
{
public:
    int pid;
    std::string nome;
    int prioridade;
    int tempoTotal;
    int tempoRestante;
    int quantumUsado;
    int tempoIO;
    EstadoProcesso estado;
    std::vector<int> paginas;

    Processo(
        int pid,
        const std::string &nome,
        int prioridade,
        int tempoTotal,
        int quantidadePaginas
    );

    void executarUmClock();
    void resetarQuantum();

    void enviarParaIO(int tempo);
    void executarIO();

    bool terminou() const;
    bool ioFinalizado() const;

    std::string estadoComoTexto() const;
};

#endif