#include "doppl.hpp"

using namespace doppl;

int doppl_main() {
    const int range = 1;

    //Stdin and stdout
    input_t input;
    output_t output;

    //Data oriented private task members
    std::array<
        DM<int>,
        range
    > _foo;
    std::array<
        FM<int>,
        range
    > _zee;

    //Shared task members
    shared<DM<int>> sha;

    //Task body
    auto task_body = [&] (
                //Task id
                const int tid
            ) -> int {    
        //Doppl runtime loop
        std::promise<void> global_yield;
        task_loop<void> loop;

        //Private Members
        decltype(_foo[tid])& foo = _foo[tid];
        decltype(_zee[tid])& zee = _zee[tid];
        
        //States
        SM<void> init;
        SM<void> bar;
        SM<void, FM<int>> tar;

        init.set([&] (auto& yield, auto& next, auto& finish) {
            output.set("Init State Works\n");

            //Data Members test
            foo.set(42);
            assert(foo.get() == 42);

            output.set("Data Members Work\n");
            
            //State Transition Test
            next.set(bar);
        });
          
        bar.set([&] (auto& yield, auto& next, auto& finish) {
            output.set("State Transition Works\n");

            //Internal State Declarations Test
            SM<int> temp([&output] (auto& yield, auto& next, auto& finish) { 
                SM<int> clone([&output] (auto& yield, auto& next, auto& finish) {
                    output.set("Transition in Cloned Tasks Works\n");
                    next.set(finish);
                    yield.set_value(99);
                }); 
                output.set("Internal State Declerations Work\n");

                //Transition in Cloned Tasks test
                next.set(clone);
            });

            //Future Members test
            zee.set(temp, false);
            assert(zee.get() == 99);

            output.set("Future Members Work\n");

            //Parametered Transition test
            next.set(tar, zee);
        });

        tar.set([&] (auto& yield, auto& next, auto& finish, FM<int> param) {
            assert(param.get() == 99);

            output.set("Parametered Transition Works\n");

            //Shared Members test
            sha.set(foo);
            assert(sha.get() == 42);

            output.set("Shared Members Work\n");
            
            next.set(finish);
        });

        return loop(init, global_yield);
    };

    //Return
    return doppl_run<range>(task_body);
};

//Start runtime
int main() {
    std::cout << std::endl << "********Doppl Runtime Test" << std::endl << std::endl;
    assert(doppl_main() == 0);
    std::cout << std::endl << "********Doppl Runtime Test Success" << std::endl;
    return 0;
};