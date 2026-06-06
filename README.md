# SysLab

Simulador didático de Sistemas Operacionais inspirado no SOsim, desenvolvido para Linux usando **C++**, **Qt Widgets** e **CMake**.

O objetivo do projeto é demonstrar, de forma visual, conceitos fundamentais de Sistemas Operacionais, como:

* criação de processos;
* estados de processos;
* fila de pronto;
* processo em execução;
* fila de I/O;
* escalonamento de CPU;
* alteração de prioridade;
* clock do sistema;
* gerenciamento de memória;
* paginação;
* substituição de páginas por FIFO;
* registro de eventos em log.

---

## Tecnologias utilizadas

| Item                     | Escolha    |
| ------------------------ | ---------- |
| Linguagem                | C++        |
| Interface gráfica        | Qt Widgets |
| Sistema de build         | CMake      |
| Escalonamento de CPU     | FIFO/FCFS  |
| Gerenciamento de memória | Paginação  |
| Substituição de páginas  | FIFO       |

---

## Algoritmos escolhidos

### Escalonamento de CPU: FIFO/FCFS

O escalonamento de processos é feito usando **FIFO**, também conhecido como **FCFS — First-Come, First-Served**.

Nesse algoritmo, os processos são executados na ordem em que chegam à fila de pronto.

Funcionamento básico:

1. O primeiro processo da fila de pronto entra em execução.
2. O processo permanece usando a CPU até finalizar ou solicitar I/O.
3. Se o processo terminar, ele é finalizado e removido da CPU.
4. Se o processo solicitar I/O, ele vai para a fila de bloqueados.
5. Quando o I/O termina, o processo volta para o fim da fila de pronto.
6. O próximo processo da fila é escalonado.

Exemplo:

```text
Fila de pronto:
P1, P2, P3

P1 entra em execução.

Enquanto P1 não terminar e não solicitar I/O:
P1 continua executando.

Quando P1 termina:
P2 entra em execução.

Depois:
P3 entra em execução.
```

O FIFO/FCFS foi escolhido porque é simples, direto e fácil de demonstrar visualmente. Ele permite observar claramente a fila de pronto, o processo em execução e a ordem de atendimento dos processos.

---

### Memória: Paginação com FIFO

A memória física é simulada como um conjunto de **quadros de página**.

Cada processo possui páginas. Quando uma página precisa ser carregada na memória, o simulador verifica se há algum quadro livre.

Se houver quadro livre, a página é carregada.

Se a memória estiver cheia, ocorre substituição de página usando **FIFO**.

No FIFO de substituição de páginas, a página mais antiga na memória é removida para dar lugar à nova página.

Exemplo:

```text
Memória física com 4 quadros:

Quadro 0 -> Página 1
Quadro 1 -> Página 2
Quadro 2 -> Página 3
Quadro 3 -> Página 4

Chega a Página 5.

Como a memória está cheia, o FIFO remove a página mais antiga.

Resultado:

Quadro 0 -> Página 5
Quadro 1 -> Página 2
Quadro 2 -> Página 3
Quadro 3 -> Página 4
```

---

## Estrutura do projeto

```text
SysLab/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── include/
│   ├── processos.hpp
│   ├── escalonador.hpp
│   ├── memoria.hpp
│   └── simulador.hpp
├── src/
│   ├── processos.cpp
│   ├── escalonador.cpp
│   ├── memoria.cpp
│   └── simulador.cpp
└── ui/
    ├── mainwindow.hpp
    └── mainwindow.cpp
```

---

## Função de cada arquivo

### `CMakeLists.txt`

Arquivo de configuração do CMake.

Ele define:

* nome do projeto;
* versão do C++;
* dependência do Qt Widgets;
* arquivos `.cpp`;
* regras de compilação.

---

### `main.cpp`

Arquivo principal do programa.

Responsável por:

* iniciar a aplicação Qt;
* criar a janela principal;
* executar o loop da interface gráfica.

---

### `include/processos.hpp` e `src/processos.cpp`

Definem a classe `Processo`.

A classe armazena informações como:

* PID;
* nome;
* prioridade;
* tempo total;
* tempo restante;
* estado atual;
* tempo de I/O;
* páginas do processo.

Estados possíveis:

```text
Novo
Pronto
Executando
Bloqueado
Finalizado
```

---

### `include/escalonador.hpp` e `src/escalonador.cpp`

Implementam o escalonador **FIFO/FCFS**.

Responsabilidades:

* armazenar a fila de pronto;
* adicionar processos à fila;
* escolher o próximo processo;
* entregar à CPU o processo que chegou primeiro.

O escalonador utiliza uma fila, onde o primeiro processo inserido é o primeiro processo a ser executado.

---

### `include/memoria.hpp` e `src/memoria.cpp`

Implementam o gerenciamento de memória.

Responsabilidades:

* representar a memória física como quadros;
* verificar se uma página já está carregada;
* carregar páginas na memória;
* detectar page fault;
* substituir páginas usando FIFO;
* liberar páginas de processos finalizados.

---

### `include/simulador.hpp` e `src/simulador.cpp`

Contêm a lógica principal da simulação.

O simulador controla:

* clock global;
* processo em execução;
* fila de pronto;
* fila de I/O;
* criação de processos;
* execução de cada tick;
* acesso à memória;
* page faults;
* finalização de processos;
* geração de logs.

A cada tick do clock, o simulador deve:

```text
1. Incrementar o clock.
2. Atualizar processos em I/O.
3. Escalonar um processo se a CPU estiver livre.
4. Executar o processo atual.
5. Simular acesso a uma página de memória.
6. Verificar falta de página.
7. Verificar se o processo terminou.
8. Atualizar a interface.
```

---

### `ui/mainwindow.hpp` e `ui/mainwindow.cpp`

Implementam a interface gráfica com **Qt Widgets**.

A janela principal contém:

* indicador do clock;
* contador de page faults;
* botão para criar processo;
* botão para executar um clock;
* botão para executar automaticamente;
* botão para pausar;
* botão para enviar processo para I/O;
* botão para alterar prioridade;
* botão para resetar;
* tabela da fila de pronto;
* tabela do processo em execução;
* tabela da fila de I/O;
* tabela da memória;
* área de log.

---

## Interface esperada

Uma interface simples pode seguir este modelo:

```text
+------------------------------------------------------+
| Clock: 12                  Page Faults: 4            |
+------------------------------------------------------+
| [Criar Processo] [Executar Clock] [Auto] [Pausar]    |
| [Alterar Prioridade] [Enviar para I/O] [Resetar]     |
+------------------------------------------------------+

Fila de pronto:
PID | Nome | Prioridade | Tempo restante | Estado

Processo em execução:
PID | Nome | Tempo restante | Estado

Fila de I/O:
PID | Nome | Tempo restante de I/O | Estado

Memória:
Quadro | PID | Página | Estado

Log:
[Clock 1] Processo P1 criado.
[Clock 2] P1 entrou em execução.
[Clock 5] Processo P1 executou 1 unidade de tempo.
```

---

## Instalação das dependências

### Fedora

```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel
```

### Arch Linux

```bash
sudo pacman -S gcc cmake qt6-base
```

### Ubuntu/Debian

```bash
sudo apt install build-essential cmake qt6-base-dev
```

---

## Como compilar

Na raiz do projeto:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

## Como executar

Ainda dentro da pasta `build`:

```bash
./SysLab
```

---

## Fluxo básico de uso

1. Abra o programa.
2. Crie alguns processos.
3. Observe os processos entrando na fila de pronto.
4. Execute o clock manualmente ou automaticamente.
5. Veja o primeiro processo da fila entrar em execução.
6. Observe que, no FIFO/FCFS, o processo permanece executando até finalizar ou ir para I/O.
7. Envie processos para I/O, se desejar.
8. Observe processos bloqueados retornando para a fila de pronto.
9. Acompanhe o mapeamento das páginas na memória.
10. Verifique os eventos no log.

---

## Escopo mínimo para apresentação

Para a primeira versão funcional, o projeto deve conseguir:

* criar processos;
* inserir processos na fila de pronto;
* executar processos com FIFO/FCFS;
* mover processo para I/O;
* retornar processo de I/O para pronto;
* finalizar processo;
* simular memória paginada;
* aplicar FIFO quando a memória estiver cheia;
* exibir tabelas na interface;
* gerar log dos eventos.

---

## Possíveis melhorias futuras

Depois que a versão básica estiver funcionando, é possível adicionar:

* Round Robin;
* Round Robin com prioridade;
* algoritmo de prioridade pura;
* algoritmo SJF;
* algoritmo LRU para memória;
* visualização colorida da memória;
* gráfico de uso da CPU;
* estatísticas de throughput;
* taxa de page faults;
* tempo médio de espera;
* salvamento do log em arquivo;
* carregamento de cenários de teste.

---

## Observação sobre prioridade

Nesta versão inicial, a prioridade pode ser armazenada e alterada, mas o algoritmo principal continua sendo **FIFO/FCFS**.

Ou seja:

```text
A prioridade aparece na interface.
A prioridade pode ser modificada.
Mas ela ainda não decide qual processo executa primeiro.
```

Uma melhoria possível é implementar escalonamento por prioridade, em que processos de maior prioridade são escolhidos antes dos demais.

---

## Resumo do projeto

O SysLab é um simulador didático de Sistemas Operacionais que demonstra:

```text
Processos + Escalonamento FIFO/FCFS + I/O + Paginação + FIFO de páginas
```

Ele não é um sistema operacional real. É uma simulação visual para fins educacionais.
