#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

// registro com as operações de cadastro de conta
struct cadastro
{
    string nome;
    string cpf;
    int numeroConta;
    int pixlidos;
    long double saldo;
    string pix[10];
};

// menu mostrando as operações disponíveis
void menu()
{
    cout << "Menu" << endl;
    cout << "(1) Cadastrar conta" << endl;
    cout << "(2) Consultar saldo" << endl;
    cout << "(3) Listar dados cadastrados ativos em ordem alfabetica" << endl;
    cout << "(4) Excluir dados" << endl;
    cout << "(5) Efetuar deposito" << endl;
    cout << "(6) Efetuar saque" << endl;
    cout << "(7) Alterar titular" << endl;
    cout << "(8) Alterar chave Pix " << endl;
    cout << "(9) Exportar contas para arquivo binario" << endl;
    cout << "(10) Sair" << endl;
    cout << endl;
}

void redimensiona(cadastro *&dados, int &tamanho) // tamanho=0
{
    cadastro *novo;                   // declarando ponteiro
    novo = new cadastro[tamanho + 3]; // alocando mais posicoes em tamanho

    copy(dados, dados + tamanho,
         novo); // copiando o que esta em dados, para o vetor novo

    delete[] dados;                // deletando o dados que so tem tres posicoes alocadas
    tamanho += 3;                  // aumentando tamanho, em qualquer escopo do codigo
    dados = new cadastro[tamanho]; // alocando o vetor, que aumenta de tamanho de
    // tres em tres

    copy(
        novo, novo + tamanho,
        dados); // copiando o vetor novo, para o vetor dados com o tamanho alocado
}

void cadastrar(cadastro *dados, int &tamanho, int capacidade, int &qtdPix)
{
    cout << "Por favor informe a quantidade de cadastros que serao realizados:" << endl;
    int nDados;    // variavel que vai armazenar o numero de cadastros
    cin >> nDados; // lendo o numero de dados
    cout << endl;

    int i = 0, jafoicadastrado = 0;
    while (i < nDados)
    {

        if (tamanho == capacidade)
        {
            redimensiona(dados, capacidade);
        }

        cout << "Digite o cpf: " << endl;
        cin >> dados[i].cpf;

        for (int x = 0; x < tamanho; x++)
        {
            if (x == i)
            {
                x++;
            }
            if (x < tamanho)
            {
                if (dados[i].cpf == dados[x].cpf)
                {
                    jafoicadastrado++;
                }
            }
        }

        if (jafoicadastrado == 0)
        {
            cout << "Digite o nome: " << endl;
            cin.ignore();
            getline(cin, dados[i].nome);
            cout << "Digite o numero da conta:" << endl;
            cin >> dados[i].numeroConta;
            cout << "Digite o valor do saldo:" << endl;
            cin >> dados[i].saldo;
            cout << "Informe a quantidade de chaves pix cadastradas, e digite abaixo:" << endl;
            cin >> qtdPix;
            dados[i].pixlidos = qtdPix;
            for (int j = 0; j < qtdPix; j++)
            {
                cin >> dados[i].pix[j];
            }
            i++;
            tamanho++;
        }
        else
        {
            cout << "ERRO! CPF JA CADASTRADO! DIGITE UM NOVO CPF!" << endl;
            ;
        }
        jafoicadastrado = 0;
        cout << endl;
    }
}

void exclui(string cpfexcluido, int tamanho, cadastro *&dados)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (cpfexcluido == dados[i].cpf)
        {
            dados[i].cpf = "0";
        }
    }
    cout << "DADOS EXCLUIDOS!" << endl
         << endl;
}

void listar(cadastro *dados, int tamanho, int qtdPix)
{

    int q = 0;
    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i].cpf > "0")
        {
            cout << dados[i].nome << endl
                 << dados[i].cpf << endl;
            //<< dados[i].numeroConta << endl
            //<< dados[i].saldo << endl;
            // while (q < dados[i].pixlidos)
            // {
            //    cout << dados[i].pix[q] << endl;
            //    q++;
            //}
        }
        q = 0;
    }
}
float buscarConta(cadastro *dados, int tamanho, int conta, int &indiceConsulta)
{
    float SaldoAtual;

    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i].numeroConta == conta)
        {
            indiceConsulta = i;
            SaldoAtual = dados[i].saldo;
            return SaldoAtual;
        }
        else
        {
            indiceConsulta = -1;
        }
    }
    return 0;
}

void depositar(cadastro *dados, int &indiceDeposito)
{
    cout << "Digite o valor que voce deseja depositar:" << endl;
    float valorDeposito;
    cin >> valorDeposito;
    dados[indiceDeposito].saldo += valorDeposito;
    cout << "\nAtual saldo agora eh: " << dados[indiceDeposito].saldo << endl
         << endl;
}

void sacar(cadastro *dados, int &indiceSaque)
{
    cout << "Digite o valor que voce deseja sacar" << endl;
    float valorSaque;
    cin >> valorSaque;

    if ((dados[indiceSaque].saldo - valorSaque) > 0)
    {
        dados[indiceSaque].saldo -= valorSaque;
        cout << "\nAtual saldo agora eh: " << dados[indiceSaque].saldo << endl
             << endl;
    }
    else
    {
        cout << "Nao e possivel sacar, nao existe esse valor na conta" << endl;
    }
}

void alteraPix(cadastro *&dados, string pixaalterar, int tamanho)
{
    string novopix;
    int verificapix = 0;
    for (int i = 0; i < tamanho; i++)
    {
        for (int x = 0; x < dados[i].pixlidos; x++)
        {
            if (pixaalterar == dados[i].pix[x])
            {
                cout << "Digite o novo pix:  ";
                cin >> novopix;
                dados[i].pix[x] = novopix;
                verificapix++;
            }
        }
    }
    if (verificapix == 0)
    {
        cout << "PIX NAO ENCONTRADO!!" << endl
             << endl;
    }
    verificapix = 0;
}

void merge(cadastro *&dados, int inicio, int meio, int fim, char opcao)
{
    cadastro *temp1 = new cadastro[meio - inicio + 1];
    cadastro *temp2 = new cadastro[fim - meio];

    for (int i = 0; i < meio - inicio + 1; i++)
    {
        temp1[i] = dados[inicio + i];
    }
    for (int i = 0; i < fim - meio; i++)
    {
        temp2[i] = dados[meio + 1 + i];
    }

    int i = 0, j = 0, k = inicio;

    if (opcao == '1') // ordena por cpf
    {

        while (i < meio - inicio + 1 && j < fim - meio)
        {
            if (temp1[i].cpf < temp2[j].cpf)
            {
                dados[k] = temp1[i];
                i++;
            }
            else
            {
                dados[k] = temp2[j];
                j++;
            }
            k++;
        }
    }
    else // ordena por nome
    {
        while (i < meio - inicio + 1 && j < fim - meio)
        {
            if (temp1[i].nome < temp2[j].nome)
            {
                dados[k] = temp1[i];
                i++;
            }
            else
            {
                dados[k] = temp2[j];
                j++;
            }
            k++;
        }
    }

    while (i < meio - inicio + 1)
    {
        dados[k] = temp1[i];
        i++;
        k++;
    }

    while (j < fim - meio)
    {
        dados[k] = temp2[j];
        j++;
        k++;
    }

    delete[] temp1;
    delete[] temp2;

    return;
}

void mergesort(cadastro *&dados, int const begin, int const end, char opcao)
{
    if (begin >= end)
        return;
    int mid = begin + (end - begin) / 2;
    mergesort(dados, begin, mid, opcao);
    mergesort(dados, mid + 1, end, opcao);
    merge(dados, begin, mid, end, opcao);
}

void alteraTitular(cadastro *&dados, int position)
{
    string novonome;
    cout << "Digite o nome do novo titular:  ";
    getline(cin, novonome);
    dados[position].nome = novonome;
}

void gravarArquivo(cadastro *dados)
{
    ofstream arq;
    arq.open("arquivo.bin", ios::binary); // abertura arquivo binario
    if (!arq)
    {
        cout << "erro";
    }
    else
    {
        arq.seekp(sizeof(cadastro) * 0, ios::end); // apontando pra ultima posicao de dados
        arq.write((const char *)(dados), sizeof(cadastro));
        arq.close();
    }
}

void consultaSaldo(cadastro *dados, string consultarosaldo, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (consultarosaldo == dados[i].cpf)
        {
            cout << "Seu saldo e:  " << dados[i].saldo << endl
                 << endl;
        }
    }
}

int main()
{

    int escolha = 0, verificasetem = 0, posicaonome = 0, verificacpf = 0;
    string cpfexcluido, pixparaalterar, titularparaalterar, consultarosaldo;
    int tamanho = 0, capacidade = 3, qtdPix = 0;
    cadastro *dados;                  /// criando ponteiro dados
    dados = new cadastro[capacidade]; // alocando cadastro com tamanho 3 capacidade

    while (escolha != 10)
    {
        menu();
        cin >> escolha;
        cout << endl;

        if (escolha == 1)
        {
            cadastrar(dados, tamanho, capacidade, qtdPix);
        }

        else if (escolha == 2)
        {
            cout << "Digite o CPF que deseja consultar o saldo:  ";
            cin >> consultarosaldo;
            for (int i = 0; i < tamanho; i++)
            {
                if (consultarosaldo == dados[i].cpf)
                {
                    verificacpf++;
                }
            }
            if (verificacpf == 0)
            {
                cout << "CPF NAO ENCONTRADO!!!" << endl
                     << endl;
            }
            else
            {
                consultaSaldo(dados, consultarosaldo, tamanho);
                verificacpf = 0;
            }
        }
        else if (escolha == 3)
        {
            char opcao;
            cout << "deseja ordenar por cpf(1) ou nome(2)?  ";
            cin >> opcao;
            mergesort(dados, 0, tamanho - 1, opcao);

            listar(dados, tamanho, qtdPix);
        }

        else if (escolha == 4)
        {
            cout << "Digite o cpf deseja excluir:  " << endl;
            cin >> cpfexcluido;

            for (int i = 0; i < tamanho; i++)
            {
                if (dados[i].cpf == cpfexcluido)
                {
                    verificacpf++;
                }
            }

            if (verificacpf > 0)
            {
                exclui(cpfexcluido, tamanho, dados);
                verificacpf = 0;
            }
            else
            {
                cout << "CPF NAO ENCONTRADO" << endl;
            }
        }

        else if (escolha == 5)
        {
            cout << "Para efetuar o deposito, digite o numero da conta" << endl;
            int numeroDeposito, indiceDeposito;
            cin >> numeroDeposito;
            buscarConta(dados, tamanho, numeroDeposito, indiceDeposito);
            if (indiceDeposito != -1)
            {
                depositar(dados, indiceDeposito);
            }
            else
            {

                cout << "Nao foi possivel efetuar o deposito,conta nao encontrada!" << endl;
            }
        }
        else if (escolha == 6)
        {
            cout << "Para efetuar o saque, digite o numero da conta" << endl;
            int numeroSaque, indiceSaque;
            cin >> numeroSaque;
            buscarConta(dados, tamanho, numeroSaque, indiceSaque);
            if (indiceSaque != -1)
            {
                sacar(dados, indiceSaque);
            }
            else
            {
                cout << "Nao foi possivel efetuar o saque,conta nao encontrada!" << endl;
            }
        }
        else if (escolha == 7)
        {
            cout << "Digite o nome de titular que deseja alterar:  ";
            cin.ignore();
            getline(cin, titularparaalterar);
            for (int zeta = 0; zeta < tamanho; zeta++)
            {
                if (dados[zeta].nome == titularparaalterar)
                {
                    verificasetem++;
                    posicaonome = zeta;
                }
            }
            if (verificasetem == 0)
            {
                cout << "TITULAR NAO ENCONTRADO" << endl;
            }
            else
            {
                alteraTitular(dados, posicaonome);
                verificasetem = 0;
            }
        }
        else if (escolha == 8)
        {
            cout << "Digite o PIX que deseja alterar:  ";
            cin >> pixparaalterar;
            alteraPix(dados, pixparaalterar, tamanho);
        }
        else if (escolha == 9)
        {
            gravarArquivo(dados);
        }
        else if (escolha == 10)
        {
            return 0;
        }
    }
}
