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

///////////////////////////////////////////////////// in order to count how many args take a std::function

template <typename Signature>
struct count_args;

template <typename Ret, typename... Args>
struct count_args<std::function<Ret(Args...)>>
{
    static constexpr size_t value = sizeof...(Args);
};  // struct count_args

///////////////////////////////////////////////////// stuff to store any type of data

struct AbsObj  // AbstractObject Interface
{
    AbsObj() {}
    virtual ~AbsObj() {}
};  // struct AbsObj

template <typename T>
struct TypedObj : AbsObj
{
    TypedObj(const T& _data) :
        AbsObj()
        , data(_data)
    {}

    T data;
};  // struct TypedObj

typedef std::vector<std::shared_ptr<AbsObj>> absObjList;
typedef std::map<std::string, std::shared_ptr<AbsObj>> absObjMap;

///////////////////////////////////////////////////// stuff to store functions

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
    int a = std::distance(listeners.begin(), i);
    int b = std::distance(listeners.begin(), j);

    if (a == b && b == 0)
    {
        const Function& addr = *i->second;
        std::function<Func> f = static_cast<const BasicFunction<Func>&>(addr).func;
        f(std::forward<Args>(args)...);
    }
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

    void dispatch(int eventNr, const absObjList& parsed_args)
    {
        callListeners(events[eventNr].listeners, parsed_args);
    }

    std::map<int, Event> events;
};

}  // namespace internal

///////////////////////////////////////////////////// to convert Hitoban values to C++ ones

internal::absObjList convert_cells_to_values(const cells& c);
std::shared_ptr<internal::AbsObj> convert_cell(const cell& c);

}  // namespace htb
