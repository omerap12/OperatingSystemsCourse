#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "BoundedQueue.h"
#include "UnboundedQueue.h"
#include "Producer.h"
#include "Dispatcher.h"
#include "CoEditor.h"
#include "ScreenManager.h"
#include <thread>




int main(int argc, char* argv[]) {
    /**
     * These are shared memory
     */
    std::vector<BoundedQueue*> bounded_queues;
    std::vector<UnboundedQueue*> unbounded_queues;
    unbounded_queues.push_back(new UnboundedQueue());
    unbounded_queues.push_back(new UnboundedQueue());
    unbounded_queues.push_back(new UnboundedQueue());
    /**
     * end of shared memory
     */


    std::vector<Producer*> producers;
    /**
     * reading from the conf file
     */
     std::ifstream file(argv[1]);
     std::string str;
     std::string producer_number;
     std::string producer_items_count;
     std::string producer_queue_size;
     std::string Co_Editor_queue_size;
     int index_for_bounded_queues = 0;
     int index = 0;


     /**
      * reading every three lines, store the info in temp string objects
      */
    while (std::getline(file,str))
     {
        Co_Editor_queue_size = str;
        if (index > 2){
            //creating producers and queues

            //creating bounded queues
            bounded_queues.push_back(new BoundedQueue(std::stoi(producer_queue_size)));
            //creating the producers
            int number_of_products = std::stoi(producer_items_count);
            producers.push_back(new Producer(bounded_queues[index_for_bounded_queues],producer_number, number_of_products));
            index = 0;
            index_for_bounded_queues++;
            continue;
        }
        if (index == 0){
            producer_number = str;
        } else if (index == 1){
            producer_items_count = str;
        }else{
            producer_queue_size = str;
        }
        index++;
     }
    //creating the bounded queue for the screen manager
    BoundedQueue* screen_manger_queue = new BoundedQueue(std::stoi(Co_Editor_queue_size));
    ScreenManager* screenManager = new ScreenManager(screen_manger_queue);



    //creating the dispatcher
    Dispatcher* dispatcher = new Dispatcher(unbounded_queues,bounded_queues,index_for_bounded_queues);



//    creating the Co-Editors
    CoEditor* co_one = new CoEditor(unbounded_queues[0],screen_manger_queue);
    CoEditor* co_two = new CoEditor(unbounded_queues[1],screen_manger_queue);
    CoEditor* co_three = new CoEditor(unbounded_queues[2],screen_manger_queue);


    //TODO: add all the other threads
    std::vector<std::thread> threads;

    //creating producers thread
    for (int i = 0; i<index_for_bounded_queues; i++){
        threads.push_back(std::thread(*producers[i]));
    }

    //adding the dispatcher to threads list
    threads.push_back(std::thread(*dispatcher));


//    //add the co-Editors to the threads
    threads.push_back(std::thread(*co_one));
    threads.push_back(std::thread(*co_two));
    threads.push_back(std::thread(*co_three));
//
    //adding the Screen manager to the threads
    threads.push_back(std::thread(*screenManager));

    //join for all threads
    for (auto& thred:threads){
        thred.join();
    }

    return 0;
}
