// Super Loop MockTOS
//
// Author: Matthew Knight
// FileName: super-loop.hpp
// Date: 2019-08-07

#pragma once

#include "svd-alias/vector-table.hpp"

#include <bitset>
#include <tuple>

// for passing flag and type info around
template <auto idx, typename T>
struct IndexPair {
    using Type = T;
    static auto const index = idx;
};

template <auto... indices, typename... Ts,
          template <typename...> typename Tuple>
auto make_index_pairs_impl(std::index_sequence<indices...>, Tuple<Ts...>) {
    return Tuple<IndexPair<indices, Ts>...>{};
}

template <typename... Ts>
auto make_index_pairs(std::tuple<Ts...> types) {
    return make_index_pairs_impl(std::index_sequence_for<Ts...>{}, types);
}

// A Task is simply a function that is triggered by something
template <typename TriggerType, typename Func>
struct Task {
    using Trigger = TriggerType;
    Func func;
};

template <typename Trigger, typename Func>
auto make_task(Func&& f) {
    return Task<Trigger, Func>{f};
}

// Trigger Types: Tick and Interrupt
template <auto ticks>
struct Tick {
    static auto const period = ticks;
};

template <auto interrupt, typename FlagAction, typename EnableAction>
struct Interrupt {
    static auto const irq = interrupt;

    static void enable() { EnableAction::execute(); }

    static void clear() { FlagAction::execute(); }
};

// predicates for deterimining Tick/Interrupt handling
template <typename Trigger>
struct IsTickTrigger {
    static auto const value = false;
};

template <auto period>
struct IsTickTrigger<Tick<period>> {
    static auto const value = true;
};

template <typename Trigger>
struct IsInterruptTrigger {
    static auto const value = false;
};

template <auto irq, typename FlagAction, typename EnableAction>
struct IsInterruptTrigger<Interrupt<irq, FlagAction, EnableAction>> {
    static auto const value = true;
};

// Predicates for the type checker
template <typename IndexPairType>
struct TickPredicate {
    static auto const value =
        IsTickTrigger<typename IndexPairType::Type::Trigger>::value;
};

template <typename IndexPairType>
struct InterruptPredicate {
    static auto const value =
        IsInterruptTrigger<typename IndexPairType::Type::Trigger>::value;
};

// filtering types from a pack
template <typename, typename>
struct Cons;

template <typename T, typename... Args>
struct Cons<T, std::tuple<Args...>> {
    using Type = std::tuple<T, Args...>;
};

template <typename...>
struct TickFilter;

template <>
struct TickFilter<> {
    using Type = std::tuple<>;
};

template <typename Head, typename... Tail>
struct TickFilter<Head, Tail...> {
    using Type = typename std::conditional_t<
        TickPredicate<Head>::value,
        typename Cons<Head, typename TickFilter<Tail...>::Type>::Type,
        typename TickFilter<Tail...>::Type>;
};

// actions on different fields or registers
template <typename FlagType>
struct Set {
    static void execute() { FlagType::write(1); }
};

template <typename FlagType>
struct Clear {
    static void execute() { FlagType::write(0); }
};

template <typename FlagType>
struct DoNothing {
    static void execute() {}
};

template <auto ticks, auto idx>
struct Counter {
    static auto const period = ticks;
    static auto const index = idx;
    std::uint32_t count{0};
};

template <typename Mcu, typename... Tasks>
class SuperLoop {
    inline static std::bitset<sizeof...(Tasks)> flags;

    template <typename... IndexPairs>
    struct SystemTick {
        inline static std::tuple<
            Counter<IndexPairs::Type::Trigger::period, IndexPairs::index>...>
            counters;

        template <typename CounterType>
        static void increment_check(CounterType& counter) {
            if (counter.count++ >= counter.period) {
                counter.count = 0;
                flags[counter.index] = true;
            }
        }

        template <typename Counters, auto... indices>
        static void increment_check_all(Counters& counters,
                                        std::index_sequence<indices...>) {
            (increment_check(std::get<indices>(counters)), ...);
        }

        static auto handler() {
            return []() {
                increment_check_all(
                    counters, std::make_index_sequence<
                                  std::tuple_size_v<decltype(counters)>>());
            };
        }
    };

    template <typename... IndexPairs, template <typename...> typename Tuple>
    auto make_system_tick_handler_impl(Tuple<IndexPairs...>&&) {
        return SystemTick<IndexPairs...>::handler();
    }

    template <typename... IndexPairs, template <typename...> typename Tuple>
    auto make_system_tick_handler(Tuple<IndexPairs...>&&) {
        return make_system_tick_handler_impl(
            typename TickFilter<IndexPairs...>::Type{});
    }

    std::tuple<Tasks...> tasks;
    Svd::VectorTable<Mcu> vectorTable;

  public:
    SuperLoop(Tasks&&... ts)
        : tasks(ts...)
        // vector table initialization needs to have
        , vectorTable(std::make_pair(
              Mcu::Interrupts::RCC,
              make_system_tick_handler(make_index_pairs(tasks)))) {}

    template <auto n>
    void check_flag() {
        if (flags[n]) {
            flags[n] = false;
            std::get<n>(tasks).func();
        }
    }

    template <auto... indices>
    void check_flags(std::index_sequence<indices...>) {
        (check_flag<indices>(), ...);
    }

    void iterate() { check_flags(std::index_sequence_for<Tasks...>{}); }

    void loop() {
        while (true) {
            iterate();
        }
    }
};

template <typename Mcu, typename... Tasks>
auto make_super_loop(Tasks&&... tasks) {
    return SuperLoop<Mcu, Tasks...>{std::forward<Tasks>(tasks)...};
}
