#ifndef SUBBER_HPP
#define SUBBER_HPP

#include <unordered_set>

/** @brief Subber is a class which subscribes to events.
	Subber (abv. Subscriber) is an abstract class that requires any concrete
	subclass to implement a notified-method. The notified-method is called
	when the global publish function is called with a matching type T.
	@author Joakim Thorén
	@date June 2016
 **/

namespace subber {

template<typename event_type>
class SubberHelper {
	static std::unordered_set<SubberHelper<event_type>*> subbers;
protected:
	constexpr SubberHelper() {
		static_assert(std::is_object<event_type>::value,
		"event_type is not an object. Do strip away qualifiers, \
		 references or pointers when inheriting from Subber!");
		subbers.emplace(this);
	}
	virtual ~SubberHelper() {
		subbers.erase(this);
	}
public:
	virtual void notified(const event_type& e) = 0;
	template<typename T, typename... Ts>
	friend void publish(const T&, const Ts&...);
};

template<typename event_type>
std::unordered_set<SubberHelper<event_type>*> SubberHelper<event_type>::subbers;
template<typename... event_types>
struct Subber : private SubberHelper<event_types>... {
	virtual ~Subber() = default;
};

template<typename subber_type, typename event_type>
struct is_subbed_to : public std::is_base_of<SubberHelper<event_type>, subber_type> { };
#if __cplusplus >= 201703L
	template<typename subber_type, typename event_type>
	inline constexpr bool is_subbed_to_v = is_subbed_to<subber_type, event_type>::value;
#endif

inline void publish() { }
template<typename event_type, typename... event_types>
void publish(const event_type& event, const event_types&... events) {
	static_assert(std::is_object<event_type>::value,
		"event_type is not an object. Do strip away qualifiers, \
		 references or pointers when calling publish!");
	using base_type = typename std::remove_cv<typename std::remove_reference<event_type>::type>::type;
	for(const auto& subber : SubberHelper<base_type>::subbers) {
		subber->notified(event);
	}
	publish(events...);
}

} //End of namespace subber

#endif //SUBSCRIBER_HPP
