#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "Product.h"
#include "Order.h"
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>

class InventoryManager
{
public:
    InventoryManager();

    // Inventory operations
    void addProduct(std::shared_ptr<Product> p); // add a product
    int removeProduct(int id); //removes a product
    void displayProducts() const; // used to display all products
    std::shared_ptr<Product> searchProduct(int id) const; //Search product by ID using STL algorithm
    void sortProducts();  // Uses STL algorithm to sort products by prices

    void sortByPrice(); //Sort by price
    void sortByQuantity(); //Sort by quantity

    // Order processing
    void processSingleOrder(Order& order); //Processes single order
    int getProductQuantity(int id) const;

private:
    std::map<int, std::shared_ptr<Product>> inventory;
    mutable std::mutex inventoryMutex;  // mutable allows locking in const methods
};

#endif
