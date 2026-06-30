#ifndef DISCOUNTEDCHOCOLATE_H
#define DISCOUNTEDCHOCOLATE_H

#include "Product.h"

class DiscountedChocolate : public Product
{
public:
    DiscountedChocolate(int id, const std::string& name, double price, int qty);

    double calculateFinalPrice() const override; //Applies 20% discount
};

#endif
