//
// Created by omer on 03/06/22.
//

#ifndef ASS3_BOUNDEDQUEUE_H
#define ASS3_BOUNDEDQUEUE_H


#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <pthread.h>


class BoundedQueue{
private:
    std::queue<std::string>* m_queue;
    sem_t m_full;
    sem_t m_empty;
    pthread_mutex_t m_mutex;


public:
    /**
     * constructor
     * @param boundSize - the size of the queue
     */
    BoundedQueue(int boundSize){
        m_queue = new std::queue<std::string>;
        sem_init(&m_full,0,0);
        sem_init(&m_empty,0,boundSize);
    }
    /**
     * push to the queue
     */
    void enqueue(const std::string& data) {
        //waiting for empty to be open -> not zero
        sem_wait(&m_empty);
        //entering critical section
        pthread_mutex_lock(&m_mutex);
        //increment semaphore full
        m_queue->push(data);
        pthread_mutex_unlock(&m_mutex);

        sem_post(&m_full);
        //leaving critical section
    }
    /**
     * pop from the queue
     * @return the last value.
     */
    std::string dequeue(){
        //waiting for the full to be open -> be less then 10
        sem_wait(&m_full);
        pthread_mutex_lock(&m_mutex);
        //entering critical section
        std::string return_value =  this->m_queue->front();
        this->m_queue->pop();
        //increment semaphore empty
        //leaving critical section
        pthread_mutex_unlock(&m_mutex);
        sem_post(&m_empty);
        return return_value;
    }





};


#endif //ASS3_BOUNDEDQUEUE_H
