#pragma once

// This project's headers
#include "exception.hpp"

namespace SpanningTree {

static const char* IncorrectTransitionString = "Incorrect transition to the state";

#define THROW_SM_EXCEPTION(MSG) {                   \
    throw SmException { MSG, __func__, __LINE__ }   \
}

/**
 * @brief Checks if pointer is null
 * 
 * @tparam T Pointer type
 * @param ptr Pointer
 * @return true If passed is null
 * @return false If passed pointer is not null
 */
template <typename T>
inline bool IsNull(const T& ptr) noexcept {
    return ptr == nullptr;
}

/**
 * @brief Get the State Instance object
 * 
 * @tparam H State handler
 * @tparam S Concrete state
 * @return STATE_HANDLER 
 */
template<typename H, typename S>
H& GetStateInstance() {
    static H state { };

    if (IsNull(state)) {
        state.reset(new S);
    }

    return state;
}

#define RETURN_SINGLETON_INSTANCE(B, D)     \
    static D instance { };                  \
    return dynamic_cast<B&>(instance)

/**
 * @tparam B Base class
 * @tparam D Derived class (optional)
 * @exception std::bad_cast if B and D are out of inheritance hierarchy
 */
template <typename B, typename D = B>
inline B& SingletonInstance() {
    static D instance { };

//    if (not std::is_base_of<B, D>::value) {
//        static B rescueInstance { };
//        return rescueInstance;
//    }

    return dynamic_cast<B&>(instance);
}

/**
 * @tparam B Base class
 * @tparam D Derived class (optional)
 */
template<typename B, typename D = B>
std::shared_ptr<B>& SharableSingleInstance() {
    static std::shared_ptr<B> state { };

    if (IsNull(state)) {
        state.reset(dynamic_cast<B*>(new D));
    }

    return state;
}

//template<typename T>
//T* GetStateInstance() {
//    static T state { };

//    return &state;
//}

} // namespace SpanningTree
