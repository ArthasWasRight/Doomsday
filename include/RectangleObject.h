//
// Created by bajaks on 1/20/21.
//

#ifndef DOOMSDAY_RECTANGLEOBJECT_H
#define DOOMSDAY_RECTANGLEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#include <vector>
#include <iostream>
#include <string>

#define xTranslate1 (0.0f)
#define yTranslate1 (0.0f)
#define zTranslate1 (20.0f)
#define xScaling1 (4.0f)
#define yScaling1 (2.0f)
#define zScaling1 (4.0f)

class RectangleObject{
private:
    float vertices[32] = {
            // positions       //normals        // texture coords
            0.5f,  0.5f, 0.0f, 0.0f,0.0f,-1.0f, 1.0f, 1.0f,            // top right
            0.5f, -0.5f, 0.0f, 0.0f,0.0f,-1.0f, 1.0f, 0.0f,            // bottom right
            -0.5f, -0.5f, 0.0f,0.0f,0.0f,-1.0f, 0.0f, 0.0f,            // bottom left
            -0.5f,  0.5f, 0.0f,0.0f,0.0f,-1.0f,   0.0f, 1.0f,           // top left
    };
    unsigned int indices [6]={
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    unsigned int diffuse;
    unsigned int specular;
    Shader rectShader;
    glm::mat4 position=glm::mat4(1.0f);
    unsigned int loadTexture(char const * path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

//    std::vector<float> vertices;

//    std::vector<unsigned int> indices;
public:
    RectangleObject(std::string diffusePath,std::string specularPath) : rectShader("resources/shaders/rectangleObjectShader.vs","resources/shaders/rectangleObjectShader.fs")
    {
        //build wall

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//        // position attribute
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//
//        // texture coord attribute
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//
//        //normals attribute
//        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8* sizeof(float),(void*)(5*sizeof(float)));
//        glEnableVertexAttribArray(2);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //normals attribute
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8* sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // load and create a texture
        // -------------------------
        //FileSystem::getPath(specularPath).c_str()
        diffuse=loadTexture(diffusePath.c_str());
        specular=loadTexture(specularPath.c_str());
        rectShader.use();
        rectShader.setInt("material.diffuse",0);
        rectShader.setInt("material.specular",1);

    }
    void setup(glm::mat4 projection,glm::mat4 view,glm::vec3 viewPos)
    {
        rectShader.use();
        rectShader.setMat4("view",view);
        rectShader.setMat4("projection",projection);
        rectShader.setMat4("model",position);

        rectShader.setVec3("viewPosition",viewPos);
        rectShader.setFloat("material.shininess",32.0f);

        rectShader.setVec3("dirLight.direction",glm::vec3(0.0f,0.0f,-1.0f));
        rectShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
        rectShader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);

        //point lights
        rectShader.setVec3("pointLights[0].position", glm::vec3(xScaling1,yScaling1,zScaling1)*(glm::vec3(30.0f,0.0f,-30.0f)+glm::vec3(xTranslate1,yTranslate1,zTranslate1)));
        rectShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        rectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        rectShader.setFloat("pointLights[0].constant", 1.0f);
        rectShader.setFloat("pointLights[0].linear", 0.009);
        rectShader.setFloat("pointLights[0].quadratic", 0.0032);
        // point light 2
        rectShader.setVec3("pointLights[1].position", glm::vec3(xScaling1,yScaling1,zScaling1)*(glm::vec3(0.0f,15.0f,-30.0f+(60.0f*1/3))+glm::vec3(xTranslate1,yTranslate1,zTranslate1)));
        rectShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        rectShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        rectShader.setFloat("pointLights[1].constant", 1.0f);
        rectShader.setFloat("pointLights[1].linear", 0.006);
        rectShader.setFloat("pointLights[1].quadratic", 0.0016);

        //point light 3
        rectShader.setVec3("pointLights[2].position", glm::vec3(xScaling1,yScaling1,zScaling1)*(glm::vec3(-60.0f*1/3,15.0f,-30.0f)+glm::vec3(xTranslate1,yTranslate1,zTranslate1)));
        rectShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        rectShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        rectShader.setFloat("pointLights[2].constant", 1.0f);
        rectShader.setFloat("pointLights[2].linear", 0.006);
        rectShader.setFloat("pointLights[2].quadratic", 0.0016);

        //point light 4
        rectShader.setVec3("pointLights[3].position", glm::vec3(xScaling1,yScaling1,zScaling1)*(glm::vec3(60.0f*1/6*1/2-15.0f,15.0f,-50.0f)+glm::vec3(xTranslate1,yTranslate1,zTranslate1)));
        rectShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        rectShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        rectShader.setFloat("pointLights[3].constant", 1.0f);
        rectShader.setFloat("pointLights[3].linear", 0.006);
        rectShader.setFloat("pointLights[3].quadratic", 0.0016);

        //point light 4
        rectShader.setVec3("pointLights[4].position", glm::vec3(xScaling1,yScaling1,zScaling1)*(glm::vec3(60.0f*1/6*1/2+15.0f,15.0f,-50.0f)+glm::vec3(xTranslate1,yTranslate1,zTranslate1)));
        rectShader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
        rectShader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
        rectShader.setFloat("pointLights[4].constant", 1.0f);
        rectShader.setFloat("pointLights[4].linear", 0.006);
        rectShader.setFloat("pointLights[4].quadratic", 0.0016);


        //ceiling glass spotlight
        rectShader.setVec3("spotLight.position",glm::vec3(4.0f,2.0f,4.0f)*(glm::vec3(60.0f*1/6*1/2,15.0f,-30.0f-(60.0f*1/6*1/2))+glm::vec3(0.0f,0.0f,20.0f)));
        rectShader.setVec3("spotLight.direction",glm::vec3(0.0f,-1.0f,0.0f));
//        rectShader.setVec3("spotLight.position",viewPos);
//        rectShader.setVec3("spotLight.direction",front);
        rectShader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
        rectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        rectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        rectShader.setFloat("spotLight.constant", 1.0f);
        rectShader.setFloat("spotLight.linear", 0.002);
        rectShader.setFloat("spotLight.quadratic", 0.0008);
        rectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(30.0f)));
        rectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(45.0f)));
    }

//    void loadTexture(std::string texturePath)
//    {
//        glGenTextures(1, &texture1);
//        glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
//        // set the texture wrapping parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        // set texture filtering parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        // load image, create texture and generate mipmaps
//        int width, height, nrChannels;
//        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//        unsigned char *data = stbi_load(FileSystem::getPath(texturePath).c_str(), &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//            glGenerateMipmap(GL_TEXTURE_2D);
//        }
//        else
//        {
//            std::cout << "Failed to load texture" << std::endl;
//        }
//        stbi_image_free(data);
//        rectShader.use();
//        rectShader.setInt("texture1",0);
//    }

    void resetTransformation()
    {
        position=glm::mat4(1.0f);
    }
    void applyTransformation()
    {
        rectShader.use();
        rectShader.setMat4("model",position);
    }
    void translate(glm::vec3 vector)
    {
        position=glm::translate(position,vector);
    }
    void rotate(float angle,glm::vec3 vector)
    {
        position=glm::rotate(position,glm::radians(angle),vector);
    }
    void scale(glm::vec3 vector)
    {
        position=glm::scale(position,vector);
    }
    void draw()
    {
        applyTransformation();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,specular);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        // TODO: google how to unbind texture (?)
//        glActiveTexture(GL_TEXTURE0);

        resetTransformation();
    }

};

#endif //DOOMSDAY_RECTANGLEOBJECT_H
