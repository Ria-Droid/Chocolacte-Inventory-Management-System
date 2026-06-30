#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product
{
protected: // allows derived classes to access them
    int productID;
    std::string name;
    double basePrice;
    int quantity;

public:
    //Constructor that creates a product with given ID, name, price, and starting quantity
    Product(int id, const std::string& name, double price, int qty);
    virtual ~Product() = default; // Virtual destructor ensures proper cleanup of derived class objects
    // getters providing read-only access to product details
    int getProductID() const;
    std::string getName() const;
    double getBasePrice() const;
    int getQuantity() const;
    //setters that actually modify product state
    void setQuantity(int qty);
    void reduceQuantity(int amount);
    // virtual methods meant to be overridden by derived classes
    virtual double calculateFinalPrice() const;
    virtual void display() const;
};

#endif
