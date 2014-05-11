#ifndef _DOPPL
#define _DOPPL

#include "doppl_libs.hpp"
#include "doppl_task_loop.hpp"
#include "doppl_data_member.hpp"
#include "doppl_future_member.hpp"
#include "doppl_state_member.hpp"
#include "doppl_stdio.hpp"
#include "doppl_shared_member.hpp"

template<int range, typename F>
inline int doppl_run(F task_body) {
	//Task group
    std::array< std::future<int> , range> task_list;

    //Task init
    for(int i = 0; i < range; i++) {
        task_list[i] = std::async(
            std::launch::async, 
            task_body,
            //Task id
            i
        );
    }

    //Task return wait
    int return_val = 0;
    for(auto& t : task_list) {
        return_val |= t.get();
    }

    return return_val;
};

#endif