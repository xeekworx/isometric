#pragma once
#include <list>
#include <numeric>
#include <functional>

namespace isometric {

    class framerate {
    private:
        double current = 0.0;
        double minimum = 0.0;
        double maximum = 0.0;
        size_t max_history = 1000;
        std::list<double> history;

    public:
        void set(double fps) {
            current = fps;

            if (minimum <= 0.1) minimum = current;
            else if (current < minimum && current > 1.0) minimum = current;

            if (current > maximum && history.size() >= max_history) maximum = current;

            history.push_back(fps);
            if (history.size() > max_history) {
                history.erase(history.begin());
            }
        }

        void set_from_delta(double delta_time) {
            set(calculate(delta_time));
        }

        double get() const {
            return current;
        }

        double get_minimum() const {
            return minimum;
        }

        double get_maximum() const {
            return maximum;
        }

        double get_average() const {
            if (history.size() < 2) return current;

            return std::accumulate(
                history.begin(), 
                history.end(), 
                0.0 //initial value of the sum
            ) / history.size();
        }

        operator double() { return current; }
        operator unsigned() { return static_cast<unsigned>(current); }

        static double calculate(double delta_time)
        {
            if (delta_time <= 0.0001) return 0.0; // Avoid calculation errors
            return 1.0 / delta_time;
        }
    };

}