#include "rpc/client.h"

#include <iostream>
#include <tuple>
#include <string>

typedef struct tagMYSTRCT
{
    int v = 20;
    std::string str = "default";

    MSGPACK_DEFINE_ARRAY(v, str)
}MYSTRCT;

void test_struct() {
    std::cout << "Test:" << __FUNCTION__ << std::endl;
    MYSTRCT myStct;
    RPCLIB_MSGPACK::sbuffer buf;
    RPCLIB_MSGPACK::pack(buf, myStct);

    auto r = RPCLIB_MSGPACK::unpack(buf.data(), buf.size());
    auto newmyStct = r.as<MYSTRCT>();

    std::cout << "myStct.str" << myStct.str << std::endl;
    std::cout << "myStct.v" << myStct.v << std::endl;
}

void test_tuple() {
    std::cout << "Test:" << __FUNCTION__ << std::endl;
    std::string text = "abc";
    auto mytuple = std::make_tuple(text, text);
    RPCLIB_MSGPACK::sbuffer buf;
    RPCLIB_MSGPACK::pack(buf, mytuple);
    auto r = RPCLIB_MSGPACK::unpack(buf.data(), buf.size());
    auto newtuple = r.as<std::tuple<std::string, std::string>>();

    std::cout << "unpacke1: " << std::get<0>(newtuple) << "; " << std::get<1>(newtuple) << std::endl;
}

#include <any>
void test_std_any() {
    std::cout << "Test:" << __FUNCTION__ << std::endl;
    std::string text = "abc";
    // any type
    std::any a = 1;
    std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
    a = 3.14;
    std::cout << a.type().name() << ": " << std::any_cast<double>(a) << '\n';
    a = true;
    std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << '\n';

    // RPCLIB_MSGPACK::sbuffer buf;
    // RPCLIB_MSGPACK::pack(buf, a);
    // auto r = RPCLIB_MSGPACK::unpack(buf.data(), buf.size());
    // auto new_a = r.as<std::any>();

    // std::cout << "unpacke1:  new_a = " << std::any_cast<bool>(new_a) << std::endl;
}

int main() {
    // test_struct(); return 0;
    // test_tuple(); return 0;
    test_std_any(); return 0;

    rpc::client c("localhost", 123);

    std::string text;
    while (std::getline(std::cin, text)) {
        if (!text.empty()) {
            // std::string result(c.call("echo", text).as<std::string>());
            // std::cout << "> " <<  result << std::endl;

            auto mytuple = std::make_tuple(text, text);
            RPCLIB_MSGPACK::sbuffer buf;
            RPCLIB_MSGPACK::pack(buf, mytuple);
            auto r = RPCLIB_MSGPACK::unpack(buf.data(), buf.size());
            auto newtuple = r.as<std::tuple<std::string, std::string>>();

            std::cout << "unpacke1: " << std::get<0>(newtuple) << "; " << std::get<1>(newtuple) << std::endl;

            std::string result2(c.call("echo", std::string(buf.data())).as<std::string>());
            std::cout << "> " <<  result2 << std::endl;
        }
    }
}
