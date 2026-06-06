#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "processos.hpp"

#include <queue>
#include <memory>
#include <vector>

class Escalonador
{
private:
    std::queue<std::shared_ptr<Processo>> filaPronto;
    int quantum;

public:
    explicit Escalonador(int quantum = 3);

    void adicionarProcesso(const std::shared_ptr<Processo> &processo);

    std::shared_ptr<Processo> proximoProcesso();

    bool temProcessoPronto() const;

    int getQuantum() const;
    void setQuantum(int novoQuantum);

    std::vector<std::shared_ptr<Processo>> getFilaPronto() const;

    void limpar();
};

#endif