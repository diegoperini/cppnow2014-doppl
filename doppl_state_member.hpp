#ifndef _DOPPL_STATE_MEMBER
#define _DOPPL_STATE_MEMBER

#include "doppl_libs.hpp"
#include "doppl_forward_decl.hpp"

namespace doppl {

    template<typename T, typename... Ts>
    class task_member<semantic_action_specifier::state, T, Ts...> {
    private:
        std::function<void(std::promise<T>&, SM<T>&, SM<T>&, Ts...)> _state;
    public:
        //Get state
        auto get() -> const decltype(_state) {
            return _state;
        };

        //state = state
        SM<T, Ts...>& set(SM<T, Ts...>& input) {
            this->_state = input._state;
            return *this;
        };

        //state = value
        SM<T, Ts...>& set(decltype(_state) input) {
            _state = input;
            return *this;
        };

        task_member() {};

        template<typename... As>
        task_member(As... input) {
            this->set(input...);
        };

    };

    template<typename T>
    class task_member<semantic_action_specifier::state, T> {
    private:
        std::function<void(std::promise<T>&, SM<T>&, SM<T>&)> _state;
    public:
        //Get state
        auto get() -> const decltype(_state) {
            return _state;
        };

        //state = state
        template<typename... Ts, typename... As>
        SM<T>& set(SM<T, Ts...>& input, As&... args) {
            this->_state = [&] (std::promise<T>& p, SM<T>& n, SM<T>& f) {
                input.get()(p, n, f, args...);   
            };
            return *this;
        };

        SM<T>& set(SM<T>& input) {
            this->_state = input._state;
            return *this;
        };        

        SM<T>& set(decltype(_state) input) {
            _state = input;
            return *this;
        };
  
        task_member() {};

        template<typename... Ts>
        task_member(Ts... input) {
            this->set(input...);
        };

    };

};

#endif