#ifndef TAXABLECHOCOLATE_H
#define TAXABLECHOCOLATE_H

#include "Product.h"
#include <string>
// A chocolate product that requires tax calculation on top of its base price
// Inherits from Product and overrides only the pricing logic
class TaxableChocolate : public Product
{

public:// Constructor passes all details to the base Product class
    TaxableChocolate(int id, const std::string& name, double price, int qty);
    // Overrides Product::calculateFinalPrice()
    // Applies tax rate to the base price
    double calculateFinalPrice() const override;
};

#endif
