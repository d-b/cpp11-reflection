/*
 * >= C++11 Reflection
 *
 * - Daniel Bloemendal
 */

#include <tuple>
#include <type_traits>
#include <string>
#include <iostream>

#include "reflection.hpp"

template<typename Klass>
class example_serializer {
    Klass& _instance;

public:
    example_serializer(Klass& instance) : _instance(instance) {}

    template<typename T>
    void operator()(const field<Klass, T>& field) {
        std::cout << field.name << " (" << field.caption << "): " << (&_instance)->*field.pointer << std::endl;
    }

    void serialize() {
        for_each(meta<Klass>::fields(), *this);
    }
};

class example_class {
public:
    int integer_field;
    float float_field;
    std::string string_field;
};

REFLECT_REGISTER(example_class,
   REFLECT_FIELD(integer_field, "an integer field"),
   REFLECT_FIELD(float_field, "a floating point field"),
   REFLECT_FIELD(string_field, "a string field"))

int main() {
    example_class ex{1, 2.0, "hello world"};
    example_serializer<example_class> s(ex);
    s.serialize();
}
