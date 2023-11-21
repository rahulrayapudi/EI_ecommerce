#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm> 

class Product {
public:
    virtual ~Product() {}
    virtual std::string getName() const = 0;
    virtual double getPrice() const = 0;
    virtual bool isAvailable() const = 0;
    virtual std::string toString() const = 0;
    virtual Product* clone() const = 0;
};

class Laptop : public Product {
public:
    Laptop(std::string name, double price)
        : name(name), price(price) {}

    std::string getName() const override {
        return name;
    }

    double getPrice() const override {
        return price;
    }

    bool isAvailable() const override {
        return true; 
    }

    std::string toString() const override {
        return name + " - Price: $" + std::to_string(price);
    }

    Product* clone() const override {
        return new Laptop(*this);
    }

private:
    std::string name;
    double price;
};

class Headphones : public Product {
public:
    Headphones(std::string name, double price)
        : name(name), price(price) {}

    std::string getName() const override {
        return name;
    }

    double getPrice() const override {
        return price;
    }

    bool isAvailable() const override {
        return true; 
    }

    std::string toString() const override {
        return name + " - Price: $" + std::to_string(price);
    }

    Product* clone() const override {
        return new Headphones(*this);
    }

private:
    std::string name;
    double price;
};

class ShoppingCart {
public:
    void add_item(Product* product, int quantity) {
        if (product->isAvailable()) {
            items.push_back({product->clone(), quantity});
        }
    }

    
void update_quantity(std::string product_name, int new_quantity) {
    std::transform(product_name.begin(), product_name.end(), product_name.begin(), ::tolower);

    bool found = false;
    for (auto& item : items) {
        std::string stored_name = item.first->getName();
        std::transform(stored_name.begin(), stored_name.end(), stored_name.begin(), ::tolower);

        if (stored_name == product_name) {
            item.second = new_quantity;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Product not found in the cart." << std::endl;
    }
}


void remove_item(std::string product_name) {
    product_name.erase(0, product_name.find_first_not_of(" \t"));
    product_name.erase(product_name.find_last_not_of(" \t") + 1);

    std::transform(product_name.begin(), product_name.end(), product_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    auto it = std::remove_if(
        items.begin(), items.end(),
        [&product_name](const std::pair<Product*, int>& item) {
            std::string stored_name = item.first->getName();

            stored_name.erase(0, stored_name.find_first_not_of(" \t"));
            stored_name.erase(stored_name.find_last_not_of(" \t") + 1);
            std::transform(stored_name.begin(), stored_name.end(), stored_name.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            return stored_name == product_name;
        });

    if (it != items.end()) {
        delete it->first; 
        items.erase(it, items.end()); 
    } else {
        std::cout << "Product not found in the cart." << std::endl;
    }
}



    double calculate_total() const {
        double total = 0;
        for (const auto& item : items) {
            total += (item.first->getPrice() * item.second);
        }
        return total;
    }

    ~ShoppingCart() {
        for (auto& item : items) {
            delete item.first;
        }
        items.clear();
    }

private:
    std::vector<std::pair<Product*, int>> items;
};

int main() {
    Laptop laptop("Laptop", 1000);
    Headphones headphones("Headphones", 50);

    ShoppingCart cart;
   while (true) {
        std::cout << "\nAvailable Products:" << std::endl;
        std::cout << laptop.toString() << std::endl;
        std::cout << headphones.toString() << std::endl;

        std::string user_input;
        std::cout << "Enter a command (add/update/remove/bill/exit): ";
        std::cin >> user_input;

        if (user_input == "add") {
            std::string product_name;
            int quantity;
            std::cout << "Enter the product name to add: ";
            std::cin >> product_name;
            std::cout << "Enter the quantity: ";
            std::cin >> quantity;

            Product* product = nullptr;
            if (product_name == "Laptop" || product_name == "laptop") {
                product = laptop.clone();
            } else if (product_name == "Headphones" || product_name == "headphones") {
                product = headphones.clone();
            } else {
                std::cout << "Invalid product name." << std::endl;
                continue;
            }
            cart.add_item(product, quantity);
        } else if (user_input == "update") {
            std::string product_name;
            int new_quantity;
            std::cout << "Enter the product name to update: ";
            std::cin >> product_name;
            std::cout << "Enter the new quantity: ";
            std::cin >> new_quantity;
            cart.update_quantity(product_name, new_quantity);
        } else if (user_input == "remove") {
            std::string product_name;
            std::cout << "Enter the product name to remove: ";
            std::cin >> product_name;
            cart.remove_item(product_name);
        } else if (user_input == "bill") {
            std::cout << "Total Bill: Your total bill is $" << cart.calculate_total() << "." << std::endl;
        } else if (user_input == "exit") {
            break;
        } else {
            std::cout << "Invalid command. Please enter a valid command." << std::endl;
        }
    }

    return 0;
}