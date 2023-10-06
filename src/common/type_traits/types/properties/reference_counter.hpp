#ifndef REFERENCE_COUNTER_HPP
#define REFERENCE_COUNTER_HPP

#include <iostream>

namespace sys {
    namespace type_traits {
        namespace types {
            namespace properties {
                class counter_ {
                    public:
                    counter_() : m_counter_(0) { }
                    counter_(const counter_&) = delete;
                    counter_(counter_&&) = default;

                    counter_& operator=(const counter_&) = delete;

                    ~counter_() { }

                    void reset() { this->m_counter_ = 0; }

                    unsigned long get() { return this->m_counter_; }

                    void operator++() { this->m_counter_++; }

                    void operator++(int) { this->m_counter_++; }

                    void operator--() { this->m_counter_--; }

                    void operator--(int) { this->m_counter_--; }

                    private:
                    unsigned long m_counter_ = 0;
                };
            }               /// <--- properties
        }           /// <--- types
    }       /// <--- type_traits
}   /// <--- sys

#endif
