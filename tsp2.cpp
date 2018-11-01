#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <time.h>

using namespace std;

class Aresta{
private:
	int origem, destino, peso;

public:
	Aresta(int origem, int destino, int peso){
		this->origem = origem;
		this->destino = destino;
		this->peso = peso;
		srand(time(NULL));
	}
	int getOrigem(){
		return origem;
	}
	int getDestino(){
		return destino;
	}
	int getPeso(){
		return peso;
	}
	
};

int ** criarVetor2D(int tamanho);
vector<int> vizinho(int **matriz, int tamanho);
vector<int> swap(int **matriz, int tamanho, vector<int> vizinho);
int calculaCusto(int **matriz, vector<int> caminho);
void imprimeSolucao(vector<int> solucao);
void vnd(vector <int> solucao, int **matriz, int tamanho);
vector <int> inverte(vector <int> solucao, int limite1, int limite2, int tamanho);
void exibevector(vector <Aresta> grafo);
vector <int> opt2(int **matriz, int tamanho, vector <int> vizinho);

bool compara(Aresta num1, Aresta num2){
	return (num1.getPeso() < num2.getPeso());
}
void grasp(vector <Aresta> lc, float alfa, int tamanho, int numMAX);
vector <Aresta> construtivoGrasp(vector <Aresta> lc, float alfa, int tamanho);

int main(){ 
	int tamanho = 0;
	int limite1, limite2;
	//Leitura do arquivo
	ifstream grafo_matriz("pcv10.txt", ios::in);
	grafo_matriz >> tamanho; 
	int **matriz = criarVetor2D(tamanho);

	vector <Aresta> lc;

	for (int i = 0; i < tamanho; i++){
		for (int j = 0; j < tamanho; j++){
			grafo_matriz >> matriz[i][j];
		}
	}
	//fim letura do arquivo
	//salva num vector de arestas

	for (int i = 0; i < tamanho; i++){
		for (int j = i + 1; j < tamanho; j++){
			Aresta aresta(i,j,matriz[i][j]);
			lc.push_back(aresta);
		}
	}
	//fim do vector de arestas
	grasp(lc, 0.25, tamanho,3);

	auto solucao_inicial = vizinho(matriz, tamanho); //Solução inicial gerada com o algortimo do vizinho mais proximo
	//vector <int> tuim = inverte(solucao_inicial, 1, 9, tamanho);
	//cout << "INVERTIDO "; imprimeSolucao(tuim); 
	cout << "Solucao Inicial: "; imprimeSolucao(solucao_inicial);
	vnd(solucao_inicial, matriz, tamanho); //Busca Local

	return 0;
}



void grasp(vector <Aresta> lc, float alfa, int tamanho, int numMAX){
	sort(lc.begin(), lc.end(), compara); //ordena a lista de candidas pelo custo
	int quantidade = alfa * (lc.size()); //guarda a porcentagem que será avaliada da lc
	int verificado[tamanho] = {0}; //array auxiliar para inserção na solucao
	vector <Aresta> solucao; //vector de saida
	int aux, aleatorio, i, indice, origem;

	srand(time(NULL));
	aux = quantidade - 1;
	aleatorio = rand() % aux; //gerando o numero aleatorio
	//seleciona o elemento aleatorio da lc
	indice = lc[aleatorio].getDestino(); 
	//guarda a origem do elemento escolhido
	origem = lc[aleatorio].getOrigem();
	//insere na solucao o primeiro elemento escolhido
	solucao.push_back(lc[aleatorio]);
	cout << "Solucao 1 :" ; exibevector(solucao);
	verificado[origem] = 1;
	//apaga todos as arestas que tem origem igual ao vertice já inserido na solução
	for(int i = 0; i < lc.size(); i++){
		if(lc[i].getOrigem() == origem){
			lc.erase(lc.begin() + i);
		}
	}

	i = 1;

	while( i < tamanho){
		quantidade = alfa * (lc.size()); //calcula a nova porcentagem
		aux = quantidade - 1;
		aleatorio = rand() % aux;

		indice = lc[aleatorio].getDestino();
		origem = lc[aleatorio].getOrigem();
		//Se o destino da aresta tiver segura...
		if(verificado[indice] == 0 && verificado[origem] == ){
			solucao.push_back(lc[aleatorio]);
			verificado[indice] = 1;
			i++;

			for(int i = 0; i < lc.size(); i++){
				if(lc[i].getOrigem() == origem){
					lc.erase(lc.begin() + 1);
				}
			}
		}

		

		cout << "solucao " << i << endl;
		exibevector(solucao);
		cout << endl;
		cout <<"iteracao: " << i << endl;
		exibevector(lc);

	
	}

	cout << "vector solucao" << endl;
	exibevector(solucao);
	


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

void exibevector(vector<Aresta> grafo){
	for(int i = 0; i < grafo.size(); i++){
		cout << " " << grafo[i].getOrigem();
		cout << "-" << grafo[i].getDestino();
		cout << " " << grafo[i].getPeso() << endl;
	}
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