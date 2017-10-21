#include<stdio.h>
//GLEW
#include <stdio.h>
#include <stdlib.h>
// Include GLEW

#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Nave
{

public:

	Model nave;
	Model tiro;
	bool atirar;
	int vidas;

	vector<glm::vec3> original_tiro; 
	vector<glm::vec3> desloc_tiro;
	glm::vec3 posicao_nave;
	// 

	Nave(Model nav, Model tir) {

		this->nave = nav;
		this->tiro = tir;
		this->vidas = 2;
		this->posicao_nave = glm::vec3(0.0f, -1.0f, 0.0f);
		this->atirar = false;
	}

	//Funçao que posiciona as balas no centro da nave e adiciona ao vetor de balas disparadas
	void bala() {

		if(!atirar)
			atirar = true;
	
		glm::vec3 desloc_inicial = glm::vec3(0.0f, 0.0f, 0.0f);
		this->desloc_tiro.push_back(desloc_inicial);
		this->original_tiro.push_back(this->posicao_nave);

	}
	//Função que apaga as balas quando elas passarem do final da tela;
	void destruir_bala(){
		for(int i= 0 ; i < this->desloc_tiro.size(); i++){

				if((this->desloc_tiro[i].y + this->original_tiro[i].y) > 1.2){
					this->desloc_tiro.erase(this->desloc_tiro.begin());
					this->original_tiro.erase(this->original_tiro.begin());
				}
				
		}
	}
	
};
