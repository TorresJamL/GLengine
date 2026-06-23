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

GLuint Shader::getUniformLoc(const std::string &unif_name) const {
	return glGetUniformLocation(ID, unif_name.c_str());
}

void Shader::setBool(const std::string &name, bool value) const {         
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const { 
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const { 
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const { 
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec2(const std::string &name, float x, float y) const { 
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const { 
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const { 
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const { 
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const { 
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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