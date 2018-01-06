#include <SDL.h>

int main(int argc, char* argv[])
{
    // ========================================================================
    // SDL allows configuration variables to be used as configuration hints.
    // They may or may not be supported or applicable on any given platform.
    // However, they can be used as hints to note how the SDL should behave.
    //
    // The full list of hints: https://wiki.libsdl.org/CategoryHints
    //
    // Hints can be either provided with normal or priorited way. Prioritized
    // hints will force the hint to be handled in a desired importance level.
    //
    // Note that hint state changes can also be listened with callbacks.
    // ========================================================================
    auto hintResult = SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_Log("[%d] SDL uses OpenGL\n", hintResult);

    // ========================================================================
    // SDL provides an easy 3-function interface to indicate errors.
    // Errors are also automatically added by the SDL if SDL functions fail.
    // ========================================================================
    SDL_Log("SDL error management testing:\n");
    SDL_Log("\tInitially: %s\n", SDL_GetError());
    SDL_SetError("Custom error message!");
    SDL_Log("\tAfter set: %s\n", SDL_GetError());
    SDL_ClearError();
    SDL_Log("\t  Cleared: %s\n", SDL_GetError());

    // ========================================================================
    // Initialize the SDL along with desired subsystems.
    // SDL_INIT_TIMER............Include SDL timer support.
    // SDL_INIT_AUDIO............Include SDL audio (???) support.
    // SDL_INIT_VIDEO............Include SDL video/graphics support.
    // SDL_INIT_JOYSTICK.........Include SDL joystick support.
    // SDL_INIT_HAPTIC...........Include SDL haptic (force feedback) support.
    // SDL_INIT_GAMECONTROLLER...Include SDL game controller support.
    // SDL_INIT_EVENTS...........Include SDL event system.
    // SDL_INIT_EVERYTHING.......Include all SDL subsystems.
    // 
    // Some subsystems also automatically imply other subsystems.
    // SDL_INIT_GAMECONTROLLER...Implies SDL_INIT_JOYSTICK.
    // SDL_INIT_VIDEO............Implies SDL_INIT_EVENTS.
    // SDL_INIT_JOYSTICK.........Implies SDL_INIT_EVENTS.
    //
    // Definitions can be OR'd together (i.e. SDL_INIT_TIMER | SDL_INIT_AUDIO)
    // ========================================================================
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    // ========================================================================
    // SDL offers a way to check which SDL subsystems has been initialized.
    // Uses the same macros than what are used with SDL_Init (see above).
    // 
    // Definitions can be OR'd together (i.e. SDL_INIT_TIMER | SDL_INIT_AUDIO)
    // ========================================================================
    SDL_Log("Initialized SDL subsystems:\n");
    SDL_Log("[%d] Timer\n", SDL_WasInit(SDL_INIT_TIMER) != 0);
    SDL_Log("[%d] Audio\n", SDL_WasInit(SDL_INIT_AUDIO) != 0);
    SDL_Log("[%d] Video\n", SDL_WasInit(SDL_INIT_VIDEO) != 0);
    SDL_Log("[%d] Joystick\n", SDL_WasInit(SDL_INIT_JOYSTICK) != 0);
    SDL_Log("[%d] Haptic\n", SDL_WasInit(SDL_INIT_HAPTIC) != 0);
    SDL_Log("[%d] Game controller\n", SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0);
    SDL_Log("[%d] Events\n", SDL_WasInit(SDL_INIT_EVENTS) != 0);

    // ========================================================================
    // ASSERTIONS
    // ========================================================================
    // SDL contains three (+ disabled) different levels of assertions.
    // 
    // SDL_assert_release....A release level assertion.
    // SDL_assert............A debug level assertion.
    // SDL_assert_paranoid...A trace level assertion.
    //
    // Assertion level is defined by redefining the SDL_ASSERT_LEVEL value.
    //
    // 0...Disables all assertions.
    // 1...Enables SDL_assert_release (default for release).
    // 2...Enables SDL_assert and SDL_assert_release (default for debug).
    // 3...Enables SDL_assert_paranoid, SDL_assert and SDL_assert_release.
    // 
    // Assertions are being tracked by the SDL. While assertions can be ignored
    // by continuing the program execution, we are able to get assertion report
    // from the framework. This report can be used to describe all failures.
    //
    // Note that SDL also allows setting a custom assertion handler if desired.
    // ========================================================================
    SDL_assert_release(true == true);
    SDL_assert(true == false);
    SDL_assert_paranoid(true == true);
    const auto* item = SDL_GetAssertionReport();
    while (item) {
        SDL_Log("%s %s (%s:%d) triggered %u times, always ignore: %s.\n",
                item->condition,
                item->function,
                item->filename,
                item->linenum,
                item->trigger_count,
                item->always_ignore ? "yes" : "no");
        item = item->next;
    }

    // ========================================================================
    // EVENTS
    // ========================================================================
    // SDL uses an event queue to store and distribute events. This system is
    // being initialized when the framework init contains SDL_INIT_EVENTS type.
    // (NOTE: The SDL_INIT_EVENTS can be implied by defining other subsystems.)
    // 
    // SDL events are built as unions where the structure is following:
    // --- type
    // --- union-structure that contains type specific substructure.
    //
    // The full list of types: https://wiki.libsdl.org/SDL_EventType
    //
    // Each event type is related to a single strcture within the event e.g.
    // on event SDL_KEYDOWN the details are located in the event.key structure.
    //
    // Note that SDL can be set to ignore (i.e. disable) unwanted event types.
    //
    // Events can be also filtered/handled from the queue with custom filters.
    // ========================================================================
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);

    auto running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        // ...
    }

    // ========================================================================
    // Shut down all SDL subsystems.
    SDL_Quit();
    
    return 0;
}