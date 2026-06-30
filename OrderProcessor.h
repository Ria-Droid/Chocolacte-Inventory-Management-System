#ifndef ORDERPROCESSOR_H
#define ORDERPROCESSOR_H

#include "InventoryManager.h"
#include "Order.h"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

/*
The purpose of this class is to handle generating and processing orders using multiple threads.
Works with InventoryManager to process each order and keeps track of history.
*/

class OrderProcessor
{
public:
    // Constructor links the processor to the inventory manager
    OrderProcessor(InventoryManager& manager);

    // Creates a number of random orders and adds them to the queue
    void generateOrders(int numOrders);

    // Starts multiple threads to process orders from the queue
    void processOrders(int numThreads);

private:
    // Function each thread runs to process orders from the queue
    void workerThread(int threadID);

    // Reference to inventory manager (used to process orders)
    InventoryManager& inventoryManager;

    // Queue storing orders waiting to be processed
    std::queue<Order> orderQueue;

    // Stores all processed orders
    std::vector<Order> orderHistory;

    // Mutex to protect access to the order queue
    std::mutex queueMutex;

    // Mutex to protect access to order history
    std::mutex historyMutex;

    // Mutex to prevent mixed console output from multiple threads
    std::mutex coutMutex;
};

#endif
