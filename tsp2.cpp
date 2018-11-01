#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <time.h>

using namespace std;

class CustoCandidato{
public:
	int custo, elemento;
	CustoCandidato(int i, int cust){
		elemento = i;
		this->custo = custo;
	}
};

int ** criarVetor2D(int tamanho);
vector<int> vizinho(int **matriz, int tamanho);
vector<int> swap(int **matriz, int tamanho, vector<int> vizinho);
int calculaCusto(int **matriz, vector<int> caminho);
void imprimeSolucao(vector<int> solucao);
vector<int>  vnd(vector <int> solucao, int **matriz, int tamanho);
vector <int> inverte(vector <int> solucao, int limite1, int limite2, int tamanho);
vector <int> opt2(int **matriz, int tamanho, vector <int> vizinho);
vector <int> GRC(int a, int tamanho, int **matriz);
CustoCandidato custo_calc(vector <int> solucao, int i, int ant, int **matriz);
void GRASP(int **matriz, int tamanho, int maxIteracao);
int minCusto(vector<CustoCandidato> &v);
int maxCusto(vector <CustoCandidato> &v);
bool procura_candidato(vector<int> v, int a);


int main(){ 
	int tamanho = 0;
	int limite1, limite2;
	//Leitura do arquivo
	//ifstream grafo_matriz("instancias/brazil58.txt", ios::in);
	//ifstream grafo_matriz("instancias/burma14.txt", ios::in);
	//ifstream grafo_matriz("instancias/gr17.txt", ios::in);
	//ifstream grafo_matriz("instancias/gr96.txt", ios::in);
	//ifstream grafo_matriz("instancias/gr120.txt", ios::in);
	//ifstream grafo_matriz("instancias/gr137.txt", ios::in);
	//ifstream grafo_matriz("instancias/gr229.txt", ios::in);
	//ifstream grafo_matriz("instancias/rbg323.txt", ios::in);
	ifstream grafo_matriz("instancias/si535.txt", ios::in);
	
	grafo_matriz >> tamanho; 
	int **matriz = criarVetor2D(tamanho);
	cout << "tamanho da matriz: " << tamanho << endl;

	for (int i = 0; i < tamanho; i++){
		for (int j = 0; j < tamanho; j++){
			grafo_matriz >> matriz[i][j];
		}
	}
	//fim letura do arquivo
	//salva num vector de arestas
	/*
	for (int i = 0; i < tamanho; i++){
		for (int j = i + 1; j < tamanho; j++){
			Aresta aresta(i,j,matriz[i][j]);
			lc.push_back(aresta);
		}
	}
	*/

	//exibir matriz lida

	//fim do vector de arestas

	auto solucao_inicial = vizinho(matriz, tamanho); //Solução inicial gerada com o algortimo do vizinho mais proximo
	//vector <int> tuim = inverte(solucao_inicial, 1, 9, tamanho);
	//cout << "INVERTIDO "; imprimeSolucao(tuim); 
	//cout << "Solucao Inicial: "; imprimeSolucao(solucao_inicial);
	GRASP(matriz, tamanho, 10);
	vnd(solucao_inicial, matriz, tamanho); //Busca Local

	return 0;
}

//Heuristica construtiva
vector<int> vizinho(int **matriz, int tamanho){

	int aux = 0;
	int custo_final = 0;
	vector <int> solucao;
	int verificado[tamanho] = {}; 
	solucao.push_back(0); 
	int caminho[tamanho] = {};

	caminho[0] = 0;
	verificado[0] = 1;


	for(int i = 0; i < tamanho; i++){
		int custo = INT_MAX;
		int indice = 0;
		for(int j = 0; j < tamanho; j++){
			if(!verificado[j] && custo > matriz[i][j]){
				indice = j;
				custo = matriz[i][j];
			}
		}

		caminho[i + 1] = indice;
		verificado[indice] = 1;
		solucao.push_back(indice);
		
	}
	caminho[tamanho] = 0;
	solucao.push_back(0);

	for (int i = 0; i < tamanho; i++){
		custo_final += matriz[caminho[i]][caminho[i+1]];
	}

	
	cout << "Custo da solucao gulosa: " << custo_final << endl;
	return solucao;
}

//Movimento de vizinhaça 1
vector<int> swap(int **matriz, int tamanho, vector<int> vizinho){
	vector<int> temporario;
	temporario = vizinho;
	int custo_primario = calculaCusto(matriz, vizinho);
	int custo_temporario;
	for(int i = 1; i < tamanho; i++){
		temporario = vizinho;

		for(int j = i + 1; j < tamanho; j++){
			int aux = temporario[i];
			temporario[i] = temporario[j];
			temporario[j] = aux;

			custo_temporario = calculaCusto(matriz, temporario);

			if(custo_temporario < custo_primario){
				custo_primario = custo_temporario;
				vizinho = temporario;
				custo_temporario = calculaCusto(matriz, vizinho);
			}
		}
	}

return vizinho;
	
}

void GRASP(int **matriz, int tamanho, int maxIteracao){
	int custo_solucao = INT_MAX;
	int total;

	vector <int> solucao;
	vector <int> melhor_solucao;

	for(int i = 0; i < maxIteracao; i++){
		solucao = GRC((float) rand() / (float) RAND_MAX, tamanho, matriz);
		solucao = vnd(solucao, matriz, tamanho);
		total = calculaCusto(matriz, solucao);
		if(total < custo_solucao){
			custo_solucao = total;
			melhor_solucao = solucao;
		}
	}

	cout << "Custo GRASP: " << custo_solucao << endl;
}

vector <int> GRC(int a, int tamanho, int **matriz){
	vector<int> solucao;
	solucao.push_back((int)(rand()%tamanho ));
	while(solucao.size() != tamanho){
		vector<CustoCandidato> custoRecurso;
		for(int i = 0; i < tamanho; i++){
			if(!procura_candidato(solucao,i))
				custoRecurso.push_back(custo_calc(solucao, i, solucao[solucao.size()-1], matriz));
		}

		vector<CustoCandidato> LCR;
		int Fcustominimo = minCusto(custoRecurso);
		int Fcustomaximo = maxCusto(custoRecurso);

		int decisao = Fcustominimo + a*(Fcustomaximo - Fcustominimo);

		for(int i = 0; i < custoRecurso.size(); i++){
			if(custoRecurso[i].custo <= decisao)
				LCR.push_back(custoRecurso[i]);
		}
		srand(clock());
		int aleatorio = rand() % (LCR.size());

		solucao.push_back(LCR[aleatorio].elemento);
	}
	solucao.push_back(solucao[0]);
	return solucao;
}

CustoCandidato custo_calc(vector <int> solucao, int i, int ant, int **matriz){
		return CustoCandidato(i, matriz[ant][i]);
}

int minCusto(vector<CustoCandidato> &v){
	int min = INT_MAX;

	for(int i = 0; i < v.size(); i++){
		if(v[i].custo < min)
			min = v[i].custo;
	}

	return min;
}

int maxCusto(vector <CustoCandidato> &v){
	int max = 0;

	for(int i = 0; i < v.size();i++){
		if(v[i].custo > max)
			max = v[i].custo;
	}

	return max;
}

bool procura_candidato(vector<int> v, int a){
	for(int i = 0; i < v.size();i++)
		if(a == v[i]) return true;
	return false;
}
//Busca Local
vector<int> vnd(vector <int> solucao, int **matriz, int tamanho){
	int numEstruturas = 2;
	int i = 1;
	int custo_atual;
	int custo;
	vector <int> melhor_solucao;
	melhor_solucao = solucao;


	while( i <= numEstruturas){
		if(i == 1){
			melhor_solucao = swap(matriz, tamanho, solucao);
			custo_atual = calculaCusto(matriz, melhor_solucao);
		}else if( i == 2){
			melhor_solucao = opt2(matriz, tamanho, solucao);
			custo_atual = calculaCusto(matriz, melhor_solucao);
			
		}
		custo = calculaCusto(matriz, solucao);
		if(custo_atual < custo){
			solucao = melhor_solucao;
			i = 1;
		}else{
			i++;
		}
	}

	//cout << "Custo VND: " << custo_atual << endl;
	//imprimeSolucao(melhor_solucao);
	return solucao;
}
//Calcula custo da solução
int calculaCusto(int **matriz, vector<int> caminho) {
    int custo = 0;

    for(int i = 0; i < caminho.size() - 1; i++) {
        custo = custo + matriz[caminho[i]][caminho[i + 1]];
    }

    return custo;
}

//Cria matriz
int ** criarVetor2D(int tamanho){
	int **a = new int*[tamanho];
	for(int i = 0; i < tamanho; i++)
		a[i] = new int[tamanho];
	for(int i = 0; i < tamanho; i++)
		for(int j = 0; j < tamanho; j++)
			a[i][j] = 0;
	return a;
}

//Exibe na tela a solução
void imprimeSolucao(vector<int> solucao){
	for(int i = 0; i < solucao.size() - 1; i++){
		cout << " " << solucao[i];
	}
	cout << endl;
}

vector <int> inverte(vector <int> solucao, int limite1, int limite2, int tamanho){
	vector <int> temporario;

	temporario = solucao;

	for(int i = limite1, j = limite2; i <= limite2; i++, j--){
		solucao[i] = temporario[j];
	}

	return solucao;
}


vector <int> opt2(int **matriz, int tamanho, vector <int> vizinho){
	vector <int> temporario;
	vector <int> inicial;
	inicial = vizinho;

	int custo_primario = calculaCusto(matriz, vizinho);
	int custo_temporario;
	temporario = vizinho;

	for(int i = 1; i < tamanho; i++){
		for(int j = i + 1; j < tamanho; j++){
			temporario = inverte(temporario, i, j, tamanho);

			custo_temporario = calculaCusto(matriz, temporario);
			if(custo_temporario < custo_primario){
				custo_primario = custo_temporario;
				vizinho = temporario;
			}
			temporario = inicial;
		}
	}

	return vizinho;
}