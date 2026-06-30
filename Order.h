#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

#include <string>
#include <iostream>

/*
 The purpose of this class is to represent a single order placed by a warehouse for a specific chocolate product.

 Each order stores:
 The orderID
 The productID being requested
 The quantity requested
 The warehouse location requesting the order
 The order status: Pending, Completed or Failed
*/

class Order
{
private:
    int orderID;
    int productID;
    int quantityRequested;
    std::string warehouseLocation;
    std::string status;   //Pending, Completed or Failed

public:

    //Create a new order
    Order(int id, int pID, int qty, const std::string& warehouse);

    int getOrderID() const;//returns the order ID

    int getProductID() const; //returns the product ID

    int getQuantityRequested() const; //returns the quantity requested

    std::string getWarehouseLocation() const; //returns the warehouse location

    std::string getStatus() const; //returns the current status of the order

    void setStatus(const std::string& s); //updates the order status

    void displayOrder() const;//prints out the order on console
};

#endif // ORDER_H_INCLUDED
