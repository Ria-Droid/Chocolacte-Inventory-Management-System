#include "TaxableChocolate.h"

// Constructor that forwards all arguments to the base Product constructor
TaxableChocolate::TaxableChocolate(int id,
                                   const std::string& name,
                                   double price,
                                   int qty)
    : Product(id, name, price, qty)
{
}

// 15% tax is applied to the chocolate's base price
// This overrides the base class version
double TaxableChocolate::calculateFinalPrice() const
{
    double tax = 0.15;
    return getBasePrice() * (1.0 + tax);
}
