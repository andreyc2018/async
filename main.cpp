#include "async.h"
#include "logger.h"
#include <iostream>
#include <string>
#include <thread>

void full_session(int size)
{
    auto h = async::connect(size);
    async::receive(h, "1", 1);
    async::receive(h, "1\n", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
}

int main(int argc, char const** argv)
{
    try {
        if (argc < 2) {
            std::cout << "usage: "
                      << std::string(argv[0]).substr(std::string(argv[0]).rfind("/")+1)
                      << " <N>\nwhere:\n  N - command block size\n";
            exit(1);
        }

        if (argc > 2) {
            gLogger->set_level(spdlog::level::debug);
        }

        std::size_t bulk = std::stoul(argv[1]);
        std::string input = "0\n\n1\n2\n3\n";
        auto h = async::connect(bulk);
        async::receive(h, input.c_str(), input.size());
        async::disconnect(h);
        auto h1 = async::connect(bulk);
        for (auto c : input) {
            async::receive(h1, &c, 1);
        }
        async::disconnect(h1);
        /*
        std::thread t1(full_session, bulk);
        std::thread t2(full_session, bulk+3);
        auto h = async::connect(bulk);
        auto h2 = async::connect(bulk);
        async::receive(h, "1", 1);
        async::receive(h, "", 1);
        async::receive(h, "2", 1);
        async::receive(h, "\n", 1);
        async::receive(h, "3", 1);
        async::receive(h, "\n", 1);
        async::receive(h, "4", 1);
        async::receive(h, "12\n3\n4", 7);

        async::receive(h2, "1\n", 2);
        async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
        async::receive(h, "b\nc\nd\n}\n89\n", 11);
        async::disconnect(h);
        async::disconnect(h2);
        t2.join();
        t1.join();
        */
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
