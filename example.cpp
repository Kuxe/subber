/** @brief example.cpp illustrates how to use subber.hpp
	@author Joakim Thorén
	@date December 2016
 **/

#include "subber.hpp"
using namespace subber;

/** QuitEvent, CircleEvent and CollatzEvent are example events.
	SimpleEvent is a simple event without data or functions.
	CircleEvent is an event containing a representation of a circle.
	CollatzEvent is an event with a member function collatz.
**/

struct SimpleEvent { };
struct CircleEvent { const float x, y, r; };
struct CollatzEvent {
	constexpr int collatz(int n) const {
		return n == 1 ? 0 : n % 2 ? collatz(3*n+1) + 1 : collatz(n/2) + 1;
	}
};

/** Foo will be notified by any publication of a SimpleEvent or a CircleEvent **/
struct Foo : public Subber<SimpleEvent, CircleEvent> {
	void notified(const SimpleEvent& event) override {
		printf("Foo received SimpleEvent\n");
	}

	void notified(const CircleEvent& event) override {
		printf("Foo received CircleEvent, whose radius is: %fmm\n", event.r);
	}
};

/** Bar will be notified by any publication of a SimpleEvent or a CollatzEvent **/
struct Bar : public Subber<SimpleEvent, CollatzEvent> {
	void notified(const SimpleEvent& event) override {
		printf("Bar received SimpleEvent\n");
	}

	void notified(const CollatzEvent& event) override {
		printf("Bar received CollatzEvent, collatz(6) = %i\n", event.collatz(6));
	}
};

//It is possible to check compile-time if a class is subscribed to some event
static_assert(is_subbed_to<Foo, SimpleEvent>::value, "Passes since Foo is subbed to SimpleEvent");
static_assert(is_subbed_to<Foo, CircleEvent>::value, "Passes since Foo is subbed to CircleEvent");
static_assert(!is_subbed_to<Foo, CollatzEvent>::value, "Passes since Foo is not subbed to CollatzEvent");
static_assert(is_subbed_to<Bar, SimpleEvent>::value, "Passes since Bar is subbed to SimpleEvent");
static_assert(!is_subbed_to<Bar, CircleEvent>::value, "Passes since Bar is not subbed to CircleEvent");
static_assert(is_subbed_to<Bar, CollatzEvent>::value, "Passes since Bar is subbed to CollatzEvent");

int main(int argc, char** argv) {

	SimpleEvent simpleEvent;
	CircleEvent circleEvent = {1.3f, 3.7f, 1.0f};
	CollatzEvent collatzEvent;

	//Neither Foo or Bar is instantiated so nothing is printed
	publish(simpleEvent, circleEvent); 
	{
		Foo foo;
		//"Foo received simpleEvent" is printed
		//but no collatz printout since Foo doesn't inherit from Subber<CollatzEvent>
		publish(simpleEvent, collatzEvent);

		//Instantiate Bar, all printouts should occur
		{
			Bar bar;
			publish(simpleEvent, circleEvent, collatzEvent);
		}
		//Bar out of scope so collatzEvent should yield no results once again
		publish(collatzEvent);
	}

	//No subber is instantiated so expect nothing
	publish(simpleEvent, circleEvent, collatzEvent);

	/* Expected output:
		Foo received SimpleEvent
		Bar received SimpleEvent
		Foo received SimpleEvent
		Foo received Circle event, whose radius is 1.0
		Bar received CollatzEvent, collatz(6) = 8
	*/
	return 0;
}
