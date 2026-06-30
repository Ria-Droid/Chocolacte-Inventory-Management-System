#include "InventoryManager.h"
#include <algorithm>
#include <iostream>

/*
The purpose of the InventoryManager is to manage all products in the inventory and to
process orders safely in a multi threaded environment.Allowing for thread safe
operations for adding,removing,searching,displaying,sorting the products
and updating order status

*/

//Constructor
InventoryManager::InventoryManager() {}

//Add a product to inventory
void InventoryManager::addProduct(std::shared_ptr<Product> p)
{
    //Lock mutex to prevent concurrent modification of inventory
    std::lock_guard<std::mutex> lock(inventoryMutex);

    if (p)
    {
    inventory[p->getProductID()] = p;

    std::cout << "ID: " << p->getProductID()
              << " | " << p->getName()
              << " | Base: $" << p->getBasePrice()
              << " | Final: $" << p->calculateFinalPrice()
              << " | Qty: " << p->getQuantity()
              << std::endl;
    }
}

//Remove product by ID
int InventoryManager::removeProduct(int id)
{
    //Lock mutex to ensure thread safe removal
    std::lock_guard<std::mutex> lock(inventoryMutex);

    //Find the product in inventory
    auto it = inventory.find(id);
    if (it != inventory.end())
    {
        //This displays the product being removed
        std::cout << "Removing product: " << it->second->getName()
                  << " (ID: " << id << ")\n";
        inventory.erase(it);
        return 1;  // Success
    }

    std::cout << "Product ID " << id << " not found.\n";
    return 0;  //Not found
}

//Display all products
void InventoryManager::displayProducts() const
{
    //Lock mutex to get a consistent view of inventory
    std::lock_guard<std::mutex> lock(inventoryMutex);

    if (inventory.empty())
    {
        std::cout << "Inventory is empty.\n";
        return;
    }

    std::cout << "\nCurrent Inventory:\n";
    std::cout << "------------------\n";
    for (const auto& item : inventory)
    {
        //Display product details including final price
        std::cout << "ID: " << item.second->getProductID()
                  << " | " << item.second->getName()
                  << " | Base: $" << item.second->getBasePrice()
                  << " | Final: $" << item.second->calculateFinalPrice()
                  << " | Qty: " << item.second->getQuantity()
                  << std::endl;
    }
    std::cout << "------------------\n";
    std::cout << "Total products: " << inventory.size() << "\n\n";
}

//Search product by ID using STL algorithm
std::shared_ptr<Product> InventoryManager::searchProduct(int id) const
{
    //Lock mutex to prevent inventory changes during search
    std::lock_guard<std::mutex> lock(inventoryMutex);

    //Using STL algorithm find_if instead of maps find to demonstrate our STL usage
    auto it = std::find_if(inventory.begin(), inventory.end(),
        [id](const std::pair<const int, std::shared_ptr<Product>>& pair) {
            return pair.first == id;
        });

    if (it == inventory.end())
    {
       std::cout << "Product ID " << id << " is available (not in use yet)\n";
        return nullptr;
    }

    //Product found then display details
    std::cout << "\nProduct Found:\n";
    std::cout << "ID: " << it->second->getProductID()
              << "\nName: " << it->second->getName()
              << "\nBase Price: $" << it->second->getBasePrice()
              << "\nFinal Price: $" << it->second->calculateFinalPrice()
              << "\nQuantity: " << it->second->getQuantity() << "\n\n";

    return it->second;
}

//Sort products by price (using STL algorithm)
void InventoryManager::sortProducts()
{
    //Lock mutex to get a consistent snapshot for sorting
    std::lock_guard<std::mutex> lock(inventoryMutex);

    if (inventory.empty()) {
        std::cout << "Inventory is empty. Nothing to sort.\n";
        return;
    }

    //Copy map values to vector for sorting
    std::vector<std::shared_ptr<Product>> productList;
    productList.reserve(inventory.size());

    for (const auto& pair : inventory) {
        productList.push_back(pair.second);
    }

    //Use STL sort algorithm to sort by price
    std::sort(productList.begin(), productList.end(),
              [](const std::shared_ptr<Product>& a, const std::shared_ptr<Product>& b) {
                  return a->getBasePrice() < b->getBasePrice();
              });

    //Display sorted products
    std::cout << "\nProducts Sorted by Price (Lowest to Highest):\n";
    std::cout << "--------------------------------------------\n";
    for (const auto& p : productList) {
        std::cout << "ID: " << p->getProductID()
                  << " | " << p->getName()
                  << " | $" << p->getBasePrice()
                  << " (Final: $" << p->calculateFinalPrice() << ")"
                  << " | Qty: " << p->getQuantity()
                  << std::endl;
    }
    std::cout << "--------------------------------------------\n";
}

//Sort by price
void InventoryManager::sortByPrice()
{
    sortProducts();
}

// Sort by quantity
void InventoryManager::sortByQuantity()
{
    // Lock mutex for sorting
    std::lock_guard<std::mutex> lock(inventoryMutex);

    if (inventory.empty()) {
        std::cout << "Inventory is empty. Nothing to sort.\n";
        return;
    }

    //Copy map values to vector for sorting
    std::vector<std::shared_ptr<Product>> productList;
    productList.reserve(inventory.size());

    for (const auto& pair : inventory) {
        productList.push_back(pair.second);
    }

    std::sort(productList.begin(), productList.end(),
              [](const std::shared_ptr<Product>& a, const std::shared_ptr<Product>& b) {
                  return a->getQuantity() < b->getQuantity();
              });

    //Display sorted products
    std::cout << "\nProducts Sorted by Quantity (Lowest to Highest):\n";
    std::cout << "----------------------------------------------\n";
    for (const auto& p : productList) {
        std::cout << "ID: " << p->getProductID()
                  << " | " << p->getName()
                  << " | Qty: " << p->getQuantity()
                  << " | Price: $" << p->getBasePrice()
                  << " (Final: $" << p->calculateFinalPrice() << ")"
                  << std::endl;
    }
    std::cout << "----------------------------------------------\n";
}

//Process a single order
void InventoryManager::processSingleOrder(Order& order)
{
    //Lock mutex to ensure atomic check and update operation
    std::lock_guard<std::mutex> lock(inventoryMutex);

    //Find product
    auto it = inventory.find(order.getProductID());

    //Case 1: Invalid quantity
    if (order.getQuantityRequested() <= 0)
    {
        order.setStatus("FAILED - INVALID QUANTITY");
        return;
    }

    //Case 2: Product doesn't exist
    if (it == inventory.end())
    {
        order.setStatus("FAILED - PRODUCT NOT FOUND");
        return;
    }


    //Case 3: Insufficient stock
    if (it->second->getQuantity() < order.getQuantityRequested())
    {
        order.setStatus("FAILED - INSUFFICIENT STOCK");
        return;
    }

    //Case 4: Success: reduce stock and mark order as successful
    it->second->reduceQuantity(order.getQuantityRequested());
    order.setStatus("SUCCESS");

}

//Get product quantity by ID
int InventoryManager::getProductQuantity(int id) const
{
    std::lock_guard<std::mutex> lock(inventoryMutex);

    auto it = inventory.find(id);
    if (it != inventory.end())
        return it->second->getQuantity();

    return -1; //Product not found
}
