#ifndef SYSTEM_INTERFACE_CONCEPT_HPP
#define SYSTEM_INTERFACE_CONCEPT_HPP

#include "reader_sequence.hpp"
#include "writer_sequence.hpp"
#include "buffer_concept.hpp"

namespace sys {
namespace interface {
/**
 * @brief The ISequence class - класс-интерфейс взаимодействия компонент
 * Интерфейс определяется на этапе чтения файлов конфигурации, содержащих информацию о сообщениях системы
 */
class ISequence_ :
        public IReaderSequence_,
        public IWriterSequence_ {
public:
    using buffer_t = boost::shared_ptr<buffer::ISequenceBufferConcept_>;

    /**
     * @brief ISequence_ - конструктор
     * @param reader_id - уникальный идентификатор читателя
     * @param writer_id - уникальный идентификатор писателя
     * @param message_name - наименование сообщения
     */
    explicit ISequence_(uint16_t component_id,
                        buffer_t buffer) :
        component_id_(component_id),
        buffer_(buffer) {}

    ~ISequence_() = default;

    /**
     * @brief read_element_sequence - получение очередного сообщения из буфера
     * @return указатель на очередное сообщение
     */
    void *read_element_sequence(){
        if (this->buffer_) {
            return this->buffer_->read_element(this->component_id_, this->count_read_messages_);
        }
        return nullptr;
    }

    /**
     * @brief delete_element_sequence - удаление прочитанного сообщения
     * @param el_seq_ - указатель на прочитанное сообщение
     * @return результат удаления
     */
    bool delete_element_sequence(void *el_seq_) {
        if (this->buffer_) {
            this->count_read_messages_++;
            return true;
        }
        return false;
    }

    /**
     * @brief add_element_sequence - добавление нового сообщения в очередь
     * @param ptr_message - указатель на новое сообщение
     */
    bool add_element_sequence(void *ptr_message){
        if (this->buffer_) {
            return this->buffer_->add_new_element(this->component_id_,ptr_message);
        }
        return false;
    }

    /**
     * @brief message_interface - получение наименования сообщения, для которого создан интерфейс
     * @return наименование сообщения
     */
    std::string message_interface() { return this->buffer_->message_name(); }

private:
    uint16_t component_id_ = 0;         /// <--- уникальный идентификатор компоненты системы
    uint64_t count_read_messages_ = 0;  /// <--- количество прочитанных сообщений компонентой
    buffer_t buffer_ = nullptr;         /// <--- указатель на буфер сообщений
};
}
}

#endif
