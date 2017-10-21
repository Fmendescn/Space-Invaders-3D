#define GLUT_DISABLE_ATEXIT_HACK

#include<stdio.h>
//GLEW
#include <stdio.h>
#include <stdlib.h>
// Include GLEW
#include <GL/glew.h>

#include<GL/glut.h>
// Include GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Bibliotecas de som
#include <windows.h>        // Windows Header
#include <fmod/fmod.h>      // FMOD Header

#include"Camera.h"
#include"Shader.h"
#include"Model.h"
#include"Mesh.h"
#include"Nave.h"
#include"Inimigo.h"

//Protótipos de funções
void InitFMOD();
void criando_inimigos(Inimigo *ini, glm::mat4 matriz_inimigo, Shader shade);
void glutSpecialFunc() {}
GLuint loadCubemap(vector<const GLchar*> faces);

//static long font = (long)GLUT_BITMAP_TIMES_ROMAN_24;

//Variáveis Globais

int fase = 1;
int pont = 0;
int flag = 1;
double indice = 1.0;
int keys[1024];
bool tiro = false;
GLuint screenWidth = 800, screenHeight = 600;
Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 direita = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 esquerda = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 posicao = glm::vec3(0.0f, -7.0f, -20.0f);
glm::vec3 tras = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 frente = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 desloc = glm::vec3(0.0f, 0.0f, 0.0f);
vector<Inimigo> todos_inimigos;

#include <iostream>
using namespace std;


void movimentacao(Nave *nav)
{
	if(keys[GLFW_KEY_A]) {
		if(nav->posicao_nave.x > -1.1f)
			nav->posicao_nave += esquerda * (0.02f);
	}
	if(keys[GLFW_KEY_D])
		if(nav->posicao_nave.x < 1.1f) {
			nav->posicao_nave += direita * (0.02f);
		}
	if(keys[GLFW_KEY_SPACE]) {
		nav->bala();
		keys[GLFW_KEY_SPACE] = false;
	}

}

//Callback do Teclado
void janela(GLFWwindow *window, int key, int scancode, int action, int mode )
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(action == GLFW_PRESS)
		keys[key] = true;

	if(action == GLFW_RELEASE)
		keys[key] = false;
}
//Desenha o modelo
void drawModel(Model mode, Shader shade, glm::mat4 model)
{
	shade.Use();
	//substitui no shader
	glUniformMatrix4fv(glGetUniformLocation(shade.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	mode.Draw(shade);
}

//Escolher tempo aleatório do tiro do inimigo
double randomInteger ()
{
	int low = 1, high = 3;
	int k;
	double d;
	d = (double) rand () / ((double) RAND_MAX + 1);
	k = d * (high - low + 1);
	return low + k;
}

void criando_inimigos(Model modelo_ini, Shader shade)
{
	float delta;
	glm::vec3 posic = glm::vec3(-1.0f, 1.0f, 0.0f);
	glm::vec3 posic_y = glm::vec3(-1.0f, 0.65f, 0.0f);
	int numero_inimigos = 5;

	for(int i = 1; i <= numero_inimigos; i++) {
		delta = randomInteger();
		posic += glm::vec3(0.5f, 0.0f, 0.0f);
		Inimigo ini(modelo_ini, delta, posic);
		todos_inimigos.push_back(ini);

		delta = randomInteger();
		posic_y += glm::vec3(0.5f, 0.0f, 0.0f);
		Inimigo ini2(modelo_ini, delta, posic_y);
		todos_inimigos.push_back(ini2);
	}

}
//Detecta colisao entre uma nave e uma bala
bool colisao(float x1, float x2, float y1, float y2, float z1, float z2, float r1, float r2)
{

	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) <= (r1 + r2) * (r1 + r2));
}
//Verifica movimentacao dos inimigos
double verifica_movimentacao()
{
	//1 para desalocar para a esquerda
	//-1 para deslocar para a direita

	for(int i = 0 ; i < todos_inimigos.size(); i++) {
		if(todos_inimigos[i].posicao_alien.x >= 1.2) {
			return -1.0;
		}
		if(todos_inimigos[i].posicao_alien.x <= -1.2) {
			return 1.0;
		}
	}
	return indice;
}


float verifica_fase(){
	if(fase == 1){
		return 0.008;
	}
	if(fase == 2){
		return 0.03;
	}
	if(fase == 3){
		return 0.08;
	}
}
/*
Desenha os inimigos, movimenta-os para esquerda e direita, atira as balas dos inimigos e detecta se houve
colisão entre com a nave do jogador, e se houver, diminui uma vida até acabar o jogo.
*/
void enemy(glm::mat4 matriz_inimigo, Shader shade, Nave *nav)
{
	indice = verifica_movimentacao();
	for(int i = 0; i < todos_inimigos.size(); i++) {
		matriz_inimigo = glm::mat4();
		todos_inimigos[i].posicao_alien += (glm::vec3(indice * 0.01f, 0.00f, 0.0f));
		matriz_inimigo = glm::translate(matriz_inimigo, todos_inimigos[i].posicao_alien);
		matriz_inimigo = glm::scale(matriz_inimigo, glm::vec3(0.018f, 0.018f, 0.018f));
		matriz_inimigo = glm::rotate(matriz_inimigo, 180.0f, glm::vec3(0.0f, 0.0f, -1.0f));
		drawModel(todos_inimigos[i].alien, shade, matriz_inimigo);

		if(todos_inimigos[i].tempo_tiro())
			todos_inimigos[i].atirar_inimigo();
    
		for(int y = 0; y < todos_inimigos[i].tiros.size(); y++) {
			float z;
			z = verifica_fase();
			todos_inimigos[i].tiros[y] += tras * z;
			glm::mat4 model2 = glm::mat4();
			model2 = glm::translate(model2, todos_inimigos[i].origem_tiros[y]);
			model2 = glm::translate(model2, todos_inimigos[i].tiros[y]);
			model2 = glm::scale(model2, glm::vec3(0.003f, 0.001f, 0.003f));
			model2 = glm::rotate(model2, 180.0f, glm::vec3(0.0f, 1.0f, 1.0f));

			glm::vec3 posicao_bala = todos_inimigos[i].origem_tiros[y] + todos_inimigos[i].tiros[y];

			drawModel(nav->tiro, shade, model2);

			if(colisao(nav->posicao_nave.x, todos_inimigos[i].tiros[y].x + todos_inimigos[i].origem_tiros[y].x , nav->posicao_nave.y, todos_inimigos[i].tiros[y].y + todos_inimigos[i].origem_tiros[y].y, nav->posicao_nave.z, todos_inimigos[i].tiros[y].z + todos_inimigos[i].origem_tiros[y].z, 0.09, 0.09)) {

				todos_inimigos[i].tiros.erase(todos_inimigos[i].tiros.begin() + y);
				todos_inimigos[i].origem_tiros.erase(todos_inimigos[i].origem_tiros.begin() + y);
				nav->vidas--;
//      
			 if(nav->vidas == 0) {
			 		cout<< "\nVoce Morreu";
					cout << "\nPontuacao: " << pont;
					pont = 0;
					FSOUND_Close();
					exit(0);
					break;
				}
			}
		}
	}
}
int main()
{
	//Iniciando música de fundo
	InitFMOD ( );
	
	//Iniciando o GLUT
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);

	//Pegando a referencia da janela
	GLFWwindow* window = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
	glfwMakeContextCurrent(window);

	//Inicializar o GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Habilitando a profundidade
	glEnable(GL_DEPTH_TEST);

	//Criando os shaders
	Shader shade = Shader("VertexShader.vs", "FragmentShader.frag");
	Shader shaderSkybox = Shader("skyboxVS.vs", "skyboxFRAG.frag");

	//Chamando um model
	Model nave = Model();
	Model tiro = Model();
	Model model_inimigo = Model();
	nave.loadModel("tom/tom.obj");
	tiro.loadModel("AIM120D.obj");
	model_inimigo.loadModel("Flogger/Migr.obj");
	
	inicio:

	//Cria a nave
	Nave nav = Nave(nave, tiro);
	
	//Escrevendo na tela
	// escrever_tela((void*)font, "TESTANDO");
// 
	//Calculando Matrizes view  e perspectiva
	glm::mat4 project = glm::perspective(cam.Zoom, 800.0f / 600.0f, 0.1f, 200.f);
	glm::mat4 view = cam.GetViewMatrix();

	shade.Use();
	
	
	//substitui a uniforme no shader
	glUniformMatrix4fv(glGetUniformLocation(shade.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shade.Program, "projection"), 1, GL_FALSE, glm::value_ptr(project));

	glm::mat4 matriz_inimigo;

	criando_inimigos(model_inimigo, shade);

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//Setando o VAO do skybox
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	//Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("skybox/right.png");
	faces.push_back("skybox/left.png");
	faces.push_back("skybox/top.png");
	faces.push_back("skybox/bottom.png");
	faces.push_back("skybox/back.png");
	faces.push_back("skybox/front.png");
	GLuint cubemapTexture = loadCubemap(faces);
	

	while(!glfwWindowShouldClose(window)) {
		//Verifica o input
		glfwSetKeyCallback(window, janela);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Skybox
		//Desenhar skybox
		glDepthMask(GL_FALSE);
		shaderSkybox.Use();
		glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
		glm::mat4 projection = glm::perspective(cam.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderSkybox.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderSkybox.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//Cubo skybox
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shade.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		glfwPollEvents();

		movimentacao(&nav);

		//Desenhando a nave
		glm::mat4 model;

		model = glm::translate(model, nav.posicao_nave);
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));

		drawModel(nav.nave, shade, model);

		enemy(matriz_inimigo, shade, &nav);

		//Desenhando as balas de acordo com o numero de clicks
		if(nav.atirar) {
			for(int i = 0; i < nav.desloc_tiro.size(); i++) {
				nav.desloc_tiro[i] += frente * 0.03f;
				glm::mat4 model2 = glm::mat4();
				model2 = glm::translate(model2, nav.original_tiro[i]);
				model2 = glm::translate(model2, nav.desloc_tiro[i]);
				model2 = glm::scale(model2, glm::vec3(0.003f, 0.001f, 0.003f));
				model2 = glm::rotate(model2, 180.0f, glm::vec3(0.0f, -1.0f, 1.0f));

				glm::vec3 posicao_bala = nav.original_tiro[i] + nav.desloc_tiro[i];

				drawModel(nav.tiro, shade, model2);

				for(int j = 0 ; j < todos_inimigos.size(); j++) {

					if(colisao(todos_inimigos[j].posicao_alien.x, posicao_bala.x, todos_inimigos[j].posicao_alien.y, posicao_bala.y, todos_inimigos[j].posicao_alien.z, posicao_bala.z, 0.06, 0.06)) {
						todos_inimigos.erase(todos_inimigos.begin() + j);
						//cout<<"\ntamanho da lista: "<< todos_inimigos.size();
						nav.original_tiro.erase(nav.original_tiro.begin() + i);
						nav.desloc_tiro.erase(nav.desloc_tiro.begin() + i);
						pont += 100;
					}

					if(todos_inimigos.size() == 0) {
						if(fase == 3 ){
							cout<<"\nVoce Venceu !!!";
							cout<<"\nNumero de vidas: "<< nav.vidas;
							cout << "\nPontuacao:" << pont<<"\n";
							exit(0);
						   	break;
						}
							fase++; 
						goto inicio;
					}
				}
			}
			nav.destruir_bala();

		}
		//Limpando a Tela
		glfwSwapBuffers(window);
	}

}
//Carrega música de fundo
void    InitFMOD ( )
{
	FSOUND_SAMPLE * son = NULL;
	int channel;

	printf ( "Initializations ...\n" );
	FSOUND_SetOutput ( FSOUND_OUTPUT_DSOUND );
	FSOUND_SetDriver ( 0 );
	FSOUND_SetMixer ( FSOUND_MIXER_AUTODETECT );
	FSOUND_Init ( 44100, 32, 0 );

	printf ( "Loading ...\n" );
	son = FSOUND_Sample_Load ( FSOUND_FREE, "sound.mp3", FSOUND_HW2D, 0, 0);
	channel = FSOUND_PlaySoundEx ( FSOUND_FREE, son, NULL, FALSE );

	printf ( "Playing ...\n" );
	printf ( "Frequence : %d Hz ", FSOUND_GetOutputRate ( ) );
	printf ( "OR %d Hz\n", FSOUND_GetFrequency( channel ));
}
//Carrega Fundo da Tela
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for(GLuint i = 0; i < faces.size(); i++) {
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
