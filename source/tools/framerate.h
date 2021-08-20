#pragma once
#include <vector>
#include <numeric>
#include <functional>

namespace isometric {

    class framerate {
    private:
        double current = 0.0;
        double minimum = 0.0;
        double maximum = 0.0;
        double overall_average = 0.0;

        size_t max_history = 1000;
        size_t history_cursor = 0;
        std::vector<double> history;

    public:

        /// <summary>
        /// Set the current framerate so that it can be used in calculations
        /// </summary>
        /// <param name="fps">Current frames per second</param>
        void set(double fps) {
            current = fps;

            // If the current framerate is greater or equalt to 5 and less than the minimum, set it, unless
            // the minimum is still below 5, then it should be equivalent to current until it gets higher.
            // 
            // This is to prevent the minimum framerate always being super low, as that's expected when the engine
            // is starting up or shutting down.
            if (minimum < 5.0 || (current >= 5.0 && current < minimum)) minimum = current;

            // Set the maximum framerate, unless the application is still considered starting up (that is before
            // the framerate history has had a chance to fill up).
            if (current > maximum && history.size() >= max_history) maximum = current;

            // Used to calculate the current average (not the overall average). Do this by keeping a history of past 
            // framerates.
            if (history.size() != max_history) {
                history.resize(max_history, 0.0);
            }

            history[history_cursor] = current;

            // Increment the history cursor if it's within range:
            history_cursor =
                history_cursor < max_history - 1
                ? history_cursor + 1
                : 0;

            // Keep a caluclation of the overall average framerate. Ignore any values below 5 in the calculation.
            if (overall_average <= 0.1 || current <= 5.0) overall_average = current;
            else {
                overall_average = (overall_average + current) / 2.0;
            }
        }

        /// <summary>
        /// Set the current framerate by calculating it with delta time.
        /// </summary>
        /// <param name="delta_time">The duration in seconds for a single frame</param>
        void set_from_delta(double delta_time) {
            set(calculate(delta_time));
        }

        /// <returns>The curent frames per second</returns>
        double get() const {
            return current;
        }

        /// <returns>The lowest frames per second for the duration of this framerate object</returns>
        double get_minimum() const {
            return minimum;
        }

        /// <returns>The highest frames per second for the duration of this framerate object</returns>
        double get_maximum() const {
            return maximum;
        }

        /// <returns>The average frames per second using a history of framerates for a finite amount of frames</returns>
        double get_average() const {
            if (history.size() < 2) return current;

            return std::accumulate(
                history.begin(), 
                history.end(), 
                0.0 //initial value of the sum
            ) / history.size();
        }

        /// <returns>The overall average frames per second for the duration of this framerate object</returns>
        double get_overall_average() const {
            return overall_average;
        }

        /// <summary>
        /// Utility function to calculate the framerate based on delta time
        /// </summary>
        /// <param name="delta_time">The duration in seconds for a single frame</param>
        /// <returns>The calculated frames per second</returns>
        static double calculate(double delta_time)
        {
            if (delta_time <= 0.0001) return 0.0; // Avoid calculation errors
            return 1.0 / delta_time;
        }

        operator double() const { return current; }
        operator float() const { return static_cast<float>(current); }
        operator unsigned() const { return static_cast<unsigned>(current); }
    };

}