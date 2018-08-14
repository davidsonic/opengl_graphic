//
// Created by jiali on 8/9/18.
//

#include <custom/Shader.h>

Shader::Shader(){
    shaderID=0;
    uniformModel=0;
    uniformProjection=0;
    uniformView=0;
}


void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode){


    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragLocation){
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragString=ReadFile(fragLocation);

    const char* vertexCode=vertexString.c_str();
    const char* fragCode=fragString.c_str();

    CompileShader(vertexCode,fragCode);
}

std::string Shader::ReadFile(const char* fileLocation){
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if(!fileStream.is_open()){
        printf("Failed to open %s", fileLocation);
        return "";
    }

    std::string line="";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
//    std::cout<<content;
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode){
    shaderID = glCreateProgram();
    if (!shaderID){
        printf("Error creating shader");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result=0;
    GLchar elog[1024]={0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
        printf("Error linking program %s\n", elog);
        return ;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
        printf("Error Validating program %s\n", elog);
        return;
    }

    uniformModel=glGetUniformLocation(shaderID, "model");
    uniformProjection=glGetUniformLocation(shaderID, "projection");
    uniformView=glGetUniformLocation(shaderID, "view");

}

void Shader::UseShader(){
    glUseProgram(shaderID);

}

void Shader::ClearShader(){
    if(shaderID!=0){
        glDeleteProgram(shaderID);
        shaderID=0;
    }
    uniformModel=0;
    uniformProjection=0;
    uniformView=0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){
    GLuint theShader= glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0]= shaderCode;

    GLint codeLength[1];
    codeLength[0]=strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result=0;
    GLchar elog[1024]={0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        printf("Error compiling program %s\n", elog);
        return ;
    }
    glAttachShader(theProgram, theShader);

}

GLuint Shader::GetProjectionLocation(){
    return uniformProjection;
}
GLuint Shader:: GetModelLocation(){
    return uniformModel;
}

GLuint Shader::GetViewLocation(){
    return uniformView;
}

Shader::~Shader(){
    ClearShader();
}