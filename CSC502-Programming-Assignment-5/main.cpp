/*
 Andrew Smiley
 CSC 502 Programming Assignment 5
 i/o, error handling and STL
 Basic POS System (Somewhat)
 */

#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>

using namespace std;


struct ItemInfo{
    //just setting up some initializer list stuff
    ItemInfo(string, string, float,float);
    ItemInfo(): SKU(""), description("Item not found"), unitPrice(0.0), shippingWeight(0.0){};
    string SKU;
    string description;
    float unitPrice;
    float shippingWeight;
};

ItemInfo::ItemInfo(string _sku, string _description, float _unitPrice, float _shippingWeight): SKU{_sku}, description{_description}, unitPrice{_unitPrice}, shippingWeight{_shippingWeight}{};

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
    /*
     Just some test cases
     */
    Catalog catalog;
    readCatalog(catalog, "CatalogData.txt");
    printCatalog(catalog);
    
    Order order;
    try {
        addItem(order, catalog, "4123BLA", 5);
        displayOrderItems(order, catalog);
        displayOrderSummary(order, catalog);
    } catch (logic_error e) {
        cout <<e.what()<<endl<<endl;
    }

    try {
        addItem(order, catalog, "4123BLA", 5);
        displayOrderItems(order, catalog);
        displayOrderSummary(order, catalog);
    } catch (logic_error &e) {
        cout <<e.what()<<endl<<endl;
    }
    try {
        addItem(order, catalog, "asd", 5);
        displayOrderItems(order, catalog);
        displayOrderSummary(order, catalog);
    } catch (logic_error &e) {
        cout <<e.what()<<endl<<endl;
    }
    try {
        removeItem(order, "4123BLA", 3);
        displayOrderItems(order, catalog);
        displayOrderSummary(order, catalog);
    } catch (logic_error &e) {
        cout <<e.what()<<endl<<endl;
    }
    try {
        removeItem(order, "abd", 3);
        displayOrderItems(order, catalog);
        displayOrderSummary(order, catalog);
    } catch (logic_error &e) {
        cout <<e.what()<<endl<<endl;
    }
    
    ItemInfo tmpItem1 = getItemData(catalog, "4123BLA");
    ItemInfo tmpItem2 = getItemData(catalog, "ABCD");
    cout << "tmpItem1: "<<tmpItem1.description<<endl;
    cout << "tmpItem2: "<<tmpItem2.description<<endl<<endl;
    
    
    
    
    return 0;
}

void readCatalog(Catalog &catalog, const string &filename) throw (runtime_error){
    // create a file-reading object
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good()){
        throw runtime_error("ERROR: An error occurred opening the file..");
    }
    //        return 1; // exit if file not found
    //
    // read each line of the file
    while (!fin.eof())
    {
        //more of an oldschool approach

        char buf[1000];
        //get the line
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
    cout <<setw(10)<<left<<"SKU"<<setw(60)<<left<<"Description"<<setw(20)<<left<<"Unit Price"<<setw(20)<<left<<"Shipping Weight"<<endl;
    cout<<setfill('=')<<setw(110)<<"="<<endl;
    cout <<setfill(' ');
    for(auto it = catalog.begin(); it!= catalog.end(); it++){
        cout <<setw(10)<<left<<it->second.SKU<<setw(60)<<left<<it->second.description<<setw(20)<<left<<it->second.unitPrice<<setw(20)<<left<<it->second.shippingWeight<<endl;
    }
    cout << endl<<endl;
    
}

ItemInfo getItemData(const Catalog & catalog, const string & sku){
    //search for the item, return a dummy item if we can't find it
    Catalog::const_iterator pos = catalog.find(sku);
    return (pos == catalog.end()) ? ItemInfo():pos->second;
}

// Lists the SKU, description, and quantity of each type of order item
void displayOrderItems(const Order & order, const Catalog & catalog){
    cout <<setw(10)<<left<<"SKU"<<setw(60)<<left<<"Description"<<setw(20)<<left<<"Quantity"<<endl;
    cout<<setfill('=')<<setw(90)<<"="<<endl;
    cout <<setfill(' ');
    //just iterate over the contents of the order
    for(auto it = order.begin(); it != order.end(); it++){
        //sexy right?
        //get the iteminfo for the corresponding SKU
        ItemInfo item = catalog.find(it->first)->second;
        cout <<setw(10)<<left<< item.SKU<<setw(60)<<left<<item.description<<setw(20)<<left<<it->second<<endl;
    }
    cout << endl<<endl;
    
}

// adds item(s) to the order; throws a logic_error if the item cannot
// be found in the catalog
void addItem(Order & order, const Catalog & catalog, const string & sku, int quantity) throw (logic_error){
    //ok first check to make sure we're working with an existent item
    Catalog::const_iterator itc = catalog.find(sku);
    if(itc == catalog.end()){
        throw logic_error("ERROR: Item does not exist in catalog");
    }
    //ok check to see if we already have that item in the order
    //notice we use an iterator vs a const_iterator
    Order::iterator ito = order.find(sku);
    if(ito == order.end()){
        order[sku] = quantity;
    }else{
        ito->second += quantity;
    }
       
}

// removes items(s) from the order; throws a logic_error if the item
// cannot be found in the order
void removeItem(Order & order, const string & sku, int quantity) throw(logic_error){
    //do the serach
    Order::iterator it = order.find(sku);
    if(it == order.end()){
        throw logic_error("ERROR: Attempting to remove item that does not exist in order");
    }else{
        //basically if we're removing all the items from the order, then we're going to go ahead and remove it from the
        //map, otherwise we want to just decrease the quantity
        if(quantity == it->second){
            order.erase(it);
        }else{
            it->second -= quantity;
        }
    }
    
}

// displays the number of unique item types, the total number of
// items, the total cost, and the total shipping weight
void displayOrderSummary(const Order & order, const Catalog & catalog){
    int totalItems,totalCost, totalWeight,distinctItems;
    totalItems = totalCost = totalWeight = distinctItems= 0;
    for(auto ito = order.begin(); ito != order.end(); ito++){
        ItemInfo item = catalog.find(ito->first)->second;
        distinctItems++;
        totalWeight += item.shippingWeight * ito->second;
        totalCost += item.unitPrice * ito->second;
        totalItems += ito->second;
    }
    cout <<"Order Summary"<<endl;
    cout<<setfill('=')<<setw(110)<<"="<<endl;
    cout <<setfill(' ');
    cout << "Distinct item types: "<<distinctItems<<endl;
    cout << "Total number of items: "<<totalItems<<endl;
    cout << "Total cost: "<<totalCost<<endl;
    cout << "Total shipping weight: "<<totalWeight<<endl<<endl;
    
}












