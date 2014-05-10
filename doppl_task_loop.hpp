#ifndef _DOPPL_TASK_LOOP
#define _DOPPL_TASK_LOOP

#include "doppl_libs.hpp"
#include "doppl_forward_decl.hpp"
#include "doppl_state_member.hpp"

namespace doppl {

    template<typename T, typename... Ts>
    class task_loop {
    private:
        SM<T> initial_state;
        SM<T> next_state;
        SM<T> finish_state;

        std::future<void> current_state_future;
    
    public:
        int operator() (SM<T, Ts...>& init, std::promise<T>& yield, Ts&... args) {
            //Set init
            initial_state.set(init, args...);
            
            //Infinite loop espace trigger
            bool is_running = true;
            
            //Set finish state
            finish_state.set([&is_running] (std::promise<T>& yield, SM<T>& next, SM<T>& finish) { is_running = false; });
            
            //Default next state assignment
            next_state.set(finish_state);
            
            //Initial state assignment (lazy)
            current_state_future = std::async(std::launch::deferred, [&] () {
                initial_state.get()(yield, next_state, finish_state);
            });

            //Program loop
            try {
                while(is_running) {
                    //Run lazily assigned state code
                    current_state_future.get();
                    //Set next state (lazy)
                    current_state_future = std::async(std::launch::deferred, [&] () {
                        next_state.get()(yield, next_state, finish_state);
                    });
                }
            } catch(std::exception& e) {
                return 1;
            }

            return 0;
        };

    };
};

#endif