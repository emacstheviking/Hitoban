#pragma once

#include "htb_includes.hpp"


namespace htb
{

// forward declarations
struct cell;
struct env;
typedef std::vector<cell> cells;

namespace internal
{

// basic class for all functions
struct Function
{
    virtual ~Function() {}
};  // struct Function

// derived class with template for functions with special signature
template <typename T>
struct BasicFunction : Function
{
    std::function<T> func;

    BasicFunction(std::function<T> _func) : func(_func) {}
};  // struct BasicFunction

// generic container of listeners for any type of function
typedef std::multimap<std::type_index, std::unique_ptr<Function>> Listeners;

template <typename Func>
static void addListener(Listeners& listeners, Func& func)
{
    std::type_index index(typeid(Func));
    std::unique_ptr<Function> func_ptr(new BasicFunction<Func>(std::function<Func>(func)));
    listeners.insert(Listeners::value_type(index,std::move(func_ptr)));
}

template <typename... Args>
static void callListeners(const Listeners& listeners, Args&&... args)
{
    typedef void Func(typename std::remove_reference<Args>::type...);
    std::type_index index(typeid(Func));
    Listeners::const_iterator i = listeners.lower_bound(index);
    Listeners::const_iterator j = listeners.upper_bound(index);

    for (; i != j; ++i)
    {
        const Function& addr = *i->second;
        std::function<Func> f = static_cast<const BasicFunction<Func>&>(addr).func;
        f(std::forward<Args>(args)...);
    }
}

struct Dispatcher
{
    typedef struct Event_ {
        Listeners listeners;
        } Event;

    template<class... Args>
    void dispatch(int eventNr, Args&&... args)
    {
        callListeners(events[eventNr].listeners, std::forward<Args>(args)...);
    }

    std::map<int, Event> events;
};

struct Property
{
    Property() {}
    virtual ~Property() {}

    std::string name;
};  // struct Property

template <typename T>
struct TypedProperty : Property
{
    TypedProperty(const T& _data) :
        Property()
        , data(_data)
    {}

    T data;
};  // struct TypedProperty

typedef std::vector<std::shared_ptr<Property>> propertyListType;
typedef std::map<std::string, std::shared_ptr<Property>> propertyMapType;

}  // namespace internal

internal::propertyListType convert_cells_to_values(const cells& c);
std::shared_ptr<internal::Property> convert_cell(const cell& c);

}  // namespace htb
