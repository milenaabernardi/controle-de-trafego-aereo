#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip> // Para a formatação da Tabela
#include <cstdlib> // Para a função rand()
#include <ctime>   // Para a função time()
#include <unistd.h>
#define QNTVOOS 50

using namespace std;

struct Voo
{
    int numero;
    int prioridade;
    bool manobra;
    int tempoExecucao;

    bool operator<(const Voo &outro) const
    {
        if (prioridade != outro.prioridade)
        {
            return prioridade > outro.prioridade;
        }
        else
        {
            return numero > outro.numero;
        }
    }
};

// Fila de prioridades para pousos e decolagens
priority_queue<Voo> filaPrioridades;

// Fila de entrada para voos que solicitam pouso ou decolagem
queue<Voo> filaEntrada;
queue<Voo> filaAmostra;

// Vetor para armazenar as pistas
vector<Voo> pistas(3);

// Prototipagem das Funções
void inicializarFilaEntrada();
void simularControleTrafego();
Voo inicializaVoo(int num);
void mostraVoo(queue<Voo> filaEntrada);
void mostraControleTrafegoTabela(int tempo);
void mostrarFilaPrioridades();
void menu();

int main (){
    srand(time(NULL));
    inicializarFilaEntrada();
    menu();
    return 0;
}

void menu()
{
    int escolha;
    do
    {
        cout << "===== Menu =====" << endl;
        cout << "1. Iniciar simulacao de controle de trafego aereo" << endl;
        cout << "2. Mostrar voos na fila de entrada" << endl;
        cout << "3. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> escolha;

        switch (escolha)
        {
        case 1:
            cout << "Voos na fila de entrada:" << endl;
            mostraVoo(filaAmostra);
            simularControleTrafego();
            break;
        case 2:
            cout << "Voos na fila de entrada:" << endl;
            mostraVoo(filaAmostra);
            break;
        case 3:
            cout << "Saindo..." << endl;
            break;
        default:
            cout << "Opcao invalida! Escolha novamente." << endl;
            break;
        }
    } while (escolha != 3);
    sleep(3);
}

Voo inicializaVoo(int num)
{
    Voo voo;
    voo.numero = num;
    voo.prioridade = 1 + rand() % 3;
    voo.manobra = (voo.prioridade != 3);
    voo.tempoExecucao = 2;
    return voo;
}

void mostraVoo(queue<Voo> filaEntrada)
{
    system("cls");
    while (!filaEntrada.empty())
    {
        Voo v = filaEntrada.front();
        filaEntrada.pop();
        cout << "Numero: " << v.numero << endl
             << "Prioridade: " << v.prioridade << endl
             << "Manobra: " << (v.manobra ? "Pouso" : "Decolagem") << endl
             << endl;
    }
    int escolha;
    do
    {
        cout << "Pressione 0 para continuar: ";
        cin >> escolha;
    } while (escolha != 0);
    system("cls");
}

void inicializarFilaEntrada()
{
    for (int i = 0; i < QNTVOOS; i++)
    {
        filaEntrada.push(inicializaVoo(i + 1));
    }
    filaAmostra = filaEntrada;
}

void simularControleTrafego()
{
    int tempo = 0;
    bool todasLivres = false;

    while (!todasLivres || !filaPrioridades.empty() || !filaEntrada.empty())
    {
        todasLivres = true;

        // A cada instante de tempo, 4 voos saem da fila de entrada e entram na fila de prioridades
        for (int i = 0; i < 4 && !filaEntrada.empty(); i++)
        {
            Voo voo = filaEntrada.front();
            filaEntrada.pop();
            filaPrioridades.push(voo);
        }

        // Atualizar tempo de execução dos voos nas pistas
        for (int i = 0; i < 3; i++)
        {
            if (pistas[i].numero != 0)
            {
                todasLivres = false; // Ainda há pelo menos uma pista ocupada
                pistas[i].tempoExecucao--;
                if (pistas[i].tempoExecucao == 0)
                {
                    // Manobra concluída, pista fica livre
                    pistas[i] = {0, 0, false, 0};
                }
            }
        }

        // Alocar novos voos às pistas livres
        for (int i = 0; i < 3; i++)
        {
            if (pistas[i].numero == 0 && !filaPrioridades.empty())
            {
                Voo voo = filaPrioridades.top();
                filaPrioridades.pop();
                pistas[i] = voo;
                todasLivres = false; // Uma pista foi alocada
            }
        }

        if (todasLivres)
        {
            break; // Se todas as pistas estiverem livres, encerra a simulação
        }

        mostraControleTrafegoTabela(tempo);

        tempo++;
    }
    int escolha;
    do
    {
        cout << "Pressione 0 para sair: ";
        cin >> escolha;
        if (escolha != 0)
        {
            cout << "INVALIDO" << endl;
        }
    } while (escolha != 0);
    system("cls");
}

void mostrarFilaPrioridades()
{
    priority_queue<Voo> temp = filaPrioridades;
    while (!temp.empty())
    {
        Voo v = temp.top();
        temp.pop();
        cout << v.numero << " ";
    }
}

void mostraControleTrafegoTabela(int tempo)
{
    vector<string> linha(4);
    cout << endl;
    linha[0] = "Tempo: " + to_string(tempo);
    for (int i = 0; i < 3; i++)
    {
        if (pistas[i].numero != 0)
        {
            linha[i + 1] = "Voo: " + to_string(pistas[i].numero) + " | " +
                           (pistas[i].manobra ? "P" : "D") + "  | Priori: " +
                           to_string(pistas[i].prioridade) + "    ";
        }
        else
        {
            linha[i + 1] = "        Pista Livre         ";
        }
    }

    cout << linha[0] << endl;
    cout << "----------------------------------------------------------------------------------------------------------------" << endl;
    cout << "|           Pista 1          |          Pista 2           |          Pista 3           |" << endl;
    cout << "|" << setw(20) << linha[1] << "|" << setw(20) << linha[2] << "|" << setw(20) << linha[3] << "|" << endl;
    cout << "________________________________________________________________________________________________________________" << endl;
    cout << endl;
    // Exibir voos na fila de prioridades em espera
    cout << "Fila de Prioridades em Espera: ";
    mostrarFilaPrioridades();
    cout << endl
         << endl;

}