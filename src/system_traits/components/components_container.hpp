#ifndef SYSTEM_COMPONENTS_CONTAINER_HPP
#define SYSTEM_COMPONENTS_CONTAINER_HPP

#include <boost/type_traits.hpp>
#include <iostream>
#include "../../common/common.hpp"

namespace sys {
namespace component {
namespace base_fc {
/// Предварительное объявление базовой функциональной компоненты
class base_functional_component_;
}

/**
 * @brief The components_container_ class - контейнер, содержащий все зарегистрированные компоненты системы
 */
class components_container_ {
public:
    using ptr_component_t = boost::shared_ptr<base_fc::base_functional_component_>;

    /**
     * @brief components_container_ - конструктор
     */
    explicit components_container_();

    /** деструктор */
    ~components_container_() { this->components_.clear(); }

    /**
     * Регистрация компоненты системы
     */
    bool component_registration(ptr_component_t kmpt, const std::string kmpt_name);

    /**
     * @brief get_component - получение указателя на компоненту
     * @param kmpt_name - наименование компоненты
     * @return указатель на компоненту
     */
    ptr_component_t get_component(const std::string kmpt_name);

    /**
     * @brief get_component_name - получить наименование компоненты по идентификатору
     * @param kmpt_id - уникальный идентификатор компоненты
     * @return наименование компоненты
     */
    std::string get_component_name(const uint16_t kmpt_id);

    /**
     * @brief remove_component - удаление компоненты из контейнера
     * @param kmpt_name - наименование компоненты
     */
    void remove_component(const std::string kmpt_name);

private:
    std::map<std::string,ptr_component_t> components_ = {};   /// <--- массив зарегистрированных компонент
    boost::mutex mtx_;              /// <--- мьютекс доступа к контейнеру компонент
};  /// <--- components_container_
}       /// <--- component
}   /// <--- sys

extern boost::shared_ptr<sys::component::components_container_> fc_container_;

#endif
