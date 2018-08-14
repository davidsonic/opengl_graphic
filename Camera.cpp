//
// Created by jiali on 8/12/18.
//

#include <custom/Camera.h>

Camera::Camera(glm::vec3 startposition, glm::vec3 startup, GLfloat startyaw, GLfloat startpitch, GLfloat startmoveSpeed, GLfloat startturnSpeed){
    position=startposition;
    worldUp=startup;
    yaw=startyaw;
    pitch=startpitch;
    front=glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed=startmoveSpeed;
    turnSpeed=startturnSpeed;

    update();
}

void Camera::keyControl(bool *keys, GLfloat deltatime){

    GLfloat velocity= moveSpeed * deltatime;

    if(keys[GLFW_KEY_W]){
        position+= front*velocity;
    }
    if(keys[GLFW_KEY_D]){
        position+= right*velocity;
    }
    if(keys[GLFW_KEY_A]){
        position-= right*velocity;
    }
    if(keys[GLFW_KEY_S]){
        position-= front*velocity;
    }

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange){
    xChange*=turnSpeed;
    yChange*=turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if(pitch>89.0f)
        pitch=89.0f;
    if(pitch<-89.0f)
        pitch=-89.0f;

    update();

}

Camera::Camera(){}
Camera::~Camera(){}

glm::mat4 Camera::calculateViewMatrix(){
    return glm::lookAt(position, position+ front, up);
}
void Camera::update(){
    front.x=cos(glm::radians(yaw))* cos(glm::radians(pitch));
    front.y=sin(glm::radians(pitch));
    front.z=sin(glm::radians(yaw))* cos(glm::radians(pitch));
    front=glm::normalize(front);

    right=glm::normalize(glm::cross(front, worldUp));
    up=glm::normalize(glm::cross(right, front));

}