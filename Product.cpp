#include "Product.h"
#include <iostream>
// Constructor which initializes a new product with ID, name, price, and stock quantity
Product::Product(int id, const std::string& name, double price, int qty)
    : productID(id), name(name), basePrice(price), quantity(qty)
{
}
// getters that returns the requested product's details
int Product::getProductID() const { return productID; }
std::string Product::getName() const { return name; }
double Product::getBasePrice() const { return basePrice; }
int Product::getQuantity() const { return quantity; }
// updates the products stock quantity when restocking
void Product::setQuantity(int qty) { quantity = qty; }
// reduces product quantity when product is sold
void Product::reduceQuantity(int amount)
{
    if (amount <= quantity)
        quantity -= amount;
}

double Product::calculateFinalPrice() const
{
    return basePrice; // base class does nothing
}
// prints product details in a clean format
void Product::display() const
{
    std::cout << productID << " | " << name
              << " | Base: $" << basePrice
              << " | Qty: " << quantity;
}
