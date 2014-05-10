#include "doppl_members.hpp"

namespace doppl {

    template<semantic_action_specifier S, typename T, typename... Ts>
    class task_member;

    template<typename T>
    using DM = task_member<semantic_action_specifier::data, T>;
    template<typename T>
    using FM = task_member<semantic_action_specifier::future, T>;
    template<typename T, typename... Ts>
    using SM = task_member<semantic_action_specifier::state, T, Ts...>;

    template<typename T, typename... Ts>
    class task_loop;

    class output_t;
    class input_t;

    template<typename T>
    class shared;
    
};
