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


class Inimigo{
	
	public:
		
	Model alien;
	glm::vec3 posicao_alien;
	vector<glm::vec3> tiros;
	vector<glm::vec3> origem_tiros;
	GLfloat tempo_atual;
	GLfloat delta;
	GLfloat ultimo_tempo;
	bool atirando;
	
	Inimigo(Model alien, float delta, glm::vec3 p){
		
		this->alien = alien;
		this->delta = delta;
		this->tempo_atual = 0.0f;
		this->ultimo_tempo = 0.0f;
		this->posicao_alien = p;
	}
	//Funçao que posiciona a bala no centro da nave inimiga e adiciona ao seu vetor de balas disparadas
	void atirar_inimigo(){
		
		this->ultimo_tempo = glfwGetTime();
		
		if(this->atirando == false)
			this->atirando = true;
		glm::vec3 posicao = glm::vec3();
		this->origem_tiros.push_back(this->posicao_alien);
		this->tiros.push_back(posicao);
	}
	
	bool tempo_tiro(){
		
		this->tempo_atual = glfwGetTime();
		
		if(this->tempo_atual - this->ultimo_tempo > this->delta)
			return true;
		
		return false;
	}

};