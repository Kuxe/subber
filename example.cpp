#include "subber.hpp"

/** @brief example.cpp illustrates how to use subber.hpp
	@author Joakim "Kuxe" Thorén
	@date June 2016
 **/
	
/** QuitEvent, CircleEvent and CollatzEvent are example events.
	SimpleEvent is a simple message without data or functions.
	CircleEvent is an event containing a representation of a circle.
	CollatzEvent is an event with a member function collatz.
**/

struct SimpleEvent { };

struct CircleEvent {
	const float x, y, r;
};

struct CollatzEvent {
	constexpr int collatz(int num) const {
		return 	num == 1 ? 0 : 
				num % 2 ? collatz(3*num+1) + 1 :
				collatz(num/2) + 1;
	}
};

/** Foo will be notified by any publication of a SimpleEvent or a CircleEvent **/
struct Foo : public Subber<SimpleEvent>, Subber<CircleEvent> {
	void notify(const SimpleEvent& event) override {
		printf("Foo received SimpleEvent\n");
	}

	void notify(const CircleEvent& event) override {
		printf("Foo received CircleEvent, whose area is: %fmm²\n", 3.14f*event.r*event.r);
	}
};

/** Bar will be notified by any publication of a SimpleEvent or a CollatzEvent **/
struct Bar : public Subber<SimpleEvent>, Subber<CollatzEvent> {
	void notify(const SimpleEvent& event) override {
		printf("Bar received SimpleEvent\n");
	}

	void notify(const CollatzEvent& event) override {
		printf("Bar received CollatzEvent, collatz(6) = %i\n", event.collatz(6));
	}
};

int main(int argc, char** argv) {

	SimpleEvent simpleEvent;
	CircleEvent circleEvent = {1.2f, 3.7f, 1.3f};
	CollatzEvent collatzEvent;

	//No subber are instantiated so we expect nothing to happen
	publish(simpleEvent);
	publish(circleEvent);

	//Instantiate Foo, publish simpleEvent and collatzEvent,
	//expect "Foo received simpleEvent" printout
	//but no collatz printout since Foo doesn't inherit from Subber<CollatzEvent>
	{
		Foo foo;
		publish(simpleEvent);
		publish(collatzEvent);

		//Instantiate Bar, all printouts should occur
		{
			Bar bar;
			publish(simpleEvent);
			publish(circleEvent);
			publish(collatzEvent);
		}
		//Bar out of scope so collatzEvent should yield no results once again
		publish(collatzEvent);
	}

	//No subber is instantiated so expect nothing
	publish(simpleEvent);
	publish(circleEvent);
	publish(collatzEvent);

	//In total Foo should print SimpleEvent twice and circleEvent once
	//and Bar should print SimpleEvent once and collatz once.
	return 0;
}

