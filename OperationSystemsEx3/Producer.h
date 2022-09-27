//
// Created by omer on 03/06/22.
//

#ifndef ASS3_PRODUCER_H
#define ASS3_PRODUCER_H


#include <string>
#include "BoundedQueue.h"

class Producer {
private:
    BoundedQueue* m_boundedQueue;
    int m_queue_number;
    std::string m_producer_name;
    int m_number_of_proucts;
    int m_sport_number;
    int m_news_number;
    int m_weather_number;
    pthread_mutex_t mutex;
public:
    Producer(BoundedQueue* boundedQueue,std::string producer_name,int number_of_proucts){
        this->m_boundedQueue = boundedQueue;
        this->m_producer_name = producer_name;
        this->m_number_of_proucts = number_of_proucts;
        this->m_news_number = 0;
        this->m_sport_number = 0;
        this->m_weather_number = 0;

    }

    void operator ()(){
        int i = 0;
        //creating number of products
        srand(time(0));
        for (i;i<m_number_of_proucts; i++){
            //generating random number
            int randNum = rand() % 3; // Generate a random number between 0 and 2
            std::string line_to_write = "Producer "+m_producer_name +" ";
            switch (randNum) {
                case 0:
                    //case news
                    line_to_write += "NEWS " + std::to_string(m_news_number);
                    m_news_number++;
                    m_boundedQueue->enqueue(line_to_write);
                    line_to_write.clear();
                    break;
                case 1:
                    //case sport
                    line_to_write += "SPORTS " + std::to_string(m_sport_number);
                    m_sport_number++;
                    m_boundedQueue->enqueue(line_to_write);
                    line_to_write.clear();
                    break;
                case 2:
                    line_to_write += "WEATHER " + std::to_string(m_weather_number);
                    m_weather_number++;
                    m_boundedQueue->enqueue(line_to_write);
                    line_to_write.clear();
                    break;
            }
        }
        //finished producing all products, writing done
        m_boundedQueue->enqueue("DONE");
    }





};


#endif //ASS3_PRODUCER_H
