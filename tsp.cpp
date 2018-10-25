#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

int ** criarVetor2D(int tamanho);
vector<int> vizinho(int **matriz, int tamanho);
int swap(int **matriz, int tamanho, vector<int> vizinho);
int calcular_custo(int **matriz, vector<int> caminho);
int opt2(int **matriz, int tamanho, vector<int> vizinho);

int main(){ 
	int tamanho = 0;
	ifstream grafo_matriz("arquivo.txt", ios::in);
  	//ifstream grafo_matriz("gr17.tsp", ios::in);
	grafo_matriz >> tamanho; //salva o tamanho da matriz
	int **matriz = criarVetor2D(tamanho);

	for (int i = 0; i < tamanho; i++){
		for (int j = 0; j < tamanho; j++){
			grafo_matriz >> matriz[i][j];
		}
	}
	auto solucao_inicial = vizinho(matriz, tamanho);
	cout << swap(matriz, tamanho, solucao_inicial);
	cout << endl;
	cout << opt2(matriz, tamanho, solucao_inicial);
	cout << endl;
	for(int i = 0; i < solucao_inicial.size()-1; i++){
		cout << " " << solucao_inicial[i];
	}
	cout << endl;
	/*
	for (int i = 0; i < tamanho; i++){
		for (int j = 0; j < tamanho; j++){
			cout << "" << matriz[i][j] << " ";
		}
		cout << endl;
	}
	*/
	return 0;
}



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

int swap(int **matriz, int tamanho, vector<int> vizinho){
	vector<int> temporario;
	temporario = vizinho;
	int custo_primario = calcular_custo(matriz, vizinho);
	int custo_temporario;
	for(int i = 1; i < tamanho; i++){
		temporario = vizinho;

		for(int j = i + 1; j < tamanho; j++){
			int aux = temporario[i];
			temporario[i] = temporario[j];
			temporario[j] = aux;

			custo_temporario = calcular_custo(matriz, temporario);

			if(custo_temporario < custo_primario){
				custo_primario = custo_temporario;
				vizinho = temporario;
				custo_temporario = calcular_custo(matriz, vizinho);
			}
		}
	}

return custo_temporario;
	
}

int calcular_custo(int **matriz, vector<int> caminho) {
    int custo = 0;

    for(int i = 0; i < caminho.size() - 1; i++) {
        custo = custo + matriz[caminho[i]][caminho[i + 1]];
    }

    return custo;
}


int opt2(int **matriz, int tamanho, vector<int> vizinho){
	vector <int> temporario;
	int custo_primario = calcular_custo(matriz, vizinho);
	int custo_temporario;
	temporario = vizinho;
	for(int j = 1; j < tamanho/2; j++){
		for(int i = j; i < tamanho/2; i++){
			
			int aux = temporario[tamanho - i];
			temporario[tamanho - i] = temporario[i];
			temporario[i] = aux;
			
			custo_temporario = calcular_custo(matriz, temporario);

			if(custo_temporario < custo_primario){
				custo_primario = custo_temporario;
				vizinho = temporario;
			}
			
		}
	}

	custo_temporario = calcular_custo(matriz, vizinho);

	return custo_temporario;
}


int ** criarVetor2D(int tamanho){
	int **a = new int*[tamanho];
	for(int i = 0; i < tamanho; i++)
		a[i] = new int[tamanho];
	for(int i = 0; i < tamanho; i++)
		for(int j = 0; j < tamanho; j++)
			a[i][j] = 0;
	return a;
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

