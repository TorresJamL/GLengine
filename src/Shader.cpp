#include "Shader.hpp"

using namespace std;
string process_file(const char* filename) {
    ifstream in(filename, ios::binary);
    if (in) {
		string contents;
		in.seekg(0, ios::end);
		contents.resize(in.tellg());
		in.seekg(0, ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	cerr << "Something has gone awefully wrong." << endl;
	throw (errno);
}

Shader::Shader(cstr vertFilename, cstr fragFilename) {
    cstr vertSource = process_file(vertFilename).c_str();
    cstr fragSource = process_file(fragFilename).c_str();

    // Start with vertex shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    compileErrors(vertShader, "VERTEX");

    // Continue with fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    compileErrors(fragShader, "FRAGMENT");

    ID = glCreateProgram();

    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);
    compileErrors(ID, "PROGRAM");

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::compileErrors(GLuint shader, cstr type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cerr << "SHADER_COMPILATION_ERROR for: " << type << "\n" << endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cerr << "SHADER_LINKING_ERROR for: " << type << "\n" << endl;
		}
	}
}