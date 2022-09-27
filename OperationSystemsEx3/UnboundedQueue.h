//
// Created by omer on 03/06/22.
//

#ifndef ASS3_UNBOUNDEDQUEUE_H
#define ASS3_UNBOUNDEDQUEUE_H


#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <pthread.h>

class UnboundedQueue {
private:
    std::queue<std::string>* m_queue;
    sem_t m_full;
    pthread_mutex_t m_mutex;
public:
    UnboundedQueue(){
        m_queue = new std::queue<std::string>;
        sem_init(&m_full,0,0);
    }
    /**
     * push to the queue
     */
    void enqueue(const std::string& data) {
        //entering critical section
        pthread_mutex_lock(&m_mutex);
        m_queue->push(data);
        //leaving critical section
        pthread_mutex_unlock(&m_mutex);
        sem_post(&m_full);
    }
    /**
     * pop from the queue
     * @return the last value.
     */
    std::string dequeue(){
        sem_wait(&m_full);
        pthread_mutex_lock(&m_mutex);
        //entering critical section
        std::string return_value =  this->m_queue->front();
        this->m_queue->pop();
        //leaving critical section
        pthread_mutex_unlock(&m_mutex);
        return return_value;
    }
};


#endif //ASS3_UNBOUNDEDQUEUE_H
