# subber

[subber.hpp](https://github.com/Kuxe/subber/blob/master/subber.hpp) contains a C++14 implementation of the publisher-subscriber pattern. The purpose of subber is simplicity. This is illustrated by the following self-contained sample code:

```c++
#include "subber.hpp"

struct SimpleEvent { };

struct Foo : public Subber<SimpleEvent> {
	void notify(const SimpleEvent& event) override {
		printf("Foo received SimpleEvent\n");
	}
};

int main(int argc, char** argv) {
	SimpleEvent simpleEvent;
	Foo foo;
	publish(simpleEvent); //"Foo recieved SimpleEvent" is printed
	return 0;
}

```

A more rigorous example can be found in [example.cpp](https://github.com/Kuxe/subber/blob/master/example.cpp).

The complexity of a call to `publish(instanceOfSomeClassParametrizedByT)` is linear in the number of instantiated subbers parametrized by T.

subber compiles on g++ 6.1.1 using the --std=c++14 flag.

Happy coding :+1:!