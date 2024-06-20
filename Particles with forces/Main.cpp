#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>



#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "chrono"
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(30ms);

#include "Shader.h"
#include "Model3D.h"

#include "P6/Vector.h"
#include "P6/Particle.h"
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "P6/ForceRegistry.h"
#include "P6/ForceGenerator.h"
#include "P6/DragForceGenerator.h"

<<<<<<< Updated upstream
float width = 700;
float height = 700;
=======
//Camera
#include "Camera/Camera.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"

float width = 800;
float height = 800;
>>>>>>> Stashed changes

float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;


//CREATING 3DMODELS
Model3D circ({ 0,0,0 });
Model3D circ2({ 0,0,0 });
Model3D circ3({ 0,0,0 });
Model3D circ4({ 0,0,0 });

//camera
OrthoCamera orthoCam({ 0,0,0 }, { 0,1,0 }, { 0.1,0.1,1 }, height, width, width, true);
PerspectiveCamera persCam({ 0,0,-400 }, { 0,1,0 }, { 0,0,1 }, 90.f, height, width, 800, false);//test view
glm::vec3 moveCam({ 0, 0, 0 });

float pitch = 0.0f;
float yaw = -90.0f;

int camState = 0;

//race
float rank = 1;
float gone = 0;


std::vector <P6::P6Particle*> res;

glm::vec3 camRotation(bool vertical, bool pos) {
    const float speed = 0.1f;


    if (vertical) {
        if (pos) {
            pitch += speed;//y
        }
        else {
            pitch -= speed;//y
        }

    }
    else {
        if (pos) {
            yaw += speed; //x
        }
        else {
            yaw -= speed; //x
        }

    }



    //making sure that you can't 360 via neck breaking
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    //setting front and the change

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * 400;
    direction.y = sin(glm::radians(pitch)) * 400;
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * 400;
    return  direction;
    //cameraPos = (direction);
    //std::cout << direction.x << "" << direction.y << "" << direction.z << std::endl;

    //Front = glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
}

void result() {
    int num = 1;
    std::string buff;
    for (int x = 0  ; x < res.size(); x++) {
        std::cout << res[x]->name << " Placement was " << res[x]->placement << std::endl;
        std::cout << "Magnitude of Velocity: " << res[x]->magVel << std::endl;
        std::cout << "Average Velocity: (" << res[x]->avgVel.x << "," << res[x]->avgVel.y << "," << res[x]->avgVel.z << ")m/s" << std::endl;
        std::cout << "Time Taken: " << res[x]->time << "secs" << std::endl << std::endl;
    }
    std::cout << "Enter any key to get out : ";
    std::cin >> buff;
   
}


bool AtCenter(P6::P6Particle obj) {
    if (obj.Position.x <=1.5 && obj.Position.x >= -1.5) {
        if (obj.Position.y <= 1.5 && obj.Position.y >= -1.5) {
            if (obj.Position.z <= 1.5 && obj.Position.z >= -1.5) {
                return true;
            }
            
        }
       
    }
    return false;
    
}
float  toHundreths(float num) {
    int conv = num * 100 + 0.5;//
    return (float)conv / 100.00f;
}

void race(P6::P6Particle* obj,float time) {
    obj->placement = rank;
    obj->time = toHundreths(time/1000);
    obj->magVel = toHundreths( obj->Velocity.Magnitude());
    obj->avgVel = P6::MyVector(toHundreths((obj->Velocity.x + obj->initVel.x)/2), toHundreths((obj->Velocity.y + obj->initVel.y) / 2), toHundreths((obj->Velocity.z + obj->initVel.z) / 2));
    rank++;
    obj->finished = true;
    res.push_back(obj);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_1)) {
        if (camState != 0) {
            camState = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2)) {
        if (camState != 1) {
            camState = 1;
        }
    }
    //yrot
    if (glfwGetKey(window, GLFW_KEY_A)) {
        moveCam = camRotation(false, false);
        if (camState == 0) {
            std::cout << "Orth: " << orthoCam.getCameraPos().x << "," << orthoCam.getCameraPos().y << "," << orthoCam.getCameraPos().z << std::endl;
        }
        if (camState == 1) {
            std::cout << "Perc: " << persCam.getCameraPos().x << "," << persCam.getCameraPos().y << "," << persCam.getCameraPos().z << std::endl;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        moveCam = camRotation(false, true);
        if (camState == 0) {
            std::cout << "Orth: " << orthoCam.getCameraPos().x << "," << orthoCam.getCameraPos().y << "," << orthoCam.getCameraPos().z << std::endl;
        }
        if (camState == 1) {
            std::cout << "Perc: " << persCam.getCameraPos().x << "," << persCam.getCameraPos().y << "," << persCam.getCameraPos().z << std::endl;
        }
    }

    //xRot
    if (glfwGetKey(window, GLFW_KEY_S)) {

        moveCam = camRotation(true, false);
        if (camState == 0) {
            std::cout << "Orth: " << orthoCam.getCameraPos().x << "," << orthoCam.getCameraPos().y << "," << orthoCam.getCameraPos().z << std::endl;
        }
        if (camState == 1) {
            std::cout << "Perc: " << persCam.getCameraPos().x << "," << persCam.getCameraPos().y << "," << persCam.getCameraPos().z << std::endl;
        }


    }
    if (glfwGetKey(window, GLFW_KEY_W)) {

        moveCam = camRotation(true, true);
        if (camState == 0) {
            std::cout << "Orth: " << orthoCam.getCameraPos().x << "," << orthoCam.getCameraPos().y << "," << orthoCam.getCameraPos().z << std::endl;
        }
        if (camState == 1) {
            std::cout << "Perc: " << persCam.getCameraPos().x << "," << persCam.getCameraPos().y << "," << persCam.getCameraPos().z << std::endl;
        }

    }






}




int main(void)
{
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height , "PC01 JAIRO RIO M. JAROPOJOP", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    
;    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    Shader sphereShader("Shaders/vertShader.vert", "Shaders/fragShader.frag");
        

   

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    




    //float vertices[] = {
    //    // positions          // colors           // texture coords
    //    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    //    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    //   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    //   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    //};
    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};
    /*EBO which order to draw indices
    * consists  generate,bindbuffer and buffer data
    * 
    * generate makes, bindbuffer sets what is currenting being edited
    * bufferdata copies our necessary data onto said buffer.\
    * 
    VBO, what the data is 
    consists of bind buffer and bufferdata
    VAO how the shaders see the data
    consists of glVertexAttribPointer 
    * how the shaders is supposed to know which object is one thing and which is another
    and glenablevertexattribpointer
    * how shader knows its active and to be used;
    */
    //=================models================//

    std::string path = "3D/sphere.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];


        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) +1]
        );

        
      
    }

    /*std::string path2 = "3D/myCube.obj";
    std::vector<tinyobj::shape_t> shapes2;
    std::vector<tinyobj::material_t> material2;
    std::string warning2, error2;

    tinyobj::attrib_t attributes2;

    bool success = tinyobj::LoadObj(
        &attributes2,
        &shapes2,
        &material2,
        &warning2,
        &error2,
        path2.c_str()
    );*/
    //=================VBO/VAO==========//

    unsigned int VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
   

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

   

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLintptr normalptr = (3 * sizeof(float));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)normalptr);
    glEnableVertexAttribArray(1);

    GLintptr UVptr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)UVptr);
    glEnableVertexAttribArray(2);

    


   
    
    

    

    
    //Camera inits
    glm::mat4 iden_Mat = glm::mat4(1.0f);
   
    glm::mat4 projection = glm::ortho(-width/2, width/2, -height/2, height/2, -400.f, 400.f);
    

    //Render Particles
    std::list<RenderParticle*> RenderParticles;
  
    //P6 WORLD
    P6::PhysicsWorld pWorld = P6::PhysicsWorld();

    


    //Particle initialize
    //red
    /*
    P6::P6Particle p1 = P6::P6Particle(1.0f, 
        P6::MyVector(-width/2,0, 201.f),
        P6::MyVector(0,0,0 ), 
        P6::MyVector(0.f, 0.f, 0.f));
    P6::MyVector dir = p1.Position.Direction();
    p1.Velocity = dir.ScalarMult(-80);
    p1.Acceleration = dir.ScalarMult(-14.5);
    p1.initVel = p1.Velocity;
    p1.name = "Red";

    p1.AddForce(P6::MyVector(6000, 0, 0));

    P6::MyVector color = P6::MyVector(1.0, 0.0, 0.0);

    pWorld.AddParticle(&p1);
    RenderParticle rp1 = RenderParticle(&p1, &circ, color, &sphereShader, &VAO, &fullVertexData);
    RenderParticles.push_back(&rp1);*/

<<<<<<< Updated upstream
    //green
    P6::P6Particle p2 = P6::P6Particle(1.0f,
        P6::MyVector(width/2, height / 2, 173.0f),
        P6::MyVector(0,0 ,0 ),
        P6::MyVector(0.f, 0.f, 0.f));
    dir = p2.Position.Direction();
    p2.Velocity = dir.ScalarMult(-90);
    p2.Acceleration = dir.ScalarMult(-8);
    p2.initVel = p2.Velocity;
    p2.name = "Green";
=======

    P6::MyVector color = P6::MyVector(1.0, 0.0, 0.0);
    int particleAmount = 0;
>>>>>>> Stashed changes

    color = P6::MyVector(0.0, 1.0, 0.0);
    pWorld.AddParticle(&p2);
    RenderParticle rp2 = RenderParticle(&p2, &circ2, color, &sphereShader, &VAO, &fullVertexData);
    RenderParticles.push_back(&rp2);
   

<<<<<<< Updated upstream
=======
    srand((unsigned)time(0));
    float randomXForce = 0, randomYForce = 0, randomZForce = 0, randomSize = 0;
    int coneRadius = 2000;
>>>>>>> Stashed changes

    //blue
    P6::P6Particle p3 = P6::P6Particle(1.0f,
        P6::MyVector(width/2, -height / 2, -300.f),
        P6::MyVector(0, 0, 0),
        P6::MyVector(0.f, 50.f, 0.f));
    dir = p3.Position.Direction();
    p3.Velocity = dir.ScalarMult(-130);
    p3.Acceleration = dir.ScalarMult(-1);
    p3.initVel = p3.Velocity;
    p3.name = "Blue";
    color = P6::MyVector(0.0, 0.0, 1.0);
    pWorld.AddParticle(&p3);
    RenderParticle rp3 = RenderParticle(&p3, &circ3, color, &sphereShader, &VAO, &fullVertexData);
    RenderParticles.push_back(&rp3);

    //yellow
    P6::P6Particle p4 = P6::P6Particle(1.0f,
        P6::MyVector(-width/2, -height / 2, -150.f),
        P6::MyVector(0, 0, 0),
        P6::MyVector(0.f, 50.f, 0.f));
    dir = p4.Position.Direction();
    p4.Velocity = dir.ScalarMult(-110);
    p4.Acceleration = dir.ScalarMult(-3);
    p4.initVel = p4.Velocity;
    p4.name = "Yellow";
    color = P6::MyVector(1.0, 1.0, 0.0);
    pWorld.AddParticle(&p4);
    RenderParticle rp4 = RenderParticle(&p4, &circ4, color, &sphereShader, &VAO, &fullVertexData);
    RenderParticles.push_back(&rp4);


    //drag
    /*
    P6::DragForceGenerator drag = P6::DragForceGenerator(0.14, 0.1);
    pWorld.forceRegistry.Add(&p1, &drag);*/



    //clock initialiaze
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::nanoseconds ns_tracker(0);
    int buffer;
   
    //1 m = 1 unit
    //1m = 1 px
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        curr_time = clock::now();

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);

        prev_time = curr_time;

        curr_ns += dur;
        ns_tracker += dur;
        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            auto ms_tracker = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker).count();

            curr_ns -= curr_ns;

<<<<<<< Updated upstream
            if (AtCenter(p1) && !p1.finished) {
                p1.Destroy();
                race(&p1,ms_tracker);
                
            }
            if (AtCenter(p2) && !p2.finished) {
                p2.Destroy();
                race(&p2,ms_tracker);
            }
            if (AtCenter(p3) && !p3.finished) {
                p3.Destroy();
                race(&p3,ms_tracker);
            }
            if (AtCenter(p4) && !p4.finished) {
                p4.Destroy();
                race(&p4, ms_tracker);
            }
=======
    
>>>>>>> Stashed changes
            //updates here
            pWorld.Update((float)ms.count() / 1000);
            
          
            
        }
        //std::cout << "normal upd" << std::endl;

        processInput(window);
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render here */
        
        //draws
        //red

        P6::MyVector converter{ 0,0,0 };

        switch (camState) {
        case 0:

            orthoCam.setCameraPos(moveCam);






            converter = P6::MyVector{ orthoCam.getCameraPos().x,orthoCam.getCameraPos().y ,orthoCam.getCameraPos().z };//this makes a myVector for...
            orthoCam.setFront(glm::vec3{ -converter.Direction().x,-converter.Direction().y,-converter.Direction().z });
            //THIS cause im insane, takes the the position, turns that to NDC and THEN we point AWAY so that it points hopefully to center.
            orthoCam.setViewMatrix();

            orthoCam.perfromSpecifics(&sphereShader);


            break;
        case 1:

            persCam.setCameraPos(moveCam);





            converter = P6::MyVector{ persCam.getCameraPos().x,persCam.getCameraPos().y ,persCam.getCameraPos().z };//this makes a myVector for...
            persCam.setFront(glm::vec3{ -converter.Direction().x,-converter.Direction().y,-converter.Direction().z });
            //THIS cause im insane, takes the the position, turns that to NDC and THEN we point AWAY so that it points hopefully to center.
            persCam.setViewMatrix();

            persCam.perfromSpecifics(&sphereShader);
            break;
        }
        
       
        
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin(); i != RenderParticles.end(); i++) {
            (*i)->Draw();
        }
        

       

   


        
       
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
<<<<<<< Updated upstream
        if (p1.finished && p2.finished && p3.finished&& p4.finished) {
            glfwSetWindowShouldClose(window, 1);
        }
=======
        
>>>>>>> Stashed changes
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    

    result();

    
    
   
    glfwTerminate();
    return 0;
}