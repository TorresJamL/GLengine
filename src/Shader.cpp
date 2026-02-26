#include "Shader.hpp"

using namespace std;
string process_file(const char* filename) {
    ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Cannot open file: " << filename << endl;
		throw errno;
	}
	string overall = "";
	string line;
    while (getline(file, line)) {
		overall += line + "\n";
    }

    file.close();
	return overall;  
}

Shader::Shader(const char* vertFilename, const char* fragFilename) {
	string vert_str = process_file(vertFilename);
	string frag_str = process_file(fragFilename);

    const char* vertSource = vert_str.c_str();
    const char* fragSource = frag_str.c_str();

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

void Shader::ApplyAspectRatio(float aspect) {
	GLuint aspectLoc = glGetUniformLocation(ID, "aspect");
	glUniform1f(aspectLoc, aspect);
}

void Shader::compileErrors(GLuint shader, cstr type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cerr << "SHADER_COMPILATION_ERROR for: " << type << "\n" << endl;
			cerr << infoLog << "\n" << endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cerr << "SHADER_LINKING_ERROR for: " << type << "\n" << endl;
			cerr << infoLog << "\n" << endl;
		}
	}
}