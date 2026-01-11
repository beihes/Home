/*
 * This example code $WHAT_IT_DOES.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#include "inc/main.h"
#include "app/scene/inc/AppScene.h"
#include "engine/core/inc/Config.h"
#include "engine/core/inc/Context.h"
#include "engine/core/inc/Core.h"
#include "engine/core/inc/Time.h"
#include "engine/physics/inc/PhysicsEngine.h"
#include "engine/render/inc/Camera.h"
#include "engine/render/inc/Renderer.h"
#include "engine/render/inc/TextRenderer.h"
#include "engine/resource/inc/ResourceManager.h"
#include "engine/input/inc/InputManager.h"
#include "engine/scene/inc/Scene.h"
#include "engine/scene/inc/SceneManager.h"
#include "utils/log/inc/Log.h"

engine::core::Core core;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!core.Init())
    {
        return SDL_APP_FAILURE;
    }
    *appstate = &core;
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if ((event->type == SDL_EVENT_QUIT) || ((event->type == SDL_EVENT_KEY_DOWN) && event->key.key == SDLK_ESCAPE))
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    switch (event->type)
    {
        case SDL_EVENT_WINDOW_RESIZED:
            if (!SDL_GetWindowSize(core.window.get(), &core.config_->windowRect.w, &core.config_->windowRect.h))
                return SDL_APP_FAILURE;
            core.camera_->SetVeiwSize({ float(core.config_->windowRect.w), float(core.config_->windowRect.h) });
            break;
        default:
            break;
    }
    core.Input(*event);
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    if (core.time_->GetTimerState()) {
        core.time_->Update();
        core.Update();
        core.Render();
        core.time_->SetTimerState(false);
    }
    if (!core.time_->GetTimerID()) {
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    core.time_->StopTimer();
    utils::log::Info(">----应用关闭----<");
    core.Close();
    utils::log::Close_LogFile();
    SDL_Quit();
}