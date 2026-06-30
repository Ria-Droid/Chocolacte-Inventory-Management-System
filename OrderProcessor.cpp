#include "OrderProcessor.h"
#include <iostream>
#include <random>

/*
The purpose of this class is to handle creating and processing customer orders using multiple threads.
Sends each order to the InventoryManager and keeps a record of results.
Uses mutexes to safely manage shared data between threads.
*/

// Constructor links this processor to the inventory manager
OrderProcessor::OrderProcessor(InventoryManager& inv)
    : inventoryManager(inv) {}

// Generate random orders and add them to the queue
void OrderProcessor::generateOrders(int numOrders)
{
    // Random generators for product IDs and quantities
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> productDist(101, 115);
    std::uniform_int_distribution<> quantityDist(1, 60); // higher range to allow failed orders

    //Warehouse List
    std::vector<std::string> warehouses = {
       "Brussels", "Sao Paulo", "Dubai", "Durban", "New York"
    };

    //Random Warehouse Picker
    std::uniform_int_distribution<> warehouseDist(0, 4);

    for (int i = 1; i <= numOrders; i++)
    {
        int pid = productDist(gen);
        int qty = quantityDist(gen);

        //pick random warehouse
        std::string warehouse = warehouses[warehouseDist(gen)];

        // Lock queue before modifying it (thread safety)
        std::lock_guard<std::mutex> lock(queueMutex);
        orderQueue.push(Order(i, pid, qty, warehouse));
    }

    // Confirm how many orders were created
    std::cout << numOrders << " random orders generated.\n";
}

// Function executed by each thread
void OrderProcessor::workerThread(int threadID)
{
    while (true)
    {
        Order order(0, 0, 0, "");

        {
            // Safely access and remove order from queue
            std::lock_guard<std::mutex> lock(queueMutex);
            if (orderQueue.empty())
                return; // stop thread if no more orders

            order = orderQueue.front();
            orderQueue.pop();
        }

        // Process the order using inventory manager
        inventoryManager.processSingleOrder(order);

        int remainingStock = inventoryManager.getProductQuantity(order.getProductID());

        {
            // Save processed order to history
            std::lock_guard<std::mutex> lock(historyMutex);
            orderHistory.push_back(order);
        }

        {
            // Print result safely (avoid mixed output from threads)
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout
                << "Thread " << std::this_thread::get_id()
                << " processed Order #" << order.getOrderID()
                << " | Warehouse: " << order.getWarehouseLocation()
                << " | Product: " << order.getProductID()
                << " | Quantity: " << order.getQuantityRequested()
                << " -> " << order.getStatus();

                if (remainingStock >= 0)
                {
                std::cout << " | Remaining Stock: " << remainingStock;
                }
                std::cout << std::endl;
        }
    }
}

// Start multiple threads to process all orders
void OrderProcessor::processOrders(int numThreads)
{
    std::vector<std::thread> threads;

    std::cout << "\nProcessing Customer Orders using "
              << numThreads << " Threads...\n";

    // Create threads and assign worker function
    for (int i = 0; i < numThreads; i++)
        threads.emplace_back(&OrderProcessor::workerThread, this, i + 1);

    // Wait for all threads to finish
    for (auto& t : threads)
        t.join();

    // Display final results after all processing is done
    std::lock_guard<std::mutex> lock(coutMutex);

    std::cout << "\nALL ORDERS PROCESSED - FINAL ORDER STATUS:\n";
    for (const auto& order : orderHistory)
    {
        std::cout
            << "Order #" << order.getOrderID()
            << " | Product: " << order.getProductID()
            << " | Quantity: " << order.getQuantityRequested()
            << " -> " << order.getStatus() << std::endl;
    }
}
