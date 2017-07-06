#include <tuple>
#include <iostream>

template<std::size_t> struct int_{};

template <typename Functor, typename Tuple>
void tuple_visitor_impl(Functor&& functor, const Tuple& t, int_<1>) {
    functor(std::get<std::tuple_size<Tuple>::value - 1>(t));
}

template <typename Functor, typename Tuple, size_t Pos>
void tuple_visitor_impl(Functor&& functor, const Tuple& t, int_<Pos>) {
    functor(std::get<std::tuple_size<Tuple>::value - Pos>(t));
    tuple_visitor_impl(std::forward<Functor&&>(functor), t, int_<Pos - 1>());
}

template <typename Functor, typename... Args>
void tuple_visitor(Functor&& functor, const std::tuple<Args...>& t) {
    tuple_visitor_impl(std::forward<Functor&&>(functor), t, int_<sizeof...(Args)>());
}

struct F {
    template <typename T>
    void operator()(T&& t) { std::cout << "unexpect type: " << typeid(std::forward<T&&>(t)).name() << std::endl; }
    void operator()(int i) { std::cout << "void F::operator()(int): " << i << std::endl; }
    void operator()(double d) { std::cout << "void F::operator()(double): " << d << std::endl; }
    void operator()(const std::string& s) { std::cout << "void F::operator()(const std::string&): " << s << std::endl; }

};

int main() {
    auto t = std::make_tuple(10, std::string("Test"), 3.14);
    F f;
    tuple_visitor(f, t);
    return 0;
}
