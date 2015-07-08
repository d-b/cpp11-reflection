/*
 * >= C++11 Reflection
 *
 * - Daniel Bloemendal
 */

#pragma once

struct for_each_impl {
    template<std::size_t I = 0, typename Function, typename... Items>
    inline typename std::enable_if<I == sizeof...(Items), void>::type
    static for_each(std::tuple<Items...>& tuple, Function& fn) {}

    template<std::size_t I = 0, typename Function, typename... Items>
    inline typename std::enable_if<I < sizeof...(Items), void>::type
    static for_each(std::tuple<Items...>& tuple, Function& fn) {
        fn(std::get<I>(tuple));
        for_each<I + 1, Function, Items...>(tuple, fn);
    }
};

template<typename Function, typename... Items>
void for_each(std::tuple<Items...>& tuple, Function& fn) {
    for_each_impl::for_each(tuple, fn);
}

template<typename Function, typename... Items>
void for_each(std::tuple<Items...>&& tuple, Function& fn) {
    for_each_impl::for_each(tuple, fn);
}

template<typename Klass, typename Type>
struct field {
    // Type definitions
    enum { flags_constant = 1, flags_private = 2 };
    typedef Type (Klass::*pointer_type);

    // Field information
    std::string  name;
    pointer_type pointer;
    int          flags;
    std::string  caption;
    std::string  doc;
};

template<typename Klass, typename Type>
field<Klass, Type> make_field(std::string name, Type (Klass::*pointer), int flags = 0, std::string caption = "", std::string doc = "") {
    return field<Klass, Type>{std::move(name), pointer, flags, std::move(caption), std::move(doc)};
}

template<typename Klass>
struct meta {
    std::tuple<> fields() { return std::tuple<>(); }
};

#define REFLECT_REGISTER(cls, ...) \
    template<> \
    struct meta<cls> { \
        typedef cls klass; \
        static auto fields() -> decltype(std::make_tuple(__VA_ARGS__)) { return std::make_tuple(__VA_ARGS__); } \
    };

#define REFLECT_FIELD(name, caption) \
    make_field(#name, &klass::name, 0, caption, "")
