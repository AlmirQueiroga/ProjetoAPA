#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

int ** criarVetor2D(int tamanho);
vector<int> vizinho(int **matriz, int tamanho);
vector<int> swap(int **matriz, int tamanho, vector<int> vizinho);
int calculaCusto(int **matriz, vector<int> caminho);
void imprimeSolucao(vector<int> solucao);
//int opt2(vector<int> solucao, int **matriz, int tamanho, int custo);
//vector<int> opt2Swap(vector<int> solucao, int limite1, int limite2);
vector <int> opt2(vector <int> solucao, int ** matriz, int tamanho);
void vnd(vector <int> solucao, int **matriz, int tamanho);
vector <int> inverte(vector <int> solucao, int limite1, int limite2, int tamanho);


int main(){ 
	int tamanho = 0;
	int limite1, limite2;
	//Leitura do arquivo
	ifstream grafo_matriz("pcv10.txt", ios::in);
	grafo_matriz >> tamanho; 
	int **matriz = criarVetor2D(tamanho);

	for (int i = 0; i < tamanho; i++){
		for (int j = 0; j < tamanho; j++){
			grafo_matriz >> matriz[i][j];
		}
	}
	//fim letura do arquivo
	
	auto solucao_inicial = vizinho(matriz, tamanho); //Solução inicial gerada com o algortimo do vizinho mais proximo
	//vector <int> tuim = inverte(solucao_inicial, 1, 9, tamanho);
	//cout << "INVERTIDO "; imprimeSolucao(tuim); 
	cout << "Solucao Inicial: "; imprimeSolucao(solucao_inicial);
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

	for (int i = 0; i < tamanho; i++)
		custo_final += matriz[caminho[i]][caminho[i+1]];

	
	cout << "Custo " << custo_final << endl;
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
//Busca Local
void vnd(vector <int> solucao, int **matriz, int tamanho){
	int numEstruturas = 2;
	int i = 1;
	int custo_atual;
	vector <int> melhor_solucao;
	melhor_solucao = solucao;


	while( i <= numEstruturas){
		if(i == 1){
			melhor_solucao = swap(matriz, tamanho, solucao);
			custo_atual = calculaCusto(matriz, melhor_solucao);
		}else if( i == 2){
			cout << "thales e gay\n";
			melhor_solucao = opt2(solucao, matriz, tamanho);
			custo_atual = calculaCusto(matriz, melhor_solucao);
		}
		int custo = calculaCusto(matriz, solucao);
		if(custo_atual < custo){
			solucao = melhor_solucao;
			i = 1;
		}else{
			i++;
		}
	}

	cout << "Melhor solução encontrada com custo: " << custo_atual << endl;
	imprimeSolucao(melhor_solucao);

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


vector <int> opt2(vector <int> solucao, int ** matriz, int tamanho){
	int custo = calculaCusto(matriz, solucao);
	vector <int> temporario;
	vector <int> final;
	int aux;

	temporario = solucao;

	for(int i = 1; i < tamanho - 1; i++){
		for(int j = 2; j < tamanho - 1; j++){
			temporario = inverte(temporario, i, j, tamanho);
			aux = calculaCusto(matriz, temporario);
			if(custo > aux){
				custo = aux;
				final = temporario;
			}
		}
	}

	return final;
}

/*
for(int i = 0; i < tamanho; i++){
		int custo = INT_MAX;
		int indice = 0;
		for(int j = 0; j < tamanho; j++){
			if(matriz[aux][j] < custo 
				&& matriz[aux][j] != 0
				&& find(solucao.begin(),solucao.end(), j+1) == solucao.end()
				) {
				custo = matriz[aux][j];
				indice = j;
			}
		}
		aux = indice; 
		solucao.push_back(indice+1);
		
	}
*/
