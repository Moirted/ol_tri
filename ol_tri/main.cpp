#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

// ��� ���������� �������
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)";

// ��� ������������ �������
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // ������ ����
    }
)";

int main() {
    // �������� ���� SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + OpenGL Triangle");
    window.setActive(true);

    // ������������� GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // ���������� ���������� �������
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // �������� ������� ������ ��� ���������� ���������� �������
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
        return -1;
    }

    // ���������� ������������ �������
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // �������� ������� ������ ��� ���������� ������������ �������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
        return -1;
    }

    // �������� ��������� ��������� � ������������� ��������
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �������� ������� ������ ��� �������� ���������
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        return -1;
    }

    // �������� ��������, ��� ��� ��� ��� � ���������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ����������� ������ ������������
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // ����� ������ �����
         0.5f, -0.5f, 0.0f, // ������ ������ �����
         0.0f,  0.5f, 0.0f  // ������� �����
    };

    // �������� ������ ������ � ������� ��������� ������
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // �������� ������ ������ � ������� ��������� ������
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // �������� ��������� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ������� ������ ������ � ������� ��������� ������
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // �������� ���� ����������
    while (window.isOpen()) {
        // ��������� ������� SFML
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ������� ������
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ��������� ��������� ���������
        glUseProgram(shaderProgram);

        // ��������� ������������
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ������� ������ ������ � ������� ��������� ������
        glBindVertexArray(0);

        // ����������� ����� SFML
        window.display();
    }

    // ������������ ��������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}