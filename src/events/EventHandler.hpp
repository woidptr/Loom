#pragma once
#include <entt/entt.hpp>
#include <utility>
#include <sdk/GameContext.hpp>

#define $add_listener(event_type, function) \
    listeners.emplace_back(EventHandler::subscribe<event_type, function>(*this))

struct BaseEventContext {};

struct CancellableEventContext : public BaseEventContext {
private:
    bool cancelFlag = false;
public:
    bool isCancelled() const { return cancelFlag; }
    void cancel() { cancelFlag = true; }
};

template <typename T>
struct InjectableEventContext : public BaseEventContext {
public:
    T returnValue;
public:
    void setReturn(T value) { returnValue = value; }
};

class EventHandler {
private:
    static inline entt::dispatcher dispatcher{};
public:
    template <typename TEvent, auto Candidate>
    static entt::scoped_connection subscribe() {
        return dispatcher.sink<TEvent>().template connect<Candidate>();
    }

    template <typename TEvent, auto Candidate, typename Type>
    static entt::scoped_connection subscribe(Type& instance) {
        return dispatcher.sink<TEvent>().template connect<Candidate>(instance);
    }

    template <typename TEvent>
    static void emit(TEvent& event) {
        dispatcher.trigger(event);
    }
};