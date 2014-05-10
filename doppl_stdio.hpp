#ifndef _DOPPL_STDIO
#define _DOPPL_STDIO

#include "doppl_libs.hpp"

namespace doppl {

	class output_t {
    private:
        std::mutex m;

    public:
        void set(const std::string& value) {
            std::lock_guard<std::mutex> lock(m);
            std::cout << value;
        }
    };

    class input_t {
    private:
        std::mutex m;

    public:
        const std::string get() {
            std::lock_guard<std::mutex> lock(m);
            std::string value;
            std::cin >> value;
            return value;
        }
    };
};

#endif