// Super Loop MockTOS
//
// Author: Matthew Knight
// FileName: super-loop.hpp
// Date: 2019-08-07

#pragma once

#include "svd-alias/vector-table.hpp"

#include <tuple>
#include <bitset>

// filtering index sequences

template <auto> struct Value {};
template <auto ...values> struct ValueSequence {};

template <auto ...as, auto ...bs>
constexpr ValueSequence<as..., bs...> operator+(ValueSequence<as...>, ValueSequence<bs...>) {
	return {};
}

template <auto value, typename Func>
constexpr auto filter_single(Value<value>, Func predicate) {
	if constexpr (predicate(value)) {
		return ValueSequence<value>{};
	} else {
		return ValueSequence<>{};
	}
}

template <auto ...values, typename Func>
constexpr auto filter(std::index_sequence<values...>, Func predicate) {
	return (filter_single(Value<values>{}, predicate) + ...);
}

template <typename T>
struct TypeHolder {
	using Type = T;
};

template <auto index, typename T, typename ...Ts>
constexpr auto get_ith_elem() {
	static_assert(index < sizeof...(Ts) + 1);
	if constexpr (index) {
		return get_ith_elem<index - 1, Ts...>();
	} else {
		return TypeHolder<T>{};
	}
}

// end of filtering

template <typename TriggerType, typename Func>
struct Task {
	using Trigger = TriggerType;
	Func func;
};

template <typename Trigger, typename Func>
auto make_task(Func&& f) {
	return Task<Trigger, Func>{f};
}

template <typename Trigger>
struct IsTickTrigger {
	static auto const value = true;
};

struct IsInterruptTrigger;

template <auto ticks>
struct Tick {
	static auto const period = ticks;
};

template <typename FlagType>
struct Set {
	static void execute() {
		FlagType::write(1);
	}
};

template <typename FlagType>
struct Clear {
	static void execute() {
		FlagType::write(0);
	}
};

template <typename FlagType>
struct DoNothing {
	static void execute() {}
};


template <auto interrupt, typename FlagAction, typename EnableAction>
struct Interrupt {
	static auto const irq = interrupt;

	static void enable() {
		EnableAction::execute();
	}

	static void clear() {
		FlagAction::execute();
	}
};

template <auto ticks, auto idx>
struct Counter {
	static auto const period = ticks;
	static auto const index = idx;
	std::uint32_t count{0};
};

template <typename Mcu, typename ...Tasks>
class SuperLoop {
	inline static std::bitset<sizeof...(Tasks)> flags;
	
	struct SystemTick {	
		template <typename CounterType>
		static void increment_check(CounterType& counter) {
			counter.count++;
			if (counter.count >= counter.period) {
				counter.count = 0;
				flags[counter.index] = true;
			}
		}

		template <typename Counters, auto ...indices>
		static void increment_check_all(Counters& counters, std::index_sequence<indices...>) {
			(increment_check(std::get<indices>(counters)), ...);
		}
	
		template <auto ...indices>
		static auto handler(ValueSequence<indices...>) {
			static std::tuple<Counter<decltype(get_ith_elem<indices, Tasks...>())::Type::Trigger::period, indices>...> counters;
			
			return []() {
				increment_check_all(counters, std::make_index_sequence<std::tuple_size_v<decltype(counters)>>());
			};
		}
	};
	
	std::tuple<Tasks...> tasks;
	Svd::VectorTable<Mcu> vectorTable;

public:
	SuperLoop(Tasks&& ...ts) 
		: tasks(ts...) 
		  // vector table initialization needs to have 
		, vectorTable(
			std::make_pair(Mcu::Interrupts::RCC, SystemTick::handler(filter(std::index_sequence_for<Tasks...>(), [](int const i){
				return true;
			})))
		)
	{}

	template <auto n>
	void check_flag() {
		if (flags[n]) {
			flags[n] = false;
			std::get<n>(tasks).func();
		}
	}

	template <auto ...indices>
	void check_flags(std::index_sequence<indices...>) {
		(check_flag<indices>(), ...);
	}

	void iterate() {
		check_flags(std::index_sequence_for<Tasks...>{});
	}

	void loop() {
		while (true) {
			iterate();
		}
	}
};

template <typename Mcu, typename ...Tasks>
auto make_super_loop(Tasks&& ...tasks) {
	return SuperLoop<Mcu, Tasks...>{std::forward<Tasks>(tasks)...};
}
