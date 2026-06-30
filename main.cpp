#include <iostream>
#include <memory>
#include <limits>

#include "InventoryManager.h"
#include "Product.h"
#include "TaxableChocolate.h"
#include "DiscountedChocolate.h"
#include "OrderProcessor.h"

using namespace std;


// Input buffer helper
void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// This is the menu that will be displayed
void displayMenu()
{
    cout << "\n=====================================\n";
    cout << "  GLOBAL CHOCOLATE DISTRIBUTOR SYSTEM\n";
    cout << "=====================================\n";
    cout << "1. Add new product\n";
    cout << "2. Remove product\n";
    cout << "3. Display products\n";
    cout << "4. Search product\n";
    cout << "5. Sort products\n";
    cout << "6. Process orders (multi-threaded)\n";
    cout << "0. Exit\n";
    cout << "=====================================\n";
    cout << "Enter choice: ";
}

// initialises the default inventory of all products
void initializeDefaultInventory(InventoryManager& inventory)
{
   inventory.addProduct(make_shared<TaxableChocolate>(101, "Belgian Dark Chocolate", 5.0, 50));
inventory.addProduct(make_shared<TaxableChocolate>(102, "Milk Chocolate Almond", 5.0, 40));
inventory.addProduct(make_shared<TaxableChocolate>(103, "Cookies & Cream", 6.0, 30));
inventory.addProduct(make_shared<TaxableChocolate>(104, "Dark Chocolate Gift Box", 15.0, 60));
inventory.addProduct(make_shared<TaxableChocolate>(105, "Luxury Assorted Box", 20.0, 45));
inventory.addProduct(make_shared<TaxableChocolate>(106, "Swiss Chocolate Collection", 25.0, 80));
inventory.addProduct(make_shared<TaxableChocolate>(107, "Dark Ganache", 15.0, 35));
inventory.addProduct(make_shared<TaxableChocolate>(108, "Hazelnut Box", 15.0, 55));

    // sugar-free (discounted) chocolates
    inventory.addProduct(make_shared<DiscountedChocolate>(109, "Sugar-Free Macarons", 20.0, 25));
    inventory.addProduct(make_shared<DiscountedChocolate>(110, "Sugar-Free Truffles", 10.0, 30));
    inventory.addProduct(make_shared<DiscountedChocolate>(111, "Sugar-Free Peanut Bar", 3.0, 100));
    inventory.addProduct(make_shared<DiscountedChocolate>(112, "Sugar-Free Pistachio Bonbons", 25.0, 65));
    inventory.addProduct(make_shared<DiscountedChocolate>(113, "Sugar-Free Caramel Bar", 5.0, 85));
    inventory.addProduct(make_shared<DiscountedChocolate>(114, "Sugar-Free Dark Box", 18.0, 40));
    inventory.addProduct(make_shared<DiscountedChocolate>(115, "Sugar-Free Chocolate Heart", 15.0, 35));

    cout << "Default inventory loaded (15 products).\n";
}

int main()
{
    InventoryManager inventory;
    initializeDefaultInventory(inventory);

    int choice;
    bool running = true;

    while (running)
    {
        displayMenu();
        cin >> choice;

        if (cin.fail())
        {
            clearInputBuffer();
            cout << "Invalid input.\n";
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            // EXIT
            case 0:
                cout << "Exiting system...\n";
                running = false;
                break;

            // Option to add a new product
            case 1:
            {
                cout << "\n--- ADD NEW PRODUCT ---\n";

                int id, qty;
                string name;
                double price;
                int type;

                cout << "1 = Normal (Taxed)\n";
                cout << "2 = Sugar-Free (Discounted)\n";
                cout << "Enter type: ";
                cin >> type;
                clearInputBuffer();
                if (type != 1 && type != 2)
            {
                cout << "Invalid type. Please enter 1 (Normal) or 2 (Sugar-Free).\n"; // ensures that the user enters a valid option
                break;
            }

                cout << "Enter Product ID: ";
                cin >> id;
                clearInputBuffer();

               if (inventory.searchProduct(id))
               {
                cout << "Product ID already exists.Please enter a unique ID\n"; // ensures that the user enters a valid and unique ID
                break;
               }

                cout << "Enter Product Name: ";
                getline(cin, name);

              cout << "Enter Base Price: ";
              cin >> price;

              // checks if input is not a number
              if (cin.fail())
            {
              clearInputBuffer();
              cout << "Invalid input. Please enter a numeric price.\n";
             break;
             }

             // checks if price is 0 or negative
             if (price <= 0)
            {
            cout << "Price must be greater than 0. Product not added.\n";
            break;
            }

                cout << "Enter Quantity: ";
                cin >> qty;

                if (qty <= 0)
{
               cout << "Invalid quantity.Quantity cannot be 0 .Product not added.\n";
                break;
}

clearInputBuffer();
                shared_ptr<Product> newProduct;

                if (type == 2)
                {
                    // if products are sugar-free, then the discount is applied
                    newProduct = make_shared<DiscountedChocolate>(id, name, price, qty);
                    cout << "Sugar-free product added (discount applied).\n";
                }
                else
                {
                    // if product is not sugar-free, then tax is applied
                    newProduct = make_shared<TaxableChocolate>(id, name, price, qty);
                    cout << "Normal product added (tax applied).\n";
                }

                inventory.addProduct(newProduct);

                cout << "Final price: $" << newProduct->calculateFinalPrice() << endl;

                break;
            }
            // removes a product by ID
            case 2:
            {
                int id;
                cout << "Enter Product ID: ";
                cin >> id;

                inventory.removeProduct(id);
                break;
            }
            // displays the products in inventory
            case 3:
                inventory.displayProducts();
                break;
            // searches for a product by ID
            case 4:
            {
                int id;
                cout << "Enter Product ID: ";
                cin >> id;

                inventory.searchProduct(id);
                break;
            }


               // sorts products by either price or quantity
               case 5:
{
    int sortChoice;

    cout << "\nSort by:\n";
    cout << "1. Price\n";
    cout << "2. Quantity\n";
    cout << "Enter choice: ";
    cin >> sortChoice;

    if (sortChoice == 1)
    {
        inventory.sortProducts();
    }
    else if (sortChoice == 2)
    {
        inventory.sortByQuantity();
    }
    else
    {
        cout << "Invalid sort option.\n";
    }

    break;
}

            // processes our concurrent orders
            case 6:
            {
                int numOrders;

                cout << "Enter number of orders: ";
                cin >> numOrders;

                // We use exactly 5 threads to simulate 5 warehouse locations (Brussels, Sao Paulo, Dubai, Durban, New York)
                int numThreads = 5;
                std::cout << "Processing orders using 5 warehouse workers...\n";

                OrderProcessor processor(inventory);
                processor.generateOrders(numOrders);
                processor.processOrders(numThreads);

                break;
            }

            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}
