#ifndef _DOPPL_FUTURE_MEMBER
#define _DOPPL_FUTURE_MEMBER

#include "doppl_libs.hpp"
#include "doppl_forward_decl.hpp"
#include "doppl_state_member.hpp"
#include "doppl_task_loop.hpp"

namespace doppl {

    template<typename T>
    class task_member<semantic_action_specifier::future, T> {
    private:
        //Internal future and promise
        std::shared_future<T> _future;
        std::promise<T> _promise;

    public:
        //Get value and sync
        const T& get() {
            return _future.get();
        };

        //future = data
        FM<T>& set(DM<T>& input) {
            std::promise<T> p;
            _future = p.get_future();
            p.set_value(input.get());
            return *this;
        };

        //future = future
        FM<T>& set(FM<T>& input) {
            this->_future = input._future;
            return *this;
        };

        //future = state
        template<typename... Ts>
        FM<T>& set(SM<T, Ts...>& input, const bool is_lazy, Ts&... args) {
            _promise = std::promise<T>();
            _future = _promise.get_future();
            auto f = std::async(
                is_lazy ? std::launch::deferred : std::launch::async,
                [&] () {
                    task_loop<T, Ts...>()(input, _promise, args...);
                }
            );
            if(is_lazy) f.get();
            return *this;
        };

        //future = value
        FM<T>& set(T&& input) {
            std::promise<T> p;
            _future = p.get_future();
            p.set_value(input);
            return *this;
        };

        task_member() {};

        template<typename V>
        task_member(V& input) {
            this->set(input);
        };

        template<typename V, typename... Ts>
        task_member(V& input, const bool is_lazy, Ts&... args) {
            this->set(input, is_lazy, args...);
        };

    };

};

#endif