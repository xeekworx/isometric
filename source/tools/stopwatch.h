#pragma once
#include <SDL.h>

namespace isometric::tools {

    class stopwatch {
    private:

        Uint64 start_tick = 0;  // Obtained using SDL_GetPerformanceCounter
        Uint64 end_tick = 0;    // Obtained using SDL_GetPerformanceCounter
        Uint64 elapsed = 0;     // In hardware specific scale, convert with / SDL_GetPerformanceFrequency

    public:

        /// <summary>
        /// Start the stopwatch, but doesn't reset the elapsed time unless specified
        /// </summary>
        /// <param name="reset">true to also reset the stopwatch, equivalent to using restart()</param>
        void start(bool reset = false) {
            if (reset) {
                this->reset();
            }

            if (start_tick == 0) {
                start_tick = get_tick();
            }

            end_tick = 0;
        }

        /// <summary>
        /// Equivalent to using start(true). Reset and start the stopwatch.
        /// </summary>
        void restart() { start(true); }

        /// <returns>true if the stopwatch is running</returns>
        bool is_started() const {
            return
                start_tick > 0 &&   // Running if start_tick is initialized (gt-zero)
                end_tick == 0;      // and end_tick hasn't been set yet by stop()
        }

        /// <summary>
        /// Stop the stopwatch and calculating the elapsed time. Use get_elapsed() to get the calculated result.
        /// </summary>
        void stop() {
            if (!is_started()) return;

            end_tick = get_tick();
            elapsed = end_tick - start_tick;
        }

        /// <summary>
        /// Reset the stopwatch, which is equivalent to stop and reset.
        /// </summary>
        void reset() {
            start_tick = 0;
            end_tick = 0;
            elapsed = 0;
        }

        /// <summary>
        /// Gets the elapsed value if the stopwatch has been stopped. This will return zero if the stopwatch is still
        /// running. This value is system specific and must be converted using get_frequency()
        /// </summary>
        /// <returns>Elapsed ticks since the stopwatch started after it has been stopped</returns>
        Uint64 get_elapsed() const {
            return elapsed;
        }

        /// <summary>
        /// Gets the elapsed value if the stopwatch has been stopped. This will return zero if the stopwatch is still
        /// running.
        /// </summary>
        /// <returns>Elapsed seconds</returns>
        double get_elapsed_sec() const {
            return static_cast<double>(elapsed) / get_frequency();
        }

        /// <summary>
        /// Gets the elapsed value if the stopwatch has been stopped. This will return zero if the stopwatch is still
        /// running.
        /// </summary>
        /// <returns>Elapsed milliseconds</returns>
        double get_elapsed_ms() const {
            return get_elapsed_sec() * 1000.0;
        }

        /// <returns>
        /// System specific tick count from the quried performance counter. Divide by get_frequency() to
        /// convert this to seconds.
        /// </returns>
        static Uint64 get_tick() {
            return SDL_GetPerformanceCounter();
        }

        /// <returns>
        /// System specific tick count conversion value. Divide get_tick() by this to convert this to seconds.
        /// </returns>
        static Uint64 get_frequency() {
            return SDL_GetPerformanceFrequency();
        }

    };

}