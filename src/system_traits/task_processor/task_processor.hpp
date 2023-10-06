#ifndef SYSTEM_TASK_PROCESSOR_HPP
#define SYSTEM_TASK_PROCESSOR_HPP

#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <iostream>
#include <chrono>
#include <thread>

#include "../../common/common.hpp"

GENERATE_REQUIRED_MEMBER_CLASS(component_name)
GENERATE_REQUIRED_MEMBER_CLASS(component_id)
GENERATE_REQUIRED_MEMBER_CLASS(init)
GENERATE_REQUIRED_MEMBER_CLASS(run)

namespace sys {
namespace process {
class task_processor : private boost::asio::noncopyable {
    template <class T>
    class base_task_runner {
        public:
        using f_init = boost::function<bool(const T&)>;
        using f_run = boost::function<void(const T&)>;
        f_init m_initter;
        f_run m_runner;
    };

    template <class T, class Enable = void>
    class task_runner : public base_task_runner<T> {
    public:
        task_runner() {
            this->m_initter = [ ](const T &task){ return task.init(); };
            this->m_runner = [ ](const T &task){ task.run(); };
        }
    };

    template <class T>
    class task_runner <
            T,
            typename boost::enable_if<
            boost::mpl::or_<
            boost::is_pointer<T>,
            sys::type_traits::type_detector::is_shared_ptr<T>
            >
            >::type
            > : public base_task_runner<T> {
    public:
        task_runner() {
            this->m_initter = [ ](const T &task){ return task->init(); };
            this->m_runner = [ ](const T &task){ task->run(); };
        }
    };

    /** Класс - обертка для регистрации задач */
    template <class T>
    class task_wrapped : private task_runner<T> {
    public:
        /** Конструктор */
        explicit task_wrapped(const T &f, uint32_t *count_tasks) :
            task_unwrapped(f),
            count_tasks_(count_tasks) {}

        /** Оператор для обработки исключений зарегистрированных задач */
        void operator()() const {
            /// Сброс прерывания
            try {
                boost::this_thread::interruption_point();
            } catch (const boost::thread_interrupted&) {}

            try {
                if (this->m_initter(this->task_unwrapped)) {
                    this->m_runner(this->task_unwrapped);
                }
                *this->count_tasks_ = *this->count_tasks_-1;
            }  catch (const std::exception &e) {
                std::cerr << "\nError! Exception: " << e.what();
            } catch (const boost::thread_interrupted&) {
                std::cerr << "\nError! Thread interrupted!";
            } catch (...) { std::cerr << "\nUnknown error!"; }
        }

    private:
        T task_unwrapped;   /// <--- Зарегистрированная задача
        uint32_t *count_tasks_ = nullptr;
    };

protected:
    boost::asio::io_service& get_ios() {
        static boost::asio::io_service ios;
        static boost::asio::io_service::work work(ios);
        return ios;
    }

    /** Функция создания task_wrapped из функтора пользователя */
    template <class T>
    task_wrapped<T> make_task_wrapped(const T &task_unwrapped) {
        return task_wrapped<T>(task_unwrapped,&this->m_count_tasks);
    }

public:
    /** Конструктор */
    task_processor() : m_count_tasks(0) { }

    /** Деструктор */
    virtual ~task_processor(){}

    /** Регистрация задач для выполнения */
    /**
     * Передано:
     *      - указатель на компоненту;
     *      - умный указатель на компоненту.
     */
    template <class T>
    typename boost::enable_if<
    boost::mpl::or_<
    boost::is_pointer<T>,
    sys::type_traits::type_detector::is_shared_ptr<T>
    >,bool
    >::type
    push_task(const T &task_unwrapped) {
        using ref_pointer = decltype(*T());
        using value_pointer = typename std::remove_reference<ref_pointer>::type;
        if (has_field_init<value_pointer>::value && has_field_run<value_pointer>::value) {
            this->get_ios().post(this->make_task_wrapped(task_unwrapped));
            if (has_field_component_name<value_pointer>::value) {
                std::cout << "\nTask_processor: push new task: " << task_unwrapped->component_name();
            }
            this->m_count_tasks++;
            return true;
        }
        return false;
    }

    /**
     * Передано:
     *      - указатель на указатель компоненты;
     *      - указатель на умный указатель компоненты;
     *      - умный указатель на умный указатель компоненты;
     *      - умный указатель на указатель компоненты;
     */
    template <class T>
    typename boost::enable_if<
    boost::mpl::or_<
    boost::mpl::and_<
    boost::mpl::or_<
    boost::is_pointer<T>,
    sys::type_traits::type_detector::is_shared_ptr<T>
    >,
    boost::is_pointer<typename boost::remove_reference<decltype(*T())>::type>
    >,
    boost::mpl::and_<
    boost::mpl::or_<
    boost::is_pointer<T>,
    sys::type_traits::type_detector::is_shared_ptr<T>
    >,
    sys::type_traits::type_detector::is_shared_ptr<typename boost::remove_reference<decltype(*T())>::type>
    >
    >,bool
    >::type
    push_task(const T &task_unwrapped) {
        using ref_smart_pointer = decltype(*T());
        ref_smart_pointer r_task = *task_unwrapped;
        using val_smart_pointer = typename std::remove_reference<ref_smart_pointer>::type;
        using ref_val_type = decltype(*val_smart_pointer());
        using val_type = typename boost::remove_reference<ref_val_type>::type;
        if (has_field_init<val_type>::value && has_field_run<val_type>::value) {
            this->get_ios().post(this->make_task_wrapped(r_task));
            if (has_field_component_name<val_type>::value) {
                std::cout << "\nTask_processor: push new task: " << r_task->component_name();
            }
            this->m_count_tasks++;
            return true;
        }
        return false;
    }

    /**
     * Передано:
     *      - не указатель;
     *      - не умный указатель.
     */
    template <class T>
    typename boost::disable_if<
    boost::mpl::or_<
    boost::is_pointer<T>,
    sys::type_traits::type_detector::is_shared_ptr<T>
    >,bool
    >::type
    push_task(const T &task_unwrapped) {
        if (has_field_init<T>::value && has_field_run<T>::value) {
            this->get_ios().post(this->make_task_wrapped(task_unwrapped));
            if (has_field_component_name<T>::value) {
                std::cout << "\nPush new task: " << task_unwrapped.component_name();
            }
            this->m_count_tasks++;
            return true;
        }
        return false;
    }

    /** Запуск всех компонент */
    void start() {
        if (this->m_count_tasks) {
            boost::asio::io_service &ios = this->get_ios();
            for (std::size_t i = 0; i < this->m_count_tasks; i++) {
                this->m_threads.create_thread([ &ios ](){
                    ios.run();
                });
            }
            while (this->m_count_tasks) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            ios.stop();
            this->m_threads.join_all();
        }
    }

protected:
    uint32_t m_count_tasks;   /// <--- количество зарегистрированных для выполнения задач
    boost::thread_group m_threads;   /// <--- потоки (выполнение работы нитей)
};
}   /// <--- process
}       /// <--- sys

#endif
