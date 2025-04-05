#include "main.h"

int main() {
    // Set OpenGL version to 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        "OpenGL", nullptr, nullptr
        );
    if (window == nullptr) {
        std::cerr << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader( reinterpret_cast<GLADloadproc>(glfwGetProcAddress) )) {
        std::cerr << "Error initializing GLAD" << std::endl;
        return 1;
    }

    int success;
    char infoLog[512];
    
    // Create three vertices, forming a triangle
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Create vertex array object to save all the upcoming state
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create vertex buffer object to store triangle
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind GL_ARRAY_BUFFER type to current VBO
    // GL_ARRAY_BUFFER now refers to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define vertex shader
    const char *vertexShaderSource =
        "#version 330 core\n"
        // Define one vertex attribute (input): a vec3 at location 0 called aPos
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        // gl_Position is the output of the vertex shader, always vec4
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0"
    ;

    // Create and compile vertex shader
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Define fragment shader
    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n"
    ;
    
    // Create and compile fragment shader
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders together, in a pipeline, to create shader program
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shaders, not needed after linking to program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define how to interpret vertex data (i.e. vertices[] we passed to the vertex shader)
    // 1. starts at location 0
    // 2. number of values in each vertex attribute is 3 (vec3)
    // 3. each value of a vertex attribute is a float
    // 5. space between vertex attributes is 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  // enable location 0 of vertex attributes
    
    // Enter render loop
    while ( !glfwWindowShouldClose(window) ) {
        processInput(window);

        // Clear buffer
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle using our pre-compiled shader
        // and the VAO state so the vertex shader can interpret vertices[] properly
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draw 3 vertices using the currently active shader,
        // the previously defined vertex attribute configuration
        // and the VBO’s vertex data (GL_ARRAY_BUFFER) (indirectly bound via the VAO).
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // could use EBOs and glDrawElements as well
        
        // Swap front/back buffers, whichever is done being drawn to
        // and process any I/O events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Called when window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    // Close on escape
    if ( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, true);
}
