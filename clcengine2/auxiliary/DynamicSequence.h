//
// Created by EgrZver on 03.10.2023.
//
#include <iostream>
#include <clcengine2/movefinder/Move.h>

#ifndef CLCENGINE2_DYNAMICSEQUENCE_H
#define CLCENGINE2_DYNAMICSEQUENCE_H

namespace CLCEngine {


    template <typename T>
    class DynamicSequence {
        private:
            T *data;
            int length, capacity;

            void makeDeepCopy(const DynamicSequence<T> &source);
        public:
            DynamicSequence();
            DynamicSequence(const DynamicSequence<T>& source);
            DynamicSequence(DynamicSequence<T>&& moved);

            ~DynamicSequence();
            void append(T element);
            void pop();
            int getLength();
            T getElement(int index);
            void copyFrom(DynamicSequence<T>* source);
            void clear();

            DynamicSequence<T>& operator=(const DynamicSequence<T>& source);
    };

    //template class DynamicSequence<int>; // TEST
    template class DynamicSequence<Move>;
    template class DynamicSequence<DynamicSequence<Move>>;
    template class DynamicSequence<std::string>;

    typedef DynamicSequence<Move> MoveList;
    typedef DynamicSequence<MoveList> MoveSequencesList;

} // CLCEngine

#endif //CLCENGINE2_DYNAMICSEQUENCE_H
