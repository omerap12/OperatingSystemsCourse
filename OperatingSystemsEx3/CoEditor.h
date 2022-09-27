//
// Created by omer on 03/06/22.
//

#ifndef ASS3_COEDITOR_H
#define ASS3_COEDITOR_H


#include <unistd.h>
#include "UnboundedQueue.h"
#include "BoundedQueue.h"

class CoEditor {
private:
    //gets from the dispatcher
    UnboundedQueue* m_unboundedQueue;
    //go to the screen manager
    BoundedQueue* m_boundedQueue;


public:
    CoEditor(UnboundedQueue* unboundedQueue, BoundedQueue* boundedQueue){
        this->m_boundedQueue = boundedQueue;
        this->m_unboundedQueue = unboundedQueue;
    }

    /**
     * getting data from the unbounded queue if gets done message break. else wait for 0.1 seconds and deliver to data
     */
    void operator ()(){
        while (1){
            //gets the data from the dispatcher queue
            std::string data = m_unboundedQueue->dequeue();
            if (data == "DONE"){
                m_boundedQueue->enqueue(data);
                break;
            }
            sleep(0.1);
            //put into the screen manager
            m_boundedQueue->enqueue(data);
        }
    }



};


#endif //ASS3_COEDITOR_H
