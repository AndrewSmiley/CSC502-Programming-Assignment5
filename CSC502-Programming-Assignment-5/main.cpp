//
//  main.cpp
//  CSC502-Programming-Assignment5
//
//  Created by Andrew Smiley on 4/22/16.
//  Copyright © 2016 Andrew Smiley. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
struct ItemInfo{
    ItemInfo(string, string, float,float);
    ItemInfo(): SKU(""), description(""), unitPrice(0.0), shippingWeight(0.0){};
    string SKU;
    string description;
    float unitPrice;
    float shippingWeight;
};
ItemInfo::ItemInfo(string _sku, string _description, float _unitPrice, float _shippingWeight): SKU{_sku}, description{_description}, unitPrice{_unitPrice}, shippingWeight{_shippingWeight}{};
//const int MAX_CHARS_PER_LINE = 1000;

typedef map<string, ItemInfo> Catalog;
typedef map<string, int> Order;

void readCatalog(Catalog & catalog, const string & fileName) throw(runtime_error);
// reads the input file and creates the catalog; throws a
// runtime_error if the file cannot be opened
void printCatalog(const Catalog & catalog);
// prints the SKU, description, price, and weight of every item in
// the catalog
ItemInfo getItemData(const Catalog & catalog, const string & sku);
// finds a single item by SKU and returns the details as a struct;
// returns a dummy struct with the description "Item not found",
// price 0.00, and weight 0.00 if the SKU is not in the catalog
void displayOrderItems(const Order & order, const Catalog & catalog);
// Lists the SKU, description, and quantity of each type of order item
void addItem(Order & order, const Catalog & catalog, const string & sku, int quantity) throw (logic_error);
// adds item(s) to the order; throws a logic_error if the item cannot
// be found in the catalog
void removeItem(Order & order, const string & sku, int quantity) throw(logic_error) ;
// removes items(s) from the order; throws a logic_error if the item
// cannot be found in the order
void displayOrderSummary(const Order & order, const Catalog & catalog) ;
// displays the number of unique item types, the total number of
// items, the total cost, and the total shipping weight


int main(int argc, const char * argv[]) {
    Catalog catalog;
    readCatalog(catalog, "CatalogData.txt");
    printCatalog(catalog);
    return 0;
}

void readCatalog(Catalog &catalog, const string &filename) throw (runtime_error){
    // create a file-reading object
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good()){
        throw runtime_error("An error occurred opening the file..");
    }
    //        return 1; // exit if file not found
    //
    // read each line of the file
    while (!fin.eof())
    {
        // read an entire line into memory
        char buf[1000];
        //        fin.getlin
        fin.getline(buf, 1000);
        
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the tokens in buf
        const char* token[4] = {}; // initialize to 0
        
        // parse the line
        token[0] = strtok(buf, ":"); // first token
        if (token[0]) // zero if line is blank
        {
            for (n = 1; n < 4; n++)
            {
                token[n] = strtok(0, ":"); // subsequent tokens
                if (!token[n]) break; // no more tokens
            }
        }
//        ok so now we create the iteminfo stuff
        catalog[string(token[0])] = ItemInfo(string(token[0]), string(token[1]), atof(token[2]), atof(token[3]));
    }
}


void printCatalog(const Catalog &catalog){
    cout << "SKU\tDescription\tUnit Price\tShipping Weight"<<endl;
    for(auto it = catalog.begin(); it!= catalog.end(); it++){
        cout << it->second.SKU<<"\t"<<it->second.description<<"\t"<<it->second.unitPrice<<"\t"<<it->second.shippingWeight<<endl;
    }
    
}










