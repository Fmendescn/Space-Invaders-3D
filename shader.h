#ifndef SHADER_H
#define SHADER_H

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include <GL/glew.h>

class Shader{
	public:
		//O id do programa
		GLuint Program;
		//Construtor que le e compila os shaders
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
			string vertexCode;
			string fragmentCode;
			ifstream vShaderFile;
			ifstream fShaderFile;
			//Garantindo que os arquivos de shader possam lançar suas excessões
			vShaderFile.exceptions(ifstream::badbit);
			fShaderFile.exceptions(ifstream::badbit);
			try{
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				stringstream vShaderStream, fShaderStream;
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				//Fechando os handles dos arquivos
				vShaderFile.close();
				fShaderFile.close();
				//Convertemdp a stream em GLchar
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}catch(ifstream::failure e){
				cout << "ERROR::SHADER::FILE_NOT_SUCCEFULLY_READ" << endl;
			}
			const GLchar* vShaderCode = vertexCode.c_str();
			const GLchar* fShaderCode = fragmentCode.c_str();
			
			//Compilando os shaders
			GLuint vertex, fragment;
			GLint succes;
			GLchar infoLog[512];
			
			//Vertex Shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex,1,&vShaderCode, NULL);
			glCompileShader(vertex);
			//Testando se compilou tudo ok
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
			if(!succes){
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
			}
			
			//Fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment,1,&fShaderCode,NULL);
			glCompileShader(fragment);
			//Testando se compilou tudo ok
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
			if(!succes){
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
			}
			
			//Linkando o programa
			this->Program = glCreateProgram();
			glAttachShader(this->Program, vertex);
			glAttachShader(this->Program, fragment);
			glLinkProgram(this->Program);
			//Testando para erros
			glGetProgramiv(this->Program, GL_LINK_STATUS, &succes);
			if(!succes){
				glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
				cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << endl;
			}
			
			//Deletando os shaders pois eles já estão ligados ao meu programa
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
		//Usa o programa
		void Use(){
			glUseProgram(this->Program);
		}
};

#endif

