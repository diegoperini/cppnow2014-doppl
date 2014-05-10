#ifndef _DOPPL_SHARED_MEMBER
#define _DOPPL_SHARED_MEMBER

#include "doppl_libs.hpp"

namespace doppl {

    template<typename T>
    class shared {
    private:
        T member;
        std::mutex m;

    public:
        //Asignment for member types
        template<typename V, typename... Vs>
        shared<T>& set(V& v, Vs&... args) {
            std::lock_guard<std::mutex> lock(m);
            member.set(v, args...);
            return *this;
        };

        //Assignment for literals
        shared<T>& set(decltype(member.get())& v) {
            std::lock_guard<std::mutex> lock(m);
            member.set(v);
            return *this;
        };

        //Forward get
        auto get() -> decltype(member.get()) {
            std::lock_guard<std::mutex> lock(m);
            return member.get();
        };
    };

};

#endif