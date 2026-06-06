#ifndef MEMORIA_HPP
#define MEMORIA_HPP

#include <vector>
#include <queue>
#include <string>

struct QuadroMemoria
{
    bool livre;
    int pid;
    int pagina;

    QuadroMemoria()
        : livre(true), pid(-1), pagina(-1)
    {
    }
};

class Memoria
{
private:
    std::vector<QuadroMemoria> quadros;
    std::queue<int> ordemFIFO;
    int pageFaults;

    int buscarQuadroLivre() const;
    int buscarPagina(int pid, int pagina) const;

public:
    explicit Memoria(int quantidadeQuadros = 8);

    bool acessarPagina(int pid, int pagina);

    void liberarProcesso(int pid);

    std::vector<QuadroMemoria> getQuadros() const;

    int getPageFaults() const;

    void limpar();
};

#endif