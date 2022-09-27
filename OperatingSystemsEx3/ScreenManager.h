//
// Created by omer on 03/06/22.
//

#ifndef ASS3_SCREENMANAGER_H
#define ASS3_SCREENMANAGER_H

#include <chrono>
#include <unistd.h>

#include "BoundedQueue.h"

class ScreenManager {
private:
    BoundedQueue* m_queue;
    int done_numbers;
public:
    ScreenManager(BoundedQueue* queue){
        this->m_queue=queue;
        this->done_numbers=0;
    }
    /**
     * going for each data , prints if it's not done, when get three done stops
     */
    void operator ()(){
        while (done_numbers != 3){
            std::string data = this->m_queue->dequeue();
            if (data == "DONE"){
                this->done_numbers++;
                continue;
            }
            std::cout<<data<<std::endl;
        }
        std::cout<<"DONE"<<std::endl;

    }

};


#endif //ASS3_SCREENMANAGER_H

