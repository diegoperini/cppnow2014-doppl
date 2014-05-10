#ifndef _DOPPL_DATA_MEMBER
#define _DOPPL_DATA_MEMBER

#include "doppl_libs.hpp"
#include "doppl_forward_decl.hpp"

namespace doppl {

	template<typename T>
    class task_member<semantic_action_specifier::data, T> {
    private:
        T _data;
    public:
        //Get value
        const T& get() {
            return _data;
        };

        //data = data
        DM<T>& set(DM<T>& input) {
            this->_data = input._data;
            return *this;
        };

        //data = future
        DM<T>& set(FM<T>& input) {
            this->_data = input.get();
            return *this;
        };

        //data = state
        template<typename... Ts>
        DM<T>& set(SM<T, Ts...>& input, Ts&... args) {
            FM<T> _future(input, true, args...);
            this->set(_future);
            return *this;
        };

        //data = value
        DM<T>& set(T&& input) {
            this->_data = input;
            return *this;
        };

        task_member() {};

        template<typename... Ts>
        task_member(Ts&... input) {
            this->set(input...);
        };

    };

};

#endif
