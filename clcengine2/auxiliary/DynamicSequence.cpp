//
// Created by EgrZver on 03.10.2023.
//
#include <iostream>
#include "DynamicSequence.h"

namespace CLCEngine {
    template <typename T>
    DynamicSequence<T>::DynamicSequence() {
        this->data = nullptr;
        //this->data = new T [1];
        this->length = 0;
        this->capacity = 0;
    }



    template <typename T>
    DynamicSequence<T>::~DynamicSequence() {
        //std::cout << "DESTRUCT: " <<this->data << std::endl;
        //if (this->length != 0 || this->data || this->capacity != 0) {
        if (this->data) {
            delete [] this->data;
        }
    }

    template <typename T>
    void DynamicSequence<T>::append(T element) {
        if (this->length + 1 <= this->capacity) {
            this->data[this->length] = element;
            this->length++;
        } else {
            if (this->capacity == 0 || !this->data) {
                this->capacity = 1;
                this->data = new T [1];
                this->data[0] = element;
                this->length = 1;
            } else {
                T* temp = new T [this->capacity];
                for (int i = 0; i < this->capacity; i++) temp[i] = this->data[i];
                delete [] this->data;
                //this->capacity *= 2;
                this->data = new T [this->capacity * 2];
                for (int i = 0; i < this->capacity; i++) this->data[i] = temp[i];
                this->capacity *= 2;
                this->data[this->length] = element;
                this->length++;
                delete [] temp;
            }

        }
    }

    template <typename T>
    void DynamicSequence<T>::pop() {
        if (this->length > 0) this->length--;
    }

    template <typename T>
    int DynamicSequence<T>::getLength() {
        return this->length;
    }

    template <typename T>
    T DynamicSequence<T>::getElement(int index) {
        return this->data[index];
    }

    template <typename T>
    void DynamicSequence<T>::copyFrom(DynamicSequence<T>* source) {
        this->clear();
        for (int i = 0; i < source->getLength(); i++) {
            this->append(source->getElement(i));
        }
    }

    template <typename T>
    void DynamicSequence<T>::clear() {
        this->length = 0;
    }

    /*
    template<typename T>
    void DynamicSequence<T>::makeDeepCopy(const DynamicSequence<T> &source) {
        if (this->capacity != 0 && this->length == 0) {
        //if ((this->capacity != 0) && (this->capacity > source.length)) {
            delete [] this->data;
        }
        this->data = new T [source.capacity];
        this->capacity = source.capacity;
        this->length = source.length;
        for (int i = 0; i < source.length; i++) {
            this->data[i] = source.data[i];
        }

    }*/

    template<typename T>
    void DynamicSequence<T>::makeDeepCopy(const DynamicSequence<T> &source) {
        //this->clear(); ENABLE TO RETURN TO PREV
        for (int i = 0; i < source.length; i++) {
            this->append(source.data[i]);
        }
    }


    /*template<typename T>
    void DynamicSequence<T>::makeDeepCopy(const DynamicSequence<T> &source) {
        if (this->capacity < source.capacity) {
            delete [] this->data;
            this->data = new T [source.capacity];
            this->capacity = source.capacity;
        } else if (!this->data) {
            this->data = new T [source.capacity];
            this->capacity = source.capacity;
        }
        this->length = source.length;
        for (int i = 0; i < source.length; i++) {
            this->data[i] = source.data[i];
        }

    }*/

    template<typename T>
    DynamicSequence<T>::DynamicSequence(const DynamicSequence<T> &source): DynamicSequence<T>() {
        this->makeDeepCopy(source);
    }

    template<typename T>
    DynamicSequence<T>::DynamicSequence(DynamicSequence<T>&& moved) {
        this->capacity = moved.capacity;
        this->length = moved.length;
        this->data = moved.data;
        moved.data = nullptr;
        //this->clear();
        //this->makeDeepCopy(moved);
        //if (moved.data) {
        //    delete [] moved.data;
        //    moved.data = nullptr;
        //}
    }

    template<typename T>
    DynamicSequence<T>& DynamicSequence<T>::operator=(const DynamicSequence<T> &source) {
        if (this != &source) {
            this->clear(); //DISABLE TO RETURN TO PREV
            this->makeDeepCopy(source);
        }
        return *this;
    }
} // CLCEngine
