#pragma once

#include <any>
#include <functional>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

class MethodResult {
public:
    MethodResult() = default;

    template <typename T>
    explicit MethodResult(T&& value)
        : _value(std::forward<T>(value)) {}

    [[nodiscard]]
    bool hasValue() const noexcept {
        return _value.has_value();
    }

    [[nodiscard]]
    const std::any& any() const noexcept {
        return _value;
    }

    template <typename T>
    T& get() {
        return std::any_cast<T&>(_value);
    }

    template <typename T>
    const T& get() const {
        return std::any_cast<const T&>(_value);
    }

    template <typename T>
    T take() {
        return std::any_cast<T>(std::move(_value));
    }

private:
    std::any _value;
};


// ============================================================
// Function traits
// ============================================================

template <typename T>
struct function_traits;


// ----------------------------
// Free function
// ----------------------------

template <typename Return, typename... Args>
struct function_traits<Return(*)(Args...)> {
    using return_type = Return;
    using args = std::tuple<Args...>;

    static constexpr std::size_t arity = sizeof...(Args);
    static constexpr bool is_member_function = false;
};


// noexcept free function

template <typename Return, typename... Args>
struct function_traits<Return(*)(Args...) noexcept>
    : function_traits<Return(*)(Args...)> {};


// ----------------------------
// Member function
// ----------------------------

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...)> {
    using return_type = Return;
    using args = std::tuple<Args...>;
    using class_type = Class;

    static constexpr std::size_t arity = sizeof...(Args);
    static constexpr bool is_member_function = true;
};


// const member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) const>
    : function_traits<Return(Class::*)(Args...)> {};


// volatile member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) volatile>
    : function_traits<Return(Class::*)(Args...)> {};


// const volatile member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) const volatile>
    : function_traits<Return(Class::*)(Args...)> {};


// noexcept member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) noexcept>
    : function_traits<Return(Class::*)(Args...)> {};


// const noexcept member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) const noexcept>
    : function_traits<Return(Class::*)(Args...) const> {};


// volatile noexcept member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) volatile noexcept>
    : function_traits<Return(Class::*)(Args...) volatile> {};


// const volatile noexcept member

template <typename Class, typename Return, typename... Args>
struct function_traits<Return(Class::*)(Args...) const volatile noexcept>
    : function_traits<Return(Class::*)(Args...) const volatile> {};


// ============================================================
// Lambda / functor traits
// ============================================================
//
// For:
//
//     [](Button& button, int value)
//
// operator() is technically a member function, but for Moddable
// we want to treat Button& as the explicit object argument.
//
// ============================================================

template <typename Callable>
struct function_traits
    : function_traits<decltype(&std::remove_cvref_t<Callable>::operator())> {

    using base =
        function_traits<decltype(&std::remove_cvref_t<Callable>::operator())>;

    static constexpr bool is_member_function = false;
};


// ============================================================
// Argument extraction
// ============================================================

namespace moddable_detail {

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;


// ------------------------------------------------------------
// std::any -> requested argument
// ------------------------------------------------------------

template <typename T>
decltype(auto) getArgument(const std::any& value) {
    using U = remove_cvref_t<T>;

    // Exact type.
    if (value.type() == typeid(U)) {
        if constexpr (std::is_reference_v<T>) {
            return std::any_cast<U&>(
                const_cast<std::any&>(value)
            );
        } else {
            return std::any_cast<U>(value);
        }
    }

    // const char* -> std::string
    if constexpr (std::is_same_v<U, std::string>) {
        if (value.type() == typeid(const char*)) {
            return std::string(std::any_cast<const char*>(value));
        }

        if (value.type() == typeid(char*)) {
            return std::string(std::any_cast<char*>(value));
        }
    }

    // std::string -> std::string_view
    if constexpr (std::is_same_v<U, std::string_view>) {
        if (value.type() == typeid(std::string)) {
            return std::string_view(
                std::any_cast<const std::string&>(value)
            );
        }

        if (value.type() == typeid(const char*)) {
            return std::string_view(
                std::any_cast<const char*>(value)
            );
        }

        if (value.type() == typeid(char*)) {
            return std::string_view(
                std::any_cast<char*>(value)
            );
        }
    }

    throw std::invalid_argument(
        "Moddable method received an argument of the wrong type"
    );
}


// ============================================================
// Invoke implementation
// ============================================================

template <
    typename Callable,
    typename Object,
    typename Tuple,
    std::size_t... I
>
MethodResult invokeFreeImpl(
    Callable& callable,
    Object& object,
    const std::vector<std::any>& arguments,
    std::index_sequence<I...>
) {
    using Traits = function_traits<Callable>;
    using Return = Traits::return_type;

    // Lambda/free function form is:
    //
    //     callable(Object&, Arg1, Arg2, ...)
    //
    using ObjectArgument =
        std::tuple_element_t<0, typename Traits::args>;

    static_assert(
        std::is_convertible_v<Object&, ObjectArgument>,
        "The first argument of a Moddable callable must accept the object"
    );

    if constexpr (std::is_void_v<Return>) {
        std::invoke(
            callable,
            object,
            getArgument<
                std::tuple_element_t<I + 1, typename Traits::args>
            >(arguments[I])...
        );

        return {};
    } else {
        static_assert(
            std::is_copy_constructible_v<std::remove_cv_t<Return>>,
            "Moddable return values must be copy constructible"
        );

        return MethodResult(
            std::invoke(
                callable,
                object,
                getArgument<
                    std::tuple_element_t<I + 1, typename Traits::args>
                >(arguments[I])...
            )
        );
    }
}


template <
    typename Callable,
    typename Object,
    typename Tuple,
    std::size_t... I
>
MethodResult invokeMemberImpl(
    Callable& callable,
    Object& object,
    const std::vector<std::any>& arguments,
    std::index_sequence<I...>
) {
    using Traits = function_traits<Callable>;
    using Return = Traits::return_type;

    using Class = Traits::class_type;

    static_assert(
        std::is_convertible_v<Object&, Class&>,
        "The member function does not belong to this object hierarchy"
    );

    if constexpr (std::is_void_v<Return>) {
        std::invoke(
            callable,
            object,
            getArgument<
                std::tuple_element_t<I, typename Traits::args>
            >(arguments[I])...
        );

        return {};
    } else {
        static_assert(
            std::is_copy_constructible_v<std::remove_cv_t<Return>>,
            "Moddable return values must be copy constructible"
        );

        return MethodResult(
            std::invoke(
                callable,
                object,
                getArgument<
                    std::tuple_element_t<I, typename Traits::args>
                >(arguments[I])...
            )
        );
    }
}


// ============================================================
// Callable -> erased Moddable function
// ============================================================

template <typename Object, typename Callable>
auto makeFunction(Callable&& callable) {

    using CallableType = std::remove_cvref_t<Callable>;
    using Traits = function_traits<CallableType>;

    constexpr bool IsMember =
        Traits::is_member_function;

    constexpr std::size_t Arity =
        Traits::arity;

    if constexpr (IsMember) {

        return [
            function = std::forward<Callable>(callable)
        ](Object& object,
          const std::vector<std::any>& arguments) mutable
            -> MethodResult {

            if (arguments.size() != Arity) {
                throw std::invalid_argument(
                    "Moddable method received the wrong number of arguments"
                );
            }

            return invokeMemberImpl<
                CallableType,
                Object,
                typename Traits::args
            >(
                function,
                object,
                arguments,
                std::make_index_sequence<Arity>{}
            );
        };

    } else {

        static_assert(
            Arity >= 1,
            "A Moddable lambda/free function must take the object as its first argument"
        );

        constexpr std::size_t ArgumentCount = Arity - 1;

        return [
            function = std::forward<Callable>(callable)
        ](Object& object,
          const std::vector<std::any>& arguments) mutable
            -> MethodResult {

            if (arguments.size() != ArgumentCount) {
                throw std::invalid_argument(
                    "Moddable method received the wrong number of arguments"
                );
            }

            return invokeFreeImpl<
                CallableType,
                Object,
                typename Traits::args
            >(
                function,
                object,
                arguments,
                std::make_index_sequence<ArgumentCount>{}
            );
        };
    }
}

} // namespace moddable_detail


// ============================================================
// Method
// ============================================================

template <typename Object>
class Method {
public:
    using Function =
        std::function<
            MethodResult(
                Object&,
                const std::vector<std::any>&
            )
        >;

    Method(
        Object& owner,
        std::string name,
        Function function
    )
        : _owner(owner),
          _name(std::move(name)),
          _function(std::move(function)) {}

    MethodResult operator()() {
        return invoke({});
    }

    template <typename... Args>
    MethodResult operator()(Args&&... args) {
        return invoke({
            std::any(std::forward<Args>(args))...
        });
    }

    [[nodiscard]]
    const std::string& name() const noexcept {
        return _name;
    }

private:
    MethodResult invoke(
        const std::vector<std::any>& arguments
    ) {
        return _function(_owner, arguments);
    }

    Object& _owner;
    std::string _name;
    Function _function;
};


// ============================================================
// Moddable
// ============================================================

template <typename Object, typename Parent = void>
class Moddable {
public:
    using Function = Method<Object>::Function;

private:
    struct Registry {
        std::unordered_map<std::string, Function> methods;
        mutable std::shared_mutex mutex;
    };

    static Registry& registry() {
        static Registry instance;
        return instance;
    }

    // --------------------------------------------------------
    // Own methods only
    // --------------------------------------------------------

    static std::optional<Function>
    findOwn(std::string_view name) {

        auto& r = registry();

        std::shared_lock lock(r.mutex);

        const auto it = r.methods.find(std::string(name));

        if (it == r.methods.end()) {
            return std::nullopt;
        }

        return it->second;
    }

public:

    // ========================================================
    // Registration
    // ========================================================

    template <typename Callable>
    static bool extend(
        std::string name,
        Callable&& callable
    ) {
        Function function =
            moddable_detail::makeFunction<Object>(
                std::forward<Callable>(callable)
            );

        auto& r = registry();

        std::unique_lock lock(r.mutex);

        const bool replaced =
            r.methods.contains(name);

        r.methods.insert_or_assign(
            std::move(name),
            std::move(function)
        );

        return replaced;
    }


    // ========================================================
    // Remove
    // ========================================================

    static bool removeMethod(
        std::string_view name
    ) {
        auto& r = registry();

        std::unique_lock lock(r.mutex);

        return r.methods.erase(
            std::string(name)
        ) != 0;
    }


    // ========================================================
    // Own method lookup
    // ========================================================

    static bool hasOwnMethod(
        std::string_view name
    ) {
        return findOwn(name).has_value();
    }


    // ========================================================
    // Full lookup
    // ========================================================

    static bool hasMethod(
        std::string_view name
    ) {
        if (findOwn(name).has_value()) {
            return true;
        }

        if constexpr (!std::is_void_v<Parent>) {
            return Parent::hasModdableMethod(name);
        }

        return false;
    }


    static Function find(
        std::string_view name
    ) {
        if (const auto own = findOwn(name)) {
            return *own;
        }

        if constexpr (!std::is_void_v<Parent>) {

            /*
             * This assertion happens when find() is actually
             * instantiated, meaning Object is complete.
             */
            static_assert(
                std::is_base_of_v<Parent, Object>,
                "Moddable's Parent must be an actual C++ base class"
            );

            const auto parentFunction =
                Parent::findModdableMethod(name);

            /*
             * Convert:
             *
             *     Parent& -> MethodResult
             *
             * into:
             *
             *     Object& -> MethodResult
             *
             * by safely viewing Object as its Parent.
             */
            return [
                parentFunction
            ](
                Object& object,
                const std::vector<std::any>& arguments
            ) mutable -> MethodResult {

                return parentFunction(
                    static_cast<Parent&>(object),
                    arguments
                );
            };
        }

        throw std::runtime_error(
            "Moddable method not found: " +
            std::string(name)
        );
    }


    // ========================================================
    // Get method
    // ========================================================

    static Method<Object> getMethod(
        Object& object,
        std::string_view name
    ) {
        return Method<Object>(
            object,
            std::string(name),
            find(name)
        );
    }


    // ========================================================
    // Method names
    // ========================================================

    static std::vector<std::string>
    ownMethodNames() {

        auto& r = registry();

        std::shared_lock lock(r.mutex);

        std::vector<std::string> result;

        result.reserve(r.methods.size());

        for (const auto& [name, _] : r.methods) {
            result.push_back(name);
        }

        return result;
    }


    static std::vector<std::string>
    methodNames() {

        std::vector<std::string> result;

        if constexpr (!std::is_void_v<Parent>) {
            result = Parent::moddableMethodNames();
        }

        const auto own = ownMethodNames();

        for (const auto& name : own) {

            const auto it =
                std::find(
                    result.begin(),
                    result.end(),
                    name
                );

            if (it == result.end()) {
                result.push_back(name);
            }
        }

        return result;
    }
};

// ============================================================
// Macros
// ============================================================

#define MODDABLE_ROOT(TYPE)                                             \
private:                                                                \
    using ModdableType = Moddable<TYPE>;                                \
                                                                         \
public:                                                                 \
    template <typename Callable>                                        \
    static bool extend(std::string name, Callable&& callable) {         \
        return ModdableType::extend(                                    \
            std::move(name),                                             \
            std::forward<Callable>(callable)                            \
        );                                                               \
    }                                                                    \
                                                                         \
    static bool removeMethod(std::string_view name) {                   \
        return ModdableType::removeMethod(name);                        \
    }                                                                    \
                                                                         \
    static bool hasMethod(std::string_view name) {                      \
        return ModdableType::hasMethod(name);                           \
    }                                                                    \
                                                                         \
    static bool hasOwnMethod(std::string_view name) {                   \
        return ModdableType::hasOwnMethod(name);                        \
    }                                                                    \
                                                                         \
    static bool hasModdableMethod(std::string_view name) {              \
        return ModdableType::hasMethod(name);                           \
    }                                                                    \
                                                                         \
    static auto findModdableMethod(std::string_view name) {             \
        return ModdableType::find(name);                                \
    }                                                                    \
                                                                         \
    static auto moddableMethodNames() {                                 \
        return ModdableType::methodNames();                             \
    }                                                                    \
                                                                         \
    Method<TYPE> m(std::string_view name) {                             \
        return ModdableType::getMethod(*this, name);                    \
    }                                                                    \
                                                                         \
    Method<TYPE> method(std::string_view name) {                        \
        return m(name);                                                  \
    }                                                                    \
                                                                         \
private:


#define MODDABLE_DERIVED(TYPE, PARENT)                                  \
private:                                                                \
    using ModdableType = Moddable<TYPE, PARENT>;                        \
                                                                         \
public:                                                                 \
    template <typename Callable>                                        \
    static bool extend(std::string name, Callable&& callable) {         \
        return ModdableType::extend(                                    \
            std::move(name),                                             \
            std::forward<Callable>(callable)                            \
        );                                                               \
    }                                                                    \
                                                                         \
    static bool removeMethod(std::string_view name) {                   \
        return ModdableType::removeMethod(name);                        \
    }                                                                    \
                                                                         \
    static bool hasMethod(std::string_view name) {                      \
        return ModdableType::hasMethod(name);                           \
    }                                                                    \
                                                                         \
    static bool hasOwnMethod(std::string_view name) {                   \
        return ModdableType::hasOwnMethod(name);                        \
    }                                                                    \
                                                                         \
    static bool hasModdableMethod(std::string_view name) {              \
        return ModdableType::hasMethod(name);                           \
    }                                                                    \
                                                                         \
    static auto findModdableMethod(std::string_view name) {             \
        return ModdableType::find(name);                                \
    }                                                                    \
                                                                         \
    static auto moddableMethodNames() {                                 \
        return ModdableType::methodNames();                             \
    }                                                                    \
                                                                         \
    Method<TYPE> m(std::string_view name) {                             \
        return ModdableType::getMethod(*this, name);                    \
    }                                                                    \
                                                                         \
    Method<TYPE> method(std::string_view name) {                        \
        return m(name);                                                  \
    }                                                                    \
                                                                         \
private:


// ============================================================
// Automatic static registration
// ============================================================

#define MODDABLE_DETAIL_JOIN_IMPL(A, B) A##B
#define MODDABLE_DETAIL_JOIN(A, B) MODDABLE_DETAIL_JOIN_IMPL(A, B)

#define MODDABLE_DETAIL_EXTEND_IMPL(                                  \
    TYPE, NAME, FUNCTION, ID                                           \
)                                                                       \
namespace {                                                             \
    [[maybe_unused]] const bool                                         \
        MODDABLE_DETAIL_JOIN(_moddable_registration_, ID) = [] {       \
            TYPE::extend(                                               \
                NAME,                                                    \
                FUNCTION                                                 \
            );                                                           \
            return true;                                                \
        }();                                                             \
}

#define MODDABLE_EXTEND(TYPE, NAME, FUNCTION)                           \
    MODDABLE_DETAIL_EXTEND_IMPL(                                        \
        TYPE, NAME, FUNCTION, __COUNTER__                                \
    )
