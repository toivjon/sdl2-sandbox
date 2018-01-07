#include <SDL.h>

static SDL_atomic_t sAtomicInt;

// ============================================================================
// TIMERS
// ============================================================================
// An example SDL timer callback function.
//
// This function is as a callback function that is called by the SDL framework
// timer functionality on periodic intervals after being registered with the
// SDL_AddTimer function.
//
// Function return value specifies when SDL should call the function for the
// next time. If the function returns 0, then the timer is being cancelled.
// ============================================================================
static Uint32 timer_callback(Uint32 interval, void* param)
{
    SDL_Log("\tSDL called the timer callback function!");
    return interval;
}

// ============================================================================
// THREADS
// ============================================================================
// An example SDL thread function.
// 
// This function is an example of a SDL thread function. Functions of this kind
// will be called when they are used along the SDL_CreateThread function.
//
// Function return value will be passed to issuer via the SDL_WaitThread.
// ============================================================================
static int thread_function(void* data)
{
    SDL_Delay(*static_cast<int*>(data));
    SDL_Log("\tSDL called a thread function on thread %d!", SDL_ThreadID());

    SDL_AtomicAdd(&sAtomicInt, 1);
    return 0;
}

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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
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
    SDL_assert(true == true);
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
    // TIMERS
    // ========================================================================
    // SDL contains a support the following timer features.
    //
    // Timer.................Add/remove timer called on a specified interval.
    // Delay.................Make the current thread to wait for some time.
    // Performance Counter...A high resolution timer value and frequency.
    // Ticks.................The number of millis since SDL init.
    // ========================================================================
    SDL_Log("Testing SDL timer features:\n");
    SDL_Log("\tPerformance counter frequency: %u\n", SDL_GetPerformanceFrequency());
    SDL_Log("\tPerforming a small SDL one second delay and using timers.\n");
    SDL_Log("\t%04u --- %u\n", SDL_GetTicks(), SDL_GetPerformanceCounter());
    SDL_Delay(1000);
    SDL_Log("\t%04u --- %u\n", SDL_GetTicks(), SDL_GetPerformanceCounter());

    SDL_Log("\tChecking how the SDL periodic timer works.\n");
    SDL_Log("\tCreating a timer with 500 millisecond interval.\n");
    auto timerId = SDL_AddTimer(500, timer_callback, nullptr);
    if (timerId == 0) {
        SDL_Log("\tUnable to create SDL timer: %s\n", SDL_GetError());
    }
    SDL_Delay(1010);
    if (SDL_RemoveTimer(timerId) == SDL_FALSE) {
        SDL_Log("\tSDL was unable to find a timer with id: %d\n", timerId);
    }
    SDL_Log("\tRemoved the timer.\n");

    // ========================================================================
    // THREADS
    // ========================================================================
    // SDL contains the following inbuilt support for multithreading.
    //
    // 1. Threads
    // 2. Synchronization primitives
    // 3. Atomic operations
    //
    // Thread management contains the following functionality.
    // 
    // 1. Thread creation.
    // 2. Thread waiting.
    // 3. Thread detaching.
    // 4. Thread-local storage.
    // 5. Thread priorities (LOW, NORMAL[default] and HIGH).
    // 
    // Synchronization primitives contain the following structures.
    //
    // 1. Condition variables
    // 2. Mutexes
    // 3. Semaphores
    //
    // Atomic operations support the following 
    // 
    // !!! IMPORTANT NOTE !!!
    // Note that window creation, rendering or event receiving cannot be done
    // in any other thread than within the main thread of the application.
    // ========================================================================
    SDL_Log("Testing SDL threading features:\n");
    const auto cpus = SDL_GetCPUCount();
    SDL_Log("\tDetected %d logical CPU cores on the client computer.\n", cpus);
    auto threadDelay = 2000;
    auto thread1 = SDL_CreateThread(thread_function, "foo-1", &threadDelay);
    auto thread2 = SDL_CreateThread(thread_function, "foo-2", &threadDelay);
    auto thread3 = SDL_CreateThread(thread_function, "foo-3", &threadDelay);

    auto threadReturn = 0;
    SDL_DetachThread(thread1);
    SDL_WaitThread(thread2, &threadReturn);
    SDL_WaitThread(thread3, &threadReturn);
    SDL_Log("\tAll threads have processed their work.\n");
    SDL_Log("\tAtomic integer is now to %d.\n", SDL_AtomicGet(&sAtomicInt));

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