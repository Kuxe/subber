# subber
[![Build Status](https://travis-ci.org/Kuxe/subber.svg?branch=master)](https://travis-ci.org/Kuxe/subber)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/kuxe/subber?svg=true)](https://ci.appveyor.com/project/Kuxe/subber)

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

It is not uncommon that a class should listen to several events. Any class can have multiple subbers (=listen to several events) as shown in the following example:

```c++
#include "subber.hpp"

struct SimpleEvent { };
struct OtherEvent { };
struct Foo : public Subber<SimpleEvent, OtherEvent> {
	void notify(const SimpleEvent& event) override { /* do something */ }
	void notify(const OtherEvent& event) override { /* do some other thing */ }
};

int main(int argc, char** argv) {
	SimpleEvent simpleEvent;
	OtherEvent otherEvent;
	Foo foo;
	publish(simpleEvent, otherEvent); //Something is done and some other thing is done
	return 0;
}
```
Note that you can publish several events at once too!

A more rigorous example can be found in [example.cpp](https://github.com/Kuxe/subber/blob/master/example.cpp). 

The complexity of a call to `publish(instanceOfSomeClassParametrizedByT)` is linear in the number of instantiated subbers parametrized by T.

subber compiles on g++ 6.1.1 using the --std=c++14 flag.

Happy coding :+1:!