namespace alg::optimize {
    // -----------------------------------
    // vvv ------AnnealingMethod------ vvv


    // Interface for annealing method
    // ResultType - standard numeric type
    template <typename ResultType>
    class AnnealingMethod {
        bool print_debug_messages_ = false;
        uint64_t debug_messages_interval_ = 1e6;

    public:
        func::Random generator;

        explicit AnnealingMethod(uint64_t seed) : generator(seed) {
        }

        void print_debug_messages(uint64_t debug_messages_interval = 1e6) {
            print_debug_messages_ = true;
            debug_messages_interval_ = debug_messages_interval;
        }

        void run_optimization(uint64_t number_iterations = 1e9, long double temperature_delta = 0.99, long double temperature = 1.0) {
            ResultType current_answer = target_function();
            for (uint64_t iteration = 0; iteration < number_iterations && continue_optimization(current_answer); ++iteration) {
                if (print_debug_messages_ && (iteration % debug_messages_interval_) == 0) {
                    std::cout << "Iteration: " << iteration << " | current answer: " << current_answer << "\n";
                }

                do_change();
                ResultType temporary_answer = target_function();
                if (temporary_answer > current_answer || generator.try_event(exp((temporary_answer - current_answer) / temperature))) {
                    current_answer = temporary_answer;
                }
                else {
                    rollback_change();
                }

                temperature *= temperature_delta;
            }
        }

        // Checks for stop optimization before passing all iterations
        virtual bool continue_optimization(const ResultType& current_answer) const {
            return true;
        }

        // Function for optimization
        virtual ResultType target_function() = 0;

        // Apply little change to current state
        virtual void do_change() = 0;

        // Rollback last change
        virtual void rollback_change() = 0;
    };


    // ^^^ ------AnnealingMethod------ ^^^
    // -----------------------------------
}   // AnnealingMethod | Version: 0.0

using namespace alg::optimize;