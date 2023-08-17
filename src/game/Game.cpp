#include "Game.h"

Game::Game(const std::string &name) : name(name){}

void InitScenes() {
    SceneManager::Init();
    SceneManager::AddScene(&Scenes::testing);
    SceneManager::AddScene(&Scenes::playground_1);

    //SceneManager::SetCurrentScene("playground_1");
}

int Game::Run() {
    // Initialize glfw
    if (!glfwInit())
        return -1;
    // Create the window
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, name.c_str());

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::LogError("Failed to initialize GLAD");
        return -1;
    }

    // Must init window after GLAD init
    window.Init();

    // Set up delta time calculation
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currFrame = 0.0f;

    InitScenes();

    // Game loop
    while (window.IsOpen())
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(255.0f/ 255.0f, 140.0f/255.0f, 251.0f/255.0f, 1.0f);

        // Calculate delta time
        currFrame = (float)glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // Update current scene
        SceneManager::Update(deltaTime);

        //Log::LogFPS(deltaTime, 100);

        // Swap front and back buffers 
        window.SwapBuffers();
        // Update current frame inputs
        InputManager::Update();
        // Poll for and process events
        glfwPollEvents();
    }

    // Free memory and end
    glfwTerminate();
    ResourceManager::FreeAll();
    return 0;
}
