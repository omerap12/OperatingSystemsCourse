//
// Created by omer on 03/06/22.
//

#ifndef ASS3_DISPATCHER_H
#define ASS3_DISPATCHER_H


#include <vector>
#include <algorithm>

#include "BoundedQueue.h"
#include "UnboundedQueue.h"

class Dispatcher {
private:
    std::vector<BoundedQueue*> m_bounded_queues;
    //0 news , 1 sport , 2 weather
    std::vector<UnboundedQueue*> m_unbounded_queues;
    int m_number_queues;
    int m_done_messages;
    std::vector<int> m_done_queues;
public:
    Dispatcher(std::vector<UnboundedQueue*>& unbounded_queues,std::vector<BoundedQueue*>& bounded_queues, int number_of_queues){
        this->m_unbounded_queues = unbounded_queues;
        this->m_bounded_queues = bounded_queues;
        this->m_number_queues = number_of_queues;
        this->m_done_messages = 0;
    }
    /**
     * cyclic round robin and put in each queue
     */
    void operator ()(){
        int turn = 0;
        while(1) {
            turn = turn % m_number_queues;
            if (m_done_messages == m_number_queues) {
                for (int i = 0; i < 3; i++) {
                    this->m_unbounded_queues[i]->enqueue("DONE");
                }
                break;
            }
            //check if this producer finished already, continue if so
            if (std::find(m_done_queues.begin(), m_done_queues.end(), turn) != m_done_queues.end()) {
                turn++;
            }else{
                std::string data = this->m_bounded_queues[turn]->dequeue();
                if (data == "DONE"){
                    this->m_done_messages+=1;
                    this->m_done_queues.push_back(turn);
                    turn++;
                }else{
                    if (data.find("NEWS")!= std::string::npos){
                        this->m_unbounded_queues[0]->enqueue(data);
                    }else if (data.find("SPORTS")!= std::string::npos){
                        this->m_unbounded_queues[1]->enqueue(data);
                    }else{
                        this->m_unbounded_queues[2]->enqueue(data);
                    }
                    turn++;
                }


            }



        }
    }

};


#endif //ASS3_DISPATCHER_H
