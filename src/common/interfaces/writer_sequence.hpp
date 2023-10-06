#ifndef SYSTEM_INTERFACE_WRITER_SEQUENCE_HPP
#define SYSTEM_INTERFACE_WRITER_SEQUENCE_HPP

namespace sys {
namespace interface {
/**
 * @brief The IWriterSequence_ class - класс-интерфейс писателя последовательности сообщений
 */
class IWriterSequence_ {
public:
    /**
     * @brief add_element_sequence - добавление нового элемента в последовательность
     * @return результат добавления
     */
    virtual bool add_element_sequence(void *ptr_message) = 0;
};
}
}

#endif
