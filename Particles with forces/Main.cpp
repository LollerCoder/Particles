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

float width = 800;
float height = 800;

float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;


//CREATING 3DMODELS
Model3D circ({ 0,0,0 });
Model3D circ2({ 0,0,0 });
Model3D circ3({ 0,0,0 });
Model3D circ4({ 0,0,0 });

//race
float rank = 1;
float gone = 0;

//Fireworks
bool isPaused = false;

std::vector <P6::P6Particle*> res;

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
    
    //yrot
    if (glfwGetKey(window, GLFW_KEY_A)) {
        yRot -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        yRot += 0.05;
    }

    //xRot
    if (glfwGetKey(window, GLFW_KEY_S)) {
        xRot -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        xRot += 0.05;
    }

    //zRot
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        zRot -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_E)) {
        zRot += 0.05;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        if (isPaused) isPaused = false;
        else isPaused = true;
    }
        
}


int GetRandomInt(int start, int end) {
    return (rand() % end) + start;
}

float GetRandomFloat(float start, float end) {
    return start + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (end - start)));
}

int main(void)
{
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height , "JAROCO Engine", NULL, NULL);
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
    RenderParticles.push_back(&rp1);


    //FIREWORK PARTICLES
    int particleAmount = 0;
    int fireworkParticleTracker = 1;

    std::cout << "How many particles? ";
    std::cin >> particleAmount;

    srand((unsigned)time(0));
    float randomXForce = 0, randomYForce = 0, randomZForce = 0, randomSize = 0;
    int coneRadius = 2500;

    //for (int i = 0; i < particleAmount; i++)
    //{
    //    //std::cout << std::endl << "creating particle...";

    //    //CREATING PARTICLE
    //    Model3D* newParticleModel = new Model3D({0,0,0});
    //    P6::P6Particle* newParticle = new P6::P6Particle(
    //        1.0f,
    //        P6::MyVector(0, -height/2.0f + 10.0f, 0),
    //        P6::MyVector(0, 0, 0),
    //        P6::MyVector(0.f, 0.f, 0.f)
    //        );

    //    //FORCE
    //    randomXForce = GetRandomInt(0, coneRadius);
    //    if (GetRandomInt(1, 2) <= 1) randomXForce *= -1;

    //    randomZForce = GetRandomInt(0, coneRadius);
    //    if (GetRandomInt(1, 2) <= 1) randomZForce *= -1;

    //    randomYForce = GetRandomInt(1000, 6000);
    //    //if (GetRandomInt(1, 2) <= 1) randomYForce *= -1;

    //    //std::cout << "Xforce: " << randomXForce << std::endl;
    //    //std::cout << "Yforce: " << randomYForce << std::endl << std::endl;

    //    newParticle->AddForce(P6::MyVector(randomXForce, randomYForce, randomZForce));

    //    //CHANGING COLOR
    //    color = P6::MyVector(
    //        GetRandomFloat(0.0f,1.0f), //r
    //        GetRandomFloat(0.0f,1.0f), //g
    //        GetRandomFloat(0.0f,1.0f) //b
    //    );

    //    //CHANGING SIZE
    //    randomSize = GetRandomFloat(2.0f,10.0f);
    //    //std::cout << "Rand size: " << randomSize << std::endl;
    //    newParticleModel->setScale(randomSize, randomSize, randomSize);

    //    //GIVING LIFE
    //    newParticle->lifeSpan = GetRandomFloat(1.0f, 10.f);

    //    //ADDING IT TO THE LISTS
    //    pWorld.AddParticle(newParticle);
    //    RenderParticle* newRP = new RenderParticle(newParticle, newParticleModel, color, &sphereShader, &VAO, &fullVertexData);
    //    RenderParticles.push_back(newRP);
    //}


    //drag
    P6::DragForceGenerator drag = P6::DragForceGenerator(0.14, 0.1);
    pWorld.forceRegistry.Add(&p1, &drag);



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

            if (AtCenter(p1) && !p1.finished) {
                p1.Destroy();
                race(&p1,ms_tracker);
                
            }

            //fireworks
            if (fireworkParticleTracker < 100) {
                Model3D* newFWPM = new Model3D({ 0,0,0 }); //FWPM short for firework particle model
                P6::P6Particle* newFWP = new P6::P6Particle(
                            1.0f,
                            P6::MyVector(0, -height/2.0f + 10.0f, 0),
                            P6::MyVector(0, 0, 0),
                            P6::MyVector(0.f, 0.f, 0.f)
                    );

                //FORCE
                randomXForce = GetRandomInt(0, coneRadius);
                if (GetRandomInt(1, 2) <= 1) randomXForce *= -1;

                randomZForce = GetRandomInt(0, coneRadius);
                if (GetRandomInt(1, 2) <= 1) randomZForce *= -1;

                randomYForce = GetRandomInt(1000, 6000);
                //if (GetRandomInt(1, 2) <= 1) randomYForce *= -1;

                //std::cout << "Xforce: " << randomXForce << std::endl;
                //std::cout << "Yforce: " << randomYForce << std::endl << std::endl;

                newFWP->AddForce(P6::MyVector(randomXForce, randomYForce, randomZForce));

                //CHANGING COLOR
                color = P6::MyVector(
                    GetRandomFloat(0.0f,1.0f), //r
                    GetRandomFloat(0.0f,1.0f), //g
                    GetRandomFloat(0.0f,1.0f) //b
                );

                //CHANGING SIZE
                randomSize = GetRandomFloat(2.0f,10.0f);
                //std::cout << "Rand size: " << randomSize << std::endl;
                newFWPM->setScale(randomSize, randomSize, randomSize);

                //GIVING LIFE
                newFWP->lifeSpan = GetRandomFloat(1.0f, 10.f);

                //ADDING IT TO THE LISTS
                pWorld.AddParticle(newFWP);
                RenderParticle* newRP = new RenderParticle(newFWP, newFWPM, color, &sphereShader, &VAO, &fullVertexData);
                RenderParticles.push_back(newRP);

                fireworkParticleTracker++;
            }

            //updates here
            if (!isPaused) {
                pWorld.Update((float)ms.count() / 1000);
            }
            else {
                std::cout << "Current particle count: " << RenderParticles.size() << std::endl;
            }
            
          
            
        }
        //std::cout << "normal upd" << std::endl;

        processInput(window);
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render here */
        
        glm::mat4 trans_Mat = glm::translate(iden_Mat, glm::vec3((glm::vec3)p1.Position));



        //X-rotation
        trans_Mat = glm::rotate(
            trans_Mat,
            glm::radians(xRot),
            glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))

        );//give quat

        //Y-rotation
        trans_Mat = glm::rotate(
            trans_Mat,
            glm::radians(yRot),
            glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))
        );

        //Z-rotation
        trans_Mat = glm::rotate(
            trans_Mat,
            glm::radians(zRot),
            glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))
        );

        //draws
        //red
        
        sphereShader.setMat4("projection", projection);
        
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin(); i != RenderParticles.end(); i++) {
            (*i)->Draw();
        }
        
       
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        if (p1.finished) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    

    result();

    
    
   
    glfwTerminate();
    return 0;
}