#include "memoria.hpp"

Memoria::Memoria(int quantidadeQuadros)
    : pageFaults(0)
{
    quadros.resize(quantidadeQuadros);
}

int Memoria::buscarQuadroLivre() const
{
    for (int i = 0; i < static_cast<int>(quadros.size()); i++) {
        if (quadros[i].livre) {
            return i;
        }
    }

    return -1;
}

int Memoria::buscarPagina(int pid, int pagina) const
{
    for (int i = 0; i < static_cast<int>(quadros.size()); i++) {
        if (!quadros[i].livre &&
            quadros[i].pid == pid &&
            quadros[i].pagina == pagina) {
            return i;
        }
    }

    return -1;
}

bool Memoria::acessarPagina(int pid, int pagina)
{
    int quadroExistente = buscarPagina(pid, pagina);

    if (quadroExistente != -1) {
        return true;
    }

    pageFaults++;

    int quadroLivre = buscarQuadroLivre();

    if (quadroLivre != -1) {
        quadros[quadroLivre].livre = false;
        quadros[quadroLivre].pid = pid;
        quadros[quadroLivre].pagina = pagina;
        ordemFIFO.push(quadroLivre);
        return false;
    }

    int quadroSubstituido = ordemFIFO.front();
    ordemFIFO.pop();

    quadros[quadroSubstituido].livre = false;
    quadros[quadroSubstituido].pid = pid;
    quadros[quadroSubstituido].pagina = pagina;

    ordemFIFO.push(quadroSubstituido);

    return false;
}

void Memoria::liberarProcesso(int pid)
{
    for (auto &quadro : quadros) {
        if (!quadro.livre && quadro.pid == pid) {
            quadro.livre = true;
            quadro.pid = -1;
            quadro.pagina = -1;
        }
    }

    std::queue<int> novaFila;

    for (int i = 0; i < static_cast<int>(quadros.size()); i++) {
        if (!quadros[i].livre) {
            novaFila.push(i);
        }
    }

    ordemFIFO = novaFila;
}

std::vector<QuadroMemoria> Memoria::getQuadros() const
{
    return quadros;
}

int Memoria::getPageFaults() const
{
    return pageFaults;
}

void Memoria::limpar()
{
    for (auto &quadro : quadros) {
        quadro.livre = true;
        quadro.pid = -1;
        quadro.pagina = -1;
    }

    while (!ordemFIFO.empty()) {
        ordemFIFO.pop();
    }

    pageFaults = 0;
}