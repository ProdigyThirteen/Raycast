#include "Game.h"
#include <iostream> // std::cout, std::endl
#include <SDL2/SDL.h> // SDL_Init, SDL_Quit
#include "../Rendering/GameWindow.h"
#include "../Rendering/RenderInstanceManager.h"
#include "InputManager.h"
#include "../Level/LevelManager.h"
#include "../Level/LevelFactory.h"
#include "../Globals/DeltaTime.h"

Game::Game(token)
{
    std::cout << "Game instance created" << std::endl;
}

Game::~Game()
{
    std::cout << "Game instance destroyed" << std::endl;
}

bool Game::Init()
{
    bool ret = 0;

    ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret != 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    ret = GameWindow::instance().Init();
    if (!ret)
    {
        std::cout << "GameWindow::Init failed" << std::endl;
        return false;
    }

    ret = RenderInstanceManager::instance().AddRaycastRenderer("main");
    if (!ret)
    {
        std::cout << "GameRenderer::Init failed" << std::endl;
        return false;
    }


    ret = InputManager::instance().Init();
    if (!ret)
    {
        std::cout << "InputManager::Init failed" << std::endl;
        return false;
    }

    LevelManager::instance().LoadLevel(LevelFactory::instance().CreateLevel());
    RenderInstanceManager::instance().GetRenderer("main")->Init();

    _running = true;
    return true;
}

void Game::Update()
{
    DeltaTime::UpdateDeltaTime();

    // SDL Event handling loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                _running = false;
                break;

            default:
                break;
        }
    }

    // Updates input state and performs any bound callbacks
    InputManager::instance().Update();

    RenderInstanceManager::instance().GetRenderer("main")->DrawFrame();
    RenderInstanceManager::instance().GetRenderer("main")->Present();

    std::cout << "Delta time: " << DeltaTime::GetDeltaTime() << std::endl;
}