#ifndef SUBBER_HPP
#define SUBBER_HPP

#include <unordered_set>

/** @brief Subber is a class which subscribes to events
	Subber (abv. Subscriber) is an abstract class which requires anyconcrete subclass to implement a notify-method.
	The notify-method is called whenever someone makes a call to the global publish-method
	if the type of the parametrization and type of argument matches.
	@author Joakim "Kuxe" Thorén
	@date June 2016
 **/

template<class T>
class SubberHelper {
	static std::unordered_set<SubberHelper<T>*> subbers;
protected:
	constexpr SubberHelper() { subbers.insert(this); }
	virtual ~SubberHelper() { subbers.erase(this); }
public:
	virtual void notify(const T& e) = 0;
	friend void publish(const auto& e);
	class Subber;
	friend class Subber;
};
template<typename T> std::unordered_set<SubberHelper<T>*> SubberHelper<T>::subbers;
template<class... Ts> struct Subber : SubberHelper<Ts>... { };

static void publish(const auto& e) {
	for(const auto& subber : SubberHelper<typename std::remove_const<typename std::remove_reference<decltype(e)>::type>::type>::subbers) {
		subber->notify(e);
	}
}

#endif //SUBSCRIBER_HPP