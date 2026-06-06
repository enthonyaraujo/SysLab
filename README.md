# SysLab

Simulador didático de Sistemas Operacionais inspirado no SOsim, desenvolvido para Linux usando **C++**, **Qt Widgets** e **CMake**.

O objetivo do projeto é demonstrar, de forma visual, conceitos fundamentais de Sistemas Operacionais, como:

- criação de processos;
- estados de processos;
- fila de pronto;
- processo em execução;
- fila de I/O;
- escalonamento de CPU;
- alteração de prioridade;
- clock do sistema;
- gerenciamento de memória;
- paginação;
- substituição de páginas por FIFO;
- registro de eventos em log.

---

## Tecnologias utilizadas

| Item | Escolha |
|---|---|
| Linguagem | C++ |
| Interface gráfica | Qt Widgets |
| Sistema de build | CMake |
| Escalonamento | Round Robin |
| Gerenciamento de memória | Paginação |
| Substituição de páginas | FIFO |

---

## Algoritmos escolhidos

### Escalonamento: Round Robin

O escalonamento de processos é feito usando **Round Robin**.

Nesse algoritmo, cada processo recebe uma fatia de tempo chamada **quantum**.

Funcionamento básico:

1. O primeiro processo da fila de pronto entra em execução.
2. O processo executa por uma quantidade limitada de clocks.
3. Se o processo terminar, ele é finalizado.
4. Se o processo solicitar I/O, ele vai para a fila de bloqueados.
5. Se o quantum acabar e o processo ainda não terminou, ele volta para o fim da fila de pronto.
6. O próximo processo da fila é escalonado.

Exemplo:

```text
Quantum = 3

Fila de pronto:
P1, P2, P3

P1 executa por 3 clocks.

Se P1 não terminou:
P1 volta para o fim da fila.

Nova fila:
P2, P3, P1
```

O Round Robin foi escolhido porque é simples, visual e fácil de demonstrar em uma apresentação curta.

---

### Memória: Paginação com FIFO

A memória física é simulada como um conjunto de **quadros de página**.

Cada processo possui páginas. Quando uma página precisa ser carregada na memória, o simulador verifica se há algum quadro livre.

Se houver quadro livre, a página é carregada.

Se a memória estiver cheia, ocorre substituição de página usando **FIFO**.

No FIFO, a página mais antiga na memória é removida para dar lugar à nova página.

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
├── include/
│   ├── processo.hpp
│   ├── escalonador.hpp
│   ├── memoria.hpp
│   ├── simulador.hpp
│   └── mainwindow.hpp
└── src/
    ├── main.cpp
    ├── processo.cpp
    ├── escalonador.cpp
    ├── memoria.cpp
    ├── simulador.cpp
    └── mainwindow.cpp
```

---

## Função de cada arquivo

### `CMakeLists.txt`

Arquivo de configuração do CMake.

Ele define:

- nome do projeto;
- versão do C++;
- dependência do Qt Widgets;
- arquivos `.cpp`;
- regras de compilação.

---

### `src/main.cpp`

Arquivo principal do programa.

Responsável por:

- iniciar a aplicação Qt;
- criar a janela principal;
- executar o loop da interface gráfica.

---

### `include/processo.hpp` e `src/processo.cpp`

Definem a classe `Processo`.

A classe armazena informações como:

- PID;
- nome;
- prioridade;
- tempo total;
- tempo restante;
- estado atual;
- quantum usado;
- tempo de I/O;
- páginas do processo.

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

Implementam o escalonador **Round Robin**.

Responsabilidades:

- armazenar a fila de pronto;
- adicionar processos à fila;
- escolher o próximo processo;
- devolver processos para o fim da fila;
- controlar o quantum.

---

### `include/memoria.hpp` e `src/memoria.cpp`

Implementam o gerenciamento de memória.

Responsabilidades:

- representar a memória física como quadros;
- verificar se uma página já está carregada;
- carregar páginas na memória;
- detectar page fault;
- substituir páginas usando FIFO;
- liberar páginas de processos finalizados.

---

### `include/simulador.hpp` e `src/simulador.cpp`

Contêm a lógica principal da simulação.

O simulador controla:

- clock global;
- processo em execução;
- fila de pronto;
- fila de I/O;
- criação de processos;
- execução de cada tick;
- acesso à memória;
- page faults;
- finalização de processos;
- geração de logs.

A cada tick do clock, o simulador deve:

```text
1. Incrementar o clock.
2. Atualizar processos em I/O.
3. Escalonar um processo se a CPU estiver livre.
4. Executar o processo atual.
5. Simular acesso a uma página de memória.
6. Verificar falta de página.
7. Verificar se o processo terminou.
8. Verificar se o quantum acabou.
9. Atualizar a interface.
```

---

### `include/mainwindow.hpp` e `src/mainwindow.cpp`

Implementam a interface gráfica com **Qt Widgets**.

A janela principal deve conter:

- indicador do clock;
- indicador do quantum;
- contador de page faults;
- botão para criar processo;
- botão para executar um clock;
- botão para executar automaticamente;
- botão para pausar;
- botão para enviar processo para I/O;
- botão para alterar prioridade;
- botão para resetar;
- tabela da fila de pronto;
- tabela do processo em execução;
- tabela da fila de I/O;
- tabela da memória;
- tabela com todos os processos;
- área de log.

---

## Interface esperada

Uma interface simples pode seguir este modelo:

```text
+------------------------------------------------------+
| Clock: 12        Quantum: 3        Page Faults: 4    |
+------------------------------------------------------+
| [Criar Processo] [Executar Clock] [Auto] [Pausar]    |
| [Alterar Prioridade] [Enviar para I/O] [Resetar]     |
+------------------------------------------------------+

Fila de pronto:
PID | Nome | Prioridade | Tempo restante | Estado

Processo em execução:
PID | Nome | Quantum usado | Tempo restante

Fila de I/O:
PID | Nome | Tempo restante de I/O

Memória:
Quadro | PID | Página | Estado

Log:
[Clock 1] Processo P1 criado.
[Clock 2] P1 entrou em execução.
[Clock 5] Quantum de P1 acabou.
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
./MiniSOsim
```

---

## Fluxo básico de uso

1. Abra o programa.
2. Crie alguns processos.
3. Observe os processos entrando na fila de pronto.
4. Execute o clock manualmente ou automaticamente.
5. Veja o processo em execução.
6. Observe a troca de contexto quando o quantum acabar.
7. Envie processos para I/O.
8. Observe processos bloqueados retornando para a fila de pronto.
9. Acompanhe o mapeamento das páginas na memória.
10. Verifique os eventos no log.

---

## Escopo mínimo para apresentação

Para a primeira versão funcional, o projeto deve conseguir:

- criar processos;
- inserir processos na fila de pronto;
- executar processos com Round Robin;
- controlar quantum;
- mover processo para I/O;
- retornar processo de I/O para pronto;
- finalizar processo;
- simular memória paginada;
- aplicar FIFO quando a memória estiver cheia;
- exibir tabelas na interface;
- gerar log dos eventos.

---

## Possíveis melhorias futuras

Depois que a versão básica estiver funcionando, é possível adicionar:

- Round Robin com prioridade;
- algoritmo de prioridade pura;
- algoritmo FIFO de CPU;
- algoritmo SJF;
- algoritmo LRU para memória;
- visualização colorida da memória;
- gráfico de uso da CPU;
- estatísticas de throughput;
- taxa de page faults;
- tempo médio de espera;
- salvamento do log em arquivo;
- carregamento de cenários de teste.

---

## Observação sobre prioridade

Nesta versão inicial, a prioridade pode ser armazenada e alterada, mas o algoritmo principal continua sendo **Round Robin puro**.

Ou seja:

```text
A prioridade aparece na interface.
A prioridade pode ser modificada.
Mas ela ainda não decide qual processo executa primeiro.
```

Uma melhoria possível é implementar **Round Robin com prioridade**, em que processos de maior prioridade são escolhidos antes, mas ainda respeitando quantum entre processos da mesma prioridade.

---

## Resumo do projeto

O MiniSOsim é um simulador didático de Sistemas Operacionais que demonstra:

```text
Processos + Escalonamento Round Robin + I/O + Paginação + FIFO
```

Ele não é um sistema operacional real. É uma simulação visual para fins educacionais.
