#include "Order.h"
#include <iostream>

/*
    This class represents a customer order in the system, it keeps track of important details such as the order ID,
    product ID, quantity requested, which warehouse is handling the order, as well as the current status of the order.
    This class also provides functions to access this data, update the order status, and display the order information.
*/

//The constructor initializes the order attributes and sets the default order status to "Pending"
Order::Order(int id, int pID, int qty, const std::string& warehouse)
{
    orderID = id;                     // set the order ID
    productID = pID;                  // set the product ID
    quantityRequested = qty;          // store how many items were requested
    warehouseLocation = warehouse;    // store which warehouse will process the order
    status = "Pending";               // all orders start as Pending
}

//Returns the order ID
int Order::getOrderID() const
{
    return orderID;
}

//Returns the product ID
int Order::getProductID() const
{
    return productID;
}

//Returns  how many items were requested
int Order::getQuantityRequested() const
{
    return quantityRequested;
}

//Returns the warehouse location
std::string Order::getWarehouseLocation() const
{
    return warehouseLocation;
}

//Returns the current status of the order
std::string Order::getStatus() const
{
    return status;
}

//Updates the order status during order processing (For example: Pending to Completed or Failed)
//Order objects are processed by multiple threads in OrderProcessor
void Order::setStatus(const std::string& s)
{
    status = s;  //Pending/Completed/Failed
}

//Prints all the order details in a readable format
void Order::displayOrder() const
{
    std::cout << "Order ID: " << orderID
              << " Product ID: " << productID
              << " Quantity: " << quantityRequested
              << " Warehouse: " << warehouseLocation
              << " Status: " << status
              << std::endl;
}

