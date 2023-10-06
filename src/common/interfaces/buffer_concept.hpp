#ifndef SYSTEM_BUFFER_CONCEPT_HPP
#define SYSTEM_BUFFER_CONCEPT_HPP

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "sequence_buffer.hpp"

GENERATE_HAS_MEMBER_TYPE(message_t)

namespace sys {
namespace interface {
namespace buffer {
class ISequenceBufferConcept_ {
    /**
     * @brief The buffer_concept_ struct - закрытая оболочка буфера сообщений
     */
    struct buffer_concept_ {
        /**
         * @brief add_new_element - добавление нового элемента в буфер сообщений
         * @param ptr_message - указатель на добавляемое сообщение
         * @return результат добавления сообщений
         */
        virtual bool add_new_element(void *ptr_message) = 0;

        /**
         * @brief read_next_element - получение указателя на следующий элемент буфера
         * @return указатель на следующее сообщение
         */
        virtual void *read_next_element(uint64_t numb_msg) = 0;

        /**
         * @brief delete_element - удаление прочитанного сообщения
         * @param numb_msg - порядковый номер сообщения
         * @return результат удаления
         */
        virtual bool delete_element(uint16_t numb_msg) = 0;

        /**
         * @brief message_name - получение наименования сообщения
         * @return наименование сообщения
         */
        virtual std::string message_name() = 0;

        /**
         * @brief message_uid - получение уникального идентификатора сообщения
         * @return уникальный идентификатор сообщения
         */
        virtual uint16_t message_uid() = 0;

        /**
         * @brief messages_count - получение количества записанных в буфер сообщений
         * @return количество записанных в буфер сообщений
         */
        virtual uint64_t messages_count() = 0;
    };

    template <class T_buffer, class Enable = void> struct buffer_model_ { };

    /**
     * Модель буфера сообщений
     */
    template <class T_buffer>
    class buffer_model_ <
            T_buffer,
            typename boost::enable_if<
            has_member_type_message_t<T_buffer>
            >::type
            > : public buffer_concept_ {
        using buffer_t = T_buffer;
        using message_t = typename buffer_t::message_t;

    public:
        explicit buffer_model_(buffer_t *buffer) : model_buffer_(buffer) { }

        ~buffer_model_(){ delete this->model_buffer_; }

        bool add_new_element(void *ptr_message) {
            return this->model_buffer_->add_new_element(*static_cast<message_t*>(ptr_message));
        }

        void *read_next_element(uint64_t numb_msg) {
            return this->model_buffer_->get_element(numb_msg);
        }

        bool delete_element(uint16_t numb_msg) {
            return this->model_buffer_->delete_element(numb_msg);
        }

        std::string message_name() {
            return this->model_buffer_->message_name();
        }

        uint16_t message_uid() {
            return this->model_buffer_->message_id();
        }

        uint64_t messages_count() {
            return this->model_buffer_->messages_count();
        }

    private:
        buffer_t *model_buffer_;    /// <--- указатель на реальный буфер сообщений
    };

public:
    /**
     * @brief ISequenceBuffer_ - Конструктор
     */
    template <class T_buffer>
    explicit ISequenceBufferConcept_(T_buffer *buffer) :
        buffer_(new buffer_model_<T_buffer>(buffer)) {}

    /** Деструктор */
    ~ISequenceBufferConcept_() = default;

    /**
     * @brief add_new_element - добавление нового элемента в буфер
     * @param writer_id - уникальный идентификатор писателя сообщений
     * @param ptr_message - указатель на новое сообщение
     * @return результат добавления нового сообщения
     */
    bool add_new_element(uint16_t writer_id, void *ptr_message) {
        boost::lock_guard<boost::mutex> lock_(this->mtx_);
        auto itr = std::find_if(this->map_writers_.begin(),this->map_writers_.end(),
                                [ & ](auto &id_){ return id_ == writer_id; });
        if (itr != this->map_writers_.end() && writer_id) {
            message::message_head_ *head_ = (message::message_head_*)ptr_message;
            head_->message_source_ = writer_id;
            head_->message_uid_ = this->buffer_->message_uid();
            head_->message_id_ = this->buffer_->messages_count();

            struct timespec ts;
            timespec_get(&ts,TIME_UTC);
            head_->message_time_ = ts.tv_nsec;

            this->buffer_->add_new_element(ptr_message);

            return true;
        }
        return false;
    }

    /**
     * @brief read_element - получение указателя на следующее сообщение в очереди для компоненты
     * @param reader_id - уникальный идентификатор компоненты
     * @return указатель на очередное сообщение
     */
    void *read_element(uint16_t reader_id, uint64_t numb_msg_) {
        boost::lock_guard<boost::mutex> lock_(this->mtx_);
        auto itr = std::find_if(this->map_readers_.begin(),this->map_readers_.end(),
                                [ & ](auto &reader_){ return reader_ == reader_id; });
        if (itr != this->map_readers_.end()) {
            return this->buffer_->read_next_element(numb_msg_);
        }
        return nullptr;
    }

    /**
     * @brief message_name - получение наименования сообщения
     * @return наименование сообщения
     */
    std::string message_name() {
        boost::lock_guard<boost::mutex> lock_(this->mtx_);
        if (this->buffer_) {
            return this->buffer_->message_name();
        }
        return "";
    }

    /**
     * @brief set_map_readers - установление списка читателей сообщения
     * @param map_readers список читателей сообщения
     */
    void set_map_readers(std::vector<uint16_t> &map_readers){
        boost::lock_guard<boost::mutex> lock_(this->mtx_);
        this->map_readers_ = map_readers;
    }

    /**
     * @brief set_map_writers - установление списка писателей сообщения
     * @param map_writers список писателей сообщения
     */
    void set_map_writers(std::vector<uint16_t> &map_writers) {
        boost::lock_guard<boost::mutex> lock_(this->mtx_);
        this->map_writers_ = map_writers;
    }

    /**
     * @brief push_back_reader - добавление читателя
     * @param component_id идентификатор читателя
     */
    void push_back_reader(uint16_t &component_id) {
        boost::lock_guard<boost::mutex> lock_(this->mtx_);
        this->map_readers_.push_back(component_id);
    }

private:
    boost::shared_ptr<buffer_concept_> buffer_ = nullptr;   /// <--- указатель на буфер сообщений

    std::vector<uint16_t> map_readers_ = {};                /// <--- список читателей сообщения данного типа
    std::vector<uint16_t> map_writers_ = {};                /// <--- список писателей сообщения данного типа
    boost::mutex mtx_;                                      /// <--- мьютекс доступа к буферу сообщений
};
}           /// <--- buffer
}       /// <--- interface
}   /// <--- sys

#endif
