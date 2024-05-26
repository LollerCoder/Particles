#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>



#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "chrono"
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(30ms);

#include "P6/Vector.h"
#include "P6/Particle.h"

float width = 600;
float height = 600;

float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;

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

        
    
    
        
}




int main(void)
{
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height , "Jaropojop und Reblando", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    
;    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::fstream vertSrc("Shaders/vertShader.vert");
    std::stringstream vertBuff;

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/fragShader.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //create vertex shader(used for movements)
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertShader, 1, &v, NULL);

    glCompileShader(vertShader);

    GLint isCompiled2 = 0;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled2);
    if (isCompiled2 == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertShader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        //glDeleteShader(vertShader); // Don't leak the shader.
        //return;
        std::cout << &errorLog[0] << std::endl;
    }
    



    //create frag shader (our objects are turned into pixels/fragments which we will use to color in an object)
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShader, 1, &f, NULL);

    glCompileShader(fragShader);

    

    GLint isCompiled = 0;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        //glDeleteShader(fragShader); // Don't leak the shader.
        //return;
        std::cout << &errorLog[0] << std::endl;
    }


    

    //create shader program that'll just run both frag and vert together as one.
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);//compile to make sure computer remembers
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

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
    //float screen_scale = 40.0f;
    //glm::mat4 projection = glm::ortho(-screen_scale, screen_scale, -screen_scale, screen_scale, -screen_scale, screen_scale);
    glm::mat4 projection = glm::ortho(-width/2, width/2, -height/2, height/2, -100.f, 100.f);
    
    //Vectors
    /*
    P6::MyVector sample(1.0f, 2.0f, 3.0f);
    P6::MyVector addTo(2.0f, 2.0f, 3.0f);

   
    float toMult = 2.0f;

    //sample = sample + addTo;
    sample += addTo;
    std::cout << "Magnitude: " << sample.Magnitude() << std::endl;
    std::cout << "Direction: " << sample.Direction().x <<" " << sample.Direction().y << " " << sample.Direction().z << std::endl;
    std::cout << "x: " << sample.x << " y:" << sample.y << " z:" << sample.z << std::endl;
    glm::vec3 test = sample.VectorProduct(addTo);
    std::cout << "Vector Product: " << test.x << " " << test.y << " " << test.z << std::endl;
    sample -= addTo;
    std::cout << "x: " << sample.x << " y:" << sample.y << " z:" << sample.z << std::endl;

    sample *= addTo;
    std::cout << "x: " << sample.x << " y:" << sample.y << " z:" << sample.z << std::endl;

    std::cout << "DotProduct: " << sample.DotProduct(addTo) << std::endl;
    */
    
    //asking velocity start
    float x, y, z;

    std::cout << "Input the Ball's initial Velocity" << std::endl;

    std::cout << "X:";
    std::cin >> x;

    std::cout << "Y:";
    std::cin >> y;

    std::cout << "Z:";
    std::cin >> z;


    //Particle initialize
    P6::P6Particle particle = P6::P6Particle(1.0f, 
        P6::MyVector(0.f,height/-2 + 1, 0.f),
        P6::MyVector(x, y, z), 
        P6::MyVector(0.f, -50.f, 0.f));


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
            auto ms_tracker = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker);

            curr_ns -= curr_ns;

            //updates here
            particle.Update((float)ms.count() / 1000);

            //stop when hit ground
            if (particle.Position.y <= height / -2) {
                std::cout << "It took " << ms_tracker.count() / 1000 << " seconds ";
                std::cout << "for it to land" << std::endl;
                std::cout << "Input any letter/number to close..." << std::endl;
                std::cin >> buffer;
                glfwSetWindowShouldClose(window, 1);
            }
        }
        //std::cout << "normal upd" << std::endl;

        processInput(window);
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render here */
        
        glm::mat4 trans_Mat = glm::translate(iden_Mat, glm::vec3((glm::vec3)particle.Position));



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
        
        glUseProgram(shaderProg);

        GLuint tex = glGetUniformLocation(shaderProg, "ourTexture");
        glUniform1i(tex, 0);

        GLuint tex1 = glGetUniformLocation(shaderProg, "ourText");
        glUniform1i(tex1, 1);

        
        GLuint transLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans_Mat));

        GLuint projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);


        
       
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glDeleteProgram(shaderProg);
    glfwTerminate();
    return 0;
}