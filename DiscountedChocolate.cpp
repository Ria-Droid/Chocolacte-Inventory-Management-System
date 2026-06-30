#include "DiscountedChocolate.h"

/*
 The purpose of this class is to calculate discounts on the sugar free products
*/

DiscountedChocolate::DiscountedChocolate(int id,
                                         const std::string& name,
                                         double price,
                                         int qty)
    : Product(id, name, price, qty)
{
}

// Calculates the final price after applying a 20% discount
double DiscountedChocolate::calculateFinalPrice() const
{
    return getBasePrice() * 0.80;
}
