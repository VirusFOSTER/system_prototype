#ifndef SYSTEM_INTERFACE_READER_SEQUENCE_HPP
#define SYSTEM_INTERFACE_READER_SEQUENCE_HPP

namespace sys {
namespace interface {
/**
 * @brief The IReaderSequence_ class - класс-интерфейс читателя последовательности сообщений
 */
class IReaderSequence_ {
public:
    /**
     * @brief read_element_sequence - чтение следующего в очереди сообщения
     * @return - указатель на сообщение
     */
    virtual void *read_element_sequence() = 0;

    /**
     * @brief delete_element_sequence - удаление сообщения из последовательности
     * если эту операцию не выполнять, компонента будет читать одно и то же сообщение при каждом запросе
     * @param el_seq_ - указатель на удаляемый элемент
     * @return результат удаления
     */
    virtual bool delete_element_sequence(void *el_seq_) = 0;
};
}
}

#endif
