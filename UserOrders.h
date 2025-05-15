#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class UserOrders {
public:
    // Constructor
    UserOrders(const std::string& name);

    // Structs to create group of variables into one place
    struct Items {
        std::string itemName;
        int count;
    };

    // Getters
    std::string getBuyerName() const;
    std::string getBuyerRealName() const;

    // Setters
    void setBuyerRealName(const std::string name);

    void addAuctionGroup(const std::string& groupName);
    void addNumToAuctionGroup(const std::string& groupName, int item);

    // Add items to auction arrays
    void addToAucNumberItems(int item);
    void addToAucTextItems(const std::string& item);

    // Get auction arrays
    std::vector<int> getGroupAuctionNumbers(const std::string& groupName) const;
    std::unordered_map<std::string, std::vector<int>> getAllAuctionGroups() const;

    std::vector<int> getAucNumberItems() const;
    std::vector<Items> getAucTextItems() const;

    void merge(std::vector<int>& arr, int left, int mid, int right);
    void mergeSort(std::vector<int>& arr, int left, int right);
    void sortAucNumberItems();
    void mergeItems(std::vector<Items>& arr, int left, int mid, int right);
    void mergeSortItems(std::vector<Items>& arr, int left, int right);

    void sortAll();

private:
    std::string buyerName;
    std::string buyerRealName;
    std::unordered_map<std::string, std::vector<int>> aucNumberItemsGroups;
    std::vector<int> aucNumberItems;
    std::vector<Items> aucTextItems;

};