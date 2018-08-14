//
// Created by jiali on 8/9/18.
//

#include <custom/Window.h>

Window::Window(){
    width=800;
    height=600;

    for (size_t i=0;i<1024;i++)
        keys[i]=0;
}

Window::Window(GLint windowWidth, GLint windowHeight){
    width=windowWidth;
    height=windowHeight;

    for( size_t i=0;i<1024;i++)
        keys[i]=0;
}

int Window::Initialize(){
    if (!glfwInit()){
        printf("GLFW initialized failed!");
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow =glfwCreateWindow(width, height, "TEST WINDOW",NULL, NULL);

    if(!mainWindow){
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }


    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    //Handle key and mouse input
    createCallbacks();
//    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental=GL_TRUE;

    if(glewInit()!=GLEW_OK){
        printf("GLEW init failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action,
                int mode){
    Window* theWindow= static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key>=0 && key<1024){
        if(action==GLFW_PRESS){
            theWindow->keys[key]=true;
//            printf("Pressed: %d\n", key);
        }
        else if(action==GLFW_RELEASE){
            theWindow->keys[key]=false;
//            printf("Releaseed: %d\n", key);
        }
    }

}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos){
    Window* theWindow= static_cast<Window*> (glfwGetWindowUserPointer(window));
    if(theWindow->mouseFirstMove){
        theWindow->lastX=xPos;
        theWindow->lastY=yPos;
        theWindow->mouseFirstMove=false;
    }

    theWindow->xChange=xPos-theWindow->lastX;
    theWindow->yChange=theWindow->lastY-yPos;

    theWindow->lastX=xPos;
    theWindow->lastY=yPos;

//    printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

void Window::createCallbacks(){
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}


GLfloat Window::getXchange(){
    GLfloat theChange=xChange;
    xChange=0.0f;
    return theChange;
}



GLfloat Window::getYchange(){
    GLfloat theChange=yChange;
    yChange=0.0f;
    return theChange;
}

Window::~Window(){
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}