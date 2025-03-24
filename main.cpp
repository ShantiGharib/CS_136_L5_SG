//Shanti Gharib
//Lab 5

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>



enum class CarFields {
    
    ID = 1,
    MODEL,
    BUILDER,
    QUANTITY,
    PRICE,

    SIZE,
    
};


class Car {

private:

    std::string id{"NULL"}, model{"NULL"}, builder{"NULL"};
    int quantity{0};
    double price{0.0};
    
    std::string ValidateCarId(const std::string &carId) const;
    std::string ValidateModel(const std::string &carModel) const;
    std::string ValidateQuantity(const int carQuantity) const;
    std::string ValidatePrice(const double carPrice) const;

public:

    Car();
    
    Car(const Car &source);
    
    ~Car();


    inline const std::string& GetId() const;
    inline const std::string& GetModel() const;
    inline const std::string& GetBuilder() const;
    inline int GetQuantity() const;
    inline double GetPrice() const;
    
    std::string ToString() const;

    std::string SetRecord(const std::string &carId, const std::string &carModel, const std::string &carBuilder, const int carQuantity, const double carPrice);

};


class Inventory {

public:

    static constexpr const int INVENTORY_MAX_SIZE{25};

private:


    Car  cars[INVENTORY_MAX_SIZE];
    Car* pCarsSorted[INVENTORY_MAX_SIZE] {nullptr};
    
    int usedSize{0};

    
    void LoadRecords();
    
    void SortByField(const int field);
    void GetPriceRangeSearchQuery(double &price, double &rangePercent) const;

    std::string SearchByCarModelBuilderNameHelper(const std::string &searchQuery) const;
    std::string SearchByPriceRangeHelper(const double lowerBound, const double upperBound) const;


public:

    
    Inventory();
    Inventory(const Inventory &source);
    ~Inventory();


    inline int GetCurrentSize() const;
    inline const Car& GetElement(const int index) const;
    inline const Car* GetPointerToElement(const int index) const;
    
    void AddElement(const Car &car);

    void PrintSortedRecords(const int field);

    void PrintModelBuilderQueryResults() const;
    void PrintPriceRangeQueryResults() const;
    std::string PrintLowInventory() const;


};


enum class MainMenu {
    
    SEARCH_RECORDS = 1,
    SORT_RECORDS,
    PRINT_OUT_AND_QUIT,

    
};

enum class SearchMenu {

    CAR_MODEL_BUILDER_NAME = 1,
    PRICE_RANGE,
    LEAVE_SUB_MENU,

};


void DisplayMenu();
void DisplaySearchMenu();
void DisplaySortMenu();

void SearchMenu(Inventory &cars);

void SortMenu(Inventory &cars);

void PrintOutAndQuit(Inventory &cars);

void HandleDefaultMenuCase();

std::string PrintHeading();
std::string PrintSeparationLine(const int width);

bool IsAlpha(char c);
bool IsDigit(char c);

void LowerToAlpha(std::string &str);






static const std::string 
 INPUT_FILE{"in_cars.txt"},
 ERROR_FILE {"err_records.txt"},
 OUTPUT_FILE{"out_cars.txt"};


int main() {

    std::fixed;

    std::setprecision(2);


    Inventory cars;


    int option{0};


    while (option != static_cast<int>(MainMenu::PRINT_OUT_AND_QUIT)) {
        
        
        DisplayMenu();

        
        std::cin >> option;

        std::cout << "\n\n";

        
        switch (option) {
        
            case static_cast<int>(MainMenu::SEARCH_RECORDS): {
            
                SearchMenu(cars);

                break;

            }

            case static_cast<int>(MainMenu::SORT_RECORDS): {
            
                SortMenu(cars);

                break;
            
            }

            case static_cast<int>(MainMenu::PRINT_OUT_AND_QUIT): {
            
                PrintOutAndQuit(cars);

                break;
            
            }

            default: {
            
                HandleDefaultMenuCase();

                break;
            
            }
        
            
        }
        
    
    }


}






Inventory::Inventory() {

    
    LoadRecords();


}


Inventory::Inventory(const Inventory &source) {}


Inventory::~Inventory() {}


void Inventory::AddElement(const Car &car) {

    if (usedSize != INVENTORY_MAX_SIZE) {
    
        cars[usedSize] = car;

        pCarsSorted[usedSize] = &cars[usedSize];

        ++usedSize;
    
    }

    else {
    
        std::cout << "inventory instance at max capacity\n\n";

    }


}


inline int Inventory::GetCurrentSize() const {


    return usedSize;


}


inline const Car& Inventory::GetElement(const int index) const {

    
    return cars[index];


}


inline const Car* Inventory::GetPointerToElement(const int index) const {

    
    return pCarsSorted[index];


}


void Inventory::SortByField(const int field) {


    bool sortStatus{false};

    while (!sortStatus) {
    
        sortStatus = true;

        for (int i = 1; i < usedSize; ++i) {

            bool isInversion{false};

            Car* pPrev{pCarsSorted[i-1]}, *pCurr{pCarsSorted[i]};

            switch (field) {
    
                case static_cast<int>(CarFields::ID): {
        
                    isInversion = pPrev->GetId() < pCurr->GetId();

                    break;
            
                }
    
                case static_cast<int>(CarFields::MODEL): {
    
                    isInversion = pPrev->GetModel() < pCurr->GetModel();
    
                    break;

                }

                case static_cast<int>(CarFields::BUILDER): {
    
                    isInversion = pPrev->GetBuilder() < pCurr->GetBuilder();

                    break;

                }

                case static_cast<int>(CarFields::QUANTITY): {
    
                    isInversion = pPrev->GetQuantity() < pCurr->GetQuantity();

                    break;
        
                }

                case static_cast<int>(CarFields::PRICE): {
    
                    isInversion = pPrev->GetPrice() < pCurr->GetPrice();

                    break;

                }
    
            }
        
            if (isInversion) {
            
                Car *pTemp = pCarsSorted[i];

                pCarsSorted[i] = pCarsSorted[i-1];

                pCarsSorted[i-1] = pTemp;

                sortStatus = false;

            }
        
        }
    
    }

}


std::string Inventory::SearchByCarModelBuilderNameHelper(const std::string &searchQuery) const {

    
    std::ostringstream result;
    
    for (int i = 0; i < usedSize; ++i) {
    
        const Car &curr = GetElement(i);
        
        const std::string &modelName = curr.GetModel(),
            &builderName = curr.GetBuilder();

        if (modelName.find(searchQuery) != std::string::npos || builderName.find(searchQuery) != std::string::npos) {
        
            result << curr.ToString() << "\n";
        
        }
    
    }

    return result.str();

}


void Inventory::PrintModelBuilderQueryResults() const {
    
    static constexpr const int HEADING_WIDTH{55};

    std::cout << "\n\nEnter Search Query:\n\t";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string searchQuery;
    std::getline(std::cin, searchQuery);

    LowerToAlpha(searchQuery);

    std::string result{SearchByCarModelBuilderNameHelper(searchQuery)};

    if (result.empty()) {
    
        std::cout << "No matching records found\n\n";

    }

    else {
    
        std::cout << "\n\n" << PrintHeading() << "\n" << PrintSeparationLine(HEADING_WIDTH) << "\n" << result << "\n\n";
    
    }
    

}


std::string Inventory::SearchByPriceRangeHelper(const double lowerBound, const double upperBound) const {
    
    static constexpr const int LOW_INVENTORY_THRESHOLD{5};
    
    
    std::ostringstream result;

    for (int i = 0; i < usedSize; ++i) {
        
        
        const Car& curr = GetElement(i);
        const double currPrice = curr.GetPrice();

        
        if (lowerBound <= currPrice && currPrice <= upperBound) {
            
            
            result << curr.ToString() << "|";
            

            if (curr.GetQuantity() < LOW_INVENTORY_THRESHOLD) {
            

                result << std::left << std::setw(10) << " low inventory" << std::right;
                
            
            }


            result << "\n";
        
        
        }
    
    }

    
    return result.str();


}


void Inventory::GetPriceRangeSearchQuery(double &price, double &rangePercent) const {

    
    std::cout << "\n\nEnter Target Price:\n\t";

    std::cin >> price;

    std::cout << "\n\nEnter Range (+- Percent):\n\t";

    std::string range;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(std::cin, range);

    std::istringstream stream(range);

    stream >> rangePercent;

    char percentSymbol{' '};
    
    stream >> percentSymbol;

    if (percentSymbol == '%') rangePercent /= 100.0;


}


void Inventory::PrintPriceRangeQueryResults() const {

    static constexpr const int HEADING_WIDTH{75};

    double price{0.0}, rangePercent{0.0};
    
    GetPriceRangeSearchQuery(price, rangePercent);

    const double lowerBound{price * (1.0 - rangePercent)}, upperBound{price * (1.0 + rangePercent)};


    std::cout << "\n\nPrice Range:\n\t$" << lowerBound << " - $" << upperBound << "\n\n\n";

    std::string result{SearchByPriceRangeHelper(lowerBound, upperBound)};
    
    
    if (result.empty()) {
    
        
        std::cout << "No Low Inventory\n\n";
    

    }

    else {
    

        std::cout << PrintHeading() << std::left << std::setw(10) << "| Inventory Status" << std::right << "\n" 
                  << PrintSeparationLine(HEADING_WIDTH) << "\n" 
                  << result << "\n";
        

    }


}


void Inventory::PrintSortedRecords(const int field) {
    
    static constexpr const int HEADING_WIDTH{55};
    
    SortByField(field);
                
    
    std::cout << PrintHeading() << "\n" << PrintSeparationLine(HEADING_WIDTH) << "\n";
                
    for (int i = 0; i < usedSize; ++i) {
                
        std::cout << GetPointerToElement(i)->ToString() << "\n";
                
    }

    std::cout << "\n\n";


}


std::string Inventory::PrintLowInventory() const {

    static constexpr const int LOW_INVENTORY_THRESHOLD{5};
    
    
    std::ostringstream result;


    for (int i = 0; i < usedSize; ++i) {
        
        const Car &curr = GetElement(i);

        if (curr.GetQuantity() < LOW_INVENTORY_THRESHOLD) {

            result << curr.ToString() << "\n";
            
        }
    
    }

    
    return result.str();

}


void Inventory::LoadRecords() {
    
    
    std::ifstream inStream(INPUT_FILE);
    
   
    if (inStream.is_open()) {
    
        std::ofstream errStream(ERROR_FILE);

    
        while (usedSize != INVENTORY_MAX_SIZE && !inStream.eof()) {
        
        
            std::string record;
            std::getline(inStream, record);
            
            std::istringstream carInfo(record);
            
            std::string carId, carModel, carBuilder;
            int carQuantity{0};
            double carPrice{0.0};

            carInfo >> carId >> carModel >> carBuilder >> carQuantity >> carPrice;

            Car temp;
            
            LowerToAlpha(carBuilder);
            

            std::string errorMessage{temp.SetRecord(carId, carModel, carBuilder, carQuantity, carPrice)};
            
            
            if (errorMessage.empty()) {
                
                
                AddElement(temp);
                

            }
            
            else if (errStream.is_open()) {

                errStream << carId       << " " 
                          << carModel    << " " 
                          << carBuilder  << " " 
                          << carQuantity << " " 
                          << carPrice    << " " << "Invalid ID:" << errorMessage << "\n";
        
            }    
    
        }
    
    
        inStream.close();

        
        if (errStream.is_open()) {
            
            
            errStream.close();

        
        }


    }

    else {
    
        
        std::cout << "Cannot access Input File to load data\n";
        
        
        exit(EXIT_FAILURE);
    

    }


}






Car::Car() {}


Car::Car(const Car &source) {}


Car::~Car() {}


inline const std::string& Car::GetId() const {
    
    
    return id;


}


inline const std::string& Car::GetModel() const {

    
    return model;


}


inline const std::string& Car::GetBuilder() const{


    return builder;


}


inline int Car::GetQuantity() const {

    
    return quantity;


}


inline double Car::GetPrice() const {

    
    return price;


}


std::string Car::ValidateCarId(const std::string &carId) const {
    
    
    static constexpr const int CAR_ID_SIZE{8}, MID{CAR_ID_SIZE / 2};
    
    int i{0};
    
    std::string errorMessage;
    
    if (carId.size() == CAR_ID_SIZE) {
        
        
        while (i < MID && IsAlpha(carId[i]) && carId[i] != 'O') ++i;
        
        
        if (i == MID) {
            
            
            while (i < CAR_ID_SIZE && IsDigit(carId[i]) && carId[i] != '0') ++i;
            
            
            if (i != CAR_ID_SIZE) errorMessage.append(" Last four characters of ID must be digits other than \'0\'.");


        }
       
        
        else errorMessage.append(" First four characters of ID must be alpha other than \'O\'.");


    }
    
    else errorMessage.append(" ID's length must equal eight.");
    

    return errorMessage;


}


std::string Car::ValidateModel(const std::string &carModel) const {
    
    static constexpr const int MODEL_MIN_SIZE{3};

    std::string errorMessage;
    
    int i{0};
    
    if (carModel.size() >= MODEL_MIN_SIZE) {
       
        if (IsAlpha(carModel[0])) {
            
            ++i;
            
            while (i < carModel.size() && (IsAlpha(carModel[i]) || IsDigit(carModel[i]))) {
            
                ++i;
            
            }
            
            if (i != carModel.size()) {
                
                errorMessage.append(" Model name must contain only alpha-numeric characters.");
            
            }
        
        }
        
        else {
            
            errorMessage.append(" First character of model name must be alpha.");
        
        }
    
    }

    else {
        
        errorMessage.append(" Model name's length must be at least three.");
   
    }

    
    return errorMessage;

}


std::string Car::ValidateQuantity(const int carQuantity) const {
    
    
    return (carQuantity >= 0) ? "" : " Cannot have negative quantity.";


}


std::string Car::ValidatePrice(const double carPrice) const {
    
    static constexpr const double FLOOR_PRICE{12999.00};

    return (carPrice > FLOOR_PRICE) ? "" : " Price must exceed the floor price of $12,999.00.";


}


std::string Car::SetRecord(const std::string &carId, const std::string &carModel, const std::string &carBuilder, const int carQuantity, const double carPrice) {

    std::string errorMessage;

    
    errorMessage.append(ValidateCarId(carId));
    
    errorMessage.append(ValidateModel(carModel));
    
    errorMessage.append(ValidateQuantity(carQuantity));
    
    errorMessage.append(ValidatePrice(carPrice));
    
    
    if (errorMessage.empty()) {
    
        id = carId;
        model = carModel;
        
        builder = carBuilder;
        
        quantity = carQuantity;
        price = carPrice;

    }

    return errorMessage;

}


std::string Car::ToString() const {

    
    std::ostringstream result;
    
    
    result << std::left
                  
        << "| " << std::setw(8)  << id
        << "| " << std::setw(8)  << model
        << "| " << std::setw(13) << builder 
                    
        << std::right
          
        << "| " << std::setw(8)  << quantity
        << "| " << std::setw(8)  << price;

    
    return result.str();


}






void HandleDefaultMenuCase() {

    std::cout << "Invalid Option\n\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


}


bool IsAlpha(char c) {
    
    static constexpr const char ALPHA_A{'A'}, ALPHA_Z{'Z'};
    
    return (ALPHA_A <= c && c <= ALPHA_Z);

}


bool IsDigit(char c) {
    
    static constexpr const char ZERO{'0'}, NINE{'9'};
    
    return (ZERO <= c && c <= NINE);

}


void LowerToAlpha(std::string &str) {

    static constexpr const char 
        TINY_A{'a'}, TINY_Z{'z'}, 
        ALPHA_A{'A'}, ALPHA_Z{'Z'};

    for (size_t i = 0; i < str.size(); ++i) {
        
        if (TINY_A <= str[i] && str[i] <= TINY_Z) str[i] += (ALPHA_A - TINY_A);
    
    }


}


std::string PrintHeading() {

    
    std::ostringstream result;


    result << std::left
        << std::setw(10) << "| ID"
        << std::setw(10) << "| Model"
        << std::setw(15) << "| Manufacturer"
        << std::setw(10) << "| Quantity" 
        << std::setw(10) << "| Price" << std::right;



    return result.str();


}


std::string PrintSeparationLine(const int width) {

   
   std::ostringstream result;

   result << std::setfill('-') << std::setw(width) << "" << std::setfill(' ');

   return result.str();


}


void SearchMenu(Inventory &cars) {


    int option{0};

    while (option != static_cast<int>(SearchMenu::LEAVE_SUB_MENU)) {
    
    
        DisplaySearchMenu();


        std::cin >> option;
        
        std::cout << "\n\n";


        switch (option) {
        
        
            case static_cast<int>(SearchMenu::CAR_MODEL_BUILDER_NAME): {
            
            
                cars.PrintModelBuilderQueryResults();

                break;
            
            }

            case static_cast<int>(SearchMenu::PRICE_RANGE): {
            
                cars.PrintPriceRangeQueryResults();

                break;


            }

            case static_cast<int>(SearchMenu::LEAVE_SUB_MENU): {
            
            
                std::cout << "Left Search Menu\n\n";

                break;
            
            }

            default: {
            
                HandleDefaultMenuCase();

                break;
            
            
            }
        
        
        
        }
    
    
    
    }
    





}


void DisplaySearchMenu() {


    std::cout << "\n\nSelect Search Category"
                 
                 "\n\t\t1. Car Model/Manufacturer Name"
                 
                 "\n\t\t2. Price Range"

                 "\n\t3. None: Leave Sub Menu\n\n";


}


void SortMenu(Inventory &cars) {

    int option{0};

    static constexpr const int LEAVE_SUB_MENU{6};

    while (option != LEAVE_SUB_MENU) {
    
    
        DisplaySortMenu();

        
        std::cin >> option;

        std::cout << "\n\n";
        
        
        switch (option) {
        
        
            case static_cast<int>(CarFields::ID):

            case static_cast<int>(CarFields::MODEL):
            
            case static_cast<int>(CarFields::BUILDER):

            case static_cast<int>(CarFields::QUANTITY):

            case static_cast<int>(CarFields::PRICE): {
            
                
                cars.PrintSortedRecords(option);

                break;


            }

            case LEAVE_SUB_MENU: {
            
            
                std::cout << "Left Sort Menu\n\n";
                
                break;


            }

            default: {
            
                
                HandleDefaultMenuCase();

                break;

            
            }
        
        
        }
    
    
    
    
    }


}


void DisplaySortMenu() {


    std::cout << "\n\nSelect a Sort Key"
                 
                 "\n\t\t1. ID"
                 
                 "\n\t\t2. Model"
                 
                 "\n\t\t3. Manufactuer"
                 
                 "\n\t\t4. Quantity On Hand"
                 
                 "\n\t\t5. Price"
                 
                 "\n\t6. None: Leave Menu\n\n";


}


void PrintOutAndQuit(Inventory &cars) {

    
    static constexpr const int HEADING_WIDTH{55};
    
    
    std::string result{cars.PrintLowInventory()};

    std::ofstream outStream(OUTPUT_FILE);

    if (!outStream.is_open()) {
    
        std::cout << "Output file cannot open. Low Inventory will output only to the console\n\n";
    
    }
    
    if (result.empty()) {
    
        std::cout << "No Low Inventory\n";
    
    }

    else {
    
    
        std::cout << "Inventory with low status:\n\n" 
            << PrintHeading() << "\n" 
            << PrintSeparationLine(HEADING_WIDTH) << "\n" 
            << result << "\n";
        
        
        if (outStream.is_open()) {
        
            outStream << result;
        
        }        
    
    
    }


}


void DisplayMenu() {


    std::cout << "\n\nOptions:" 
             
        "\n\t1. Search cars"
             
        "\n\t2. Sort cars"
             
        "\n\t3. Quit\n\n";


}


//  TestCase1: Search Function; mixed alpha & non-alpha characters
//  Output
//  
//   Options:
//           1. Search cars
//           2. Sort cars
//           3. Quit
//  
//   1
//  
//   Select Search Category
//                   1. Car Model/Manufacturer Name
//                   2. Price Range
//           3. None: Leave Sub Menu
//  
//   1
//  
//   Enter Search Query:
//           HONda
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//  
//  -------------------------------------------------------------------------------------
//  
//  TestCase2: Search Function; Price Range with percent symbol
//  
//  Output
//  
//   Select Search Category
//                   1. Car Model/Manufacturer Name
//                   2. Price Range
//           3. None: Leave Sub Menu
//  
//   2
//  
//   Enter Target Price:
//           50000
//  
//  
//   Enter Range (+- Percent):
//           25%
//  
//  
//   Price Range:
//           $37500 - $62500
//  
//   No Low Inventory
//  
//  -------------------------------------------------------------------------------------
//  
//  TestCase3: Search Function: Price Range without percent symbol
//  
//  Output
//  
//   Select Search Category
//                   1. Car Model/Manufacturer Name
//                   2. Price Range
//           3. None: Leave Sub Menu
//  
//   2
//  
//   Enter Target Price:
//           50000
//  
//  
//   Enter Range (+- Percent):
//           0.25
//  
//  
//   Price Range:
//           $37500 - $62500
//  
//   No Low Inventory
//  
//  ----------------------------------------------------------------------------------------
//  
//  TestCase4: Search Function: Low Inventory search by Price
//  Output:
//  
//   Options:
//           1. Search cars
//           2. Sort cars
//           3. Quit
//  
//   1
//  
//   Select Search Category
//                   1. Car Model/Manufacturer Name
//                   2. Price Range
//           3. None: Leave Sub Menu
//  
//   2
//  
//   Enter Target Price:
//           15000
//  
//  
//   Enter Range (+- Percent):
//           0%
//  
//  
//   Price Range:
//           $15000 - $15000
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price   | Inventory Status
//   ---------------------------------------------------------------------------
//   | MPAZ2297| RX791X  | FORD         |       33|    15000|
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000| low inventory
//  
//  
//  ----------------------------------------------------------------------------------------
//  TestCase5: Leaving Search Sub Menu
//  Output:
//  
//   Select Search Category
//                   1. Car Model/Manufacturer Name
//                   2. Price Range
//           3. None: Leave Sub Menu
//  
//   3
//  
//  
//   Left Search Menu
//  
//  -------------------------------------------------------------------
//  
//  TestCase6: Sort Function: By ID
//  Output:
//  
//   Options:
//           1. Search cars
//           2. Sort cars
//           3. Quit
//  
//   2
//  
//   Select a Sort Key
//                   1. ID
//                   2. Model
//                   3. Manufactuer
//                   4. Quantity On Hand
//                   5. Price
//           6. None: Leave Menu
//  
//   1
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPTZ2129| RX791W  | DODGE        |       12|  17999.2
//   | MPAZ2297| RX791X  | FORD         |       33|    15000
//   | MPAZ2293| RX791C  | BENTLY       |       29|    12999
//   | MPAZ2291| RX791O  | KIA          |       24|    13999
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//   | MPAZ2129| RX791W  | FIAT         |       12|  15999.2
//  
//  -------------------------------------------------------------------
//  
//  TestCase7: Sort Function: By Model
//  Output:
//  
//   Select a Sort Key
//                   1. ID
//                   2. Model
//                   3. Manufactuer
//                   4. Quantity On Hand
//                   5. Price
//           6. None: Leave Menu
//  
//   2
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPAZ2297| RX791X  | FORD         |       33|    15000
//   | MPTZ2129| RX791W  | DODGE        |       12|  17999.2
//   | MPAZ2129| RX791W  | FIAT         |       12|  15999.2
//   | MPAZ2291| RX791O  | KIA          |       24|    13999
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//   | MPAZ2293| RX791C  | BENTLY       |       29|    12999
//  
//  -------------------------------------------------------------------
//  
//  TestCase8: Sort Function: By Manufacturer
//  Output:
//  
//   Select a Sort Key
//                   1. ID
//                   2. Model
//                   3. Manufactuer
//                   4. Quantity On Hand
//                   5. Price
//           6. None: Leave Menu
//  
//   3
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPAZ2291| RX791O  | KIA          |       24|    13999
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//   | MPAZ2297| RX791X  | FORD         |       33|    15000
//   | MPAZ2129| RX791W  | FIAT         |       12|  15999.2
//   | MPTZ2129| RX791W  | DODGE        |       12|  17999.2
//   | MPAZ2293| RX791C  | BENTLY       |       29|    12999
//  
//  -------------------------------------------------------------------
//  
//  TestCase9: Sort Function: By Quantity
//  Output:
//  
//   Select a Sort Key
//                   1. ID
//                   2. Model
//                   3. Manufactuer
//                   4. Quantity On Hand
//                   5. Price
//           6. None: Leave Menu
//  
//   4
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPAZ2297| RX791X  | FORD         |       33|    15000
//   | MPAZ2293| RX791C  | BENTLY       |       29|    12999
//   | MPAZ2291| RX791O  | KIA          |       24|    13999
//   | MPAZ2129| RX791W  | FIAT         |       12|  15999.2
//   | MPTZ2129| RX791W  | DODGE        |       12|  17999.2
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//  
//  -------------------------------------------------------------------
//  
//  TestCase10: Sort Function: By Price
//  Output:
//  
//   Select a Sort Key
//                   1. ID
//                   2. Model
//                   3. Manufactuer
//                   4. Quantity On Hand
//                   5. Price
//           6. None: Leave Menu
//  
//   5
//  
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPTZ2129| RX791W  | DODGE        |       12|  17999.2
//   | MPAZ2129| RX791W  | FIAT         |       12|  15999.2
//   | MPAZ2297| RX791X  | FORD         |       33|    15000
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//   | MPAZ2291| RX791O  | KIA          |       24|    13999
//   | MPAZ2293| RX791C  | BENTLY       |       29|    12999
//  
//  -------------------------------------------------------------------
//  
//  TestCase11: Sort Function: Leaving Sort Sub Menu
//  Output:
//  
//   Select a Sort Key
//                   1. ID
//                   2. Model
//                   3. Manufactuer
//                   4. Quantity On Hand
//                   5. Price
//           6. None: Leave Menu
//  
//   6
//  
//  
//   Left Sort Menu
//  
//  -------------------------------------------------------------------
//  
//  TestCase12: Print Out And Quit Function
//  Output:
//  
//   Options:
//           1. Search cars
//           2. Sort cars
//           3. Quit
//  
//   3
//  
//  
//   Inventory with low status:
//  
//   | ID      | Model   | Manufacturer | Quantity| Price
//   -------------------------------------------------------
//   | MPAZ2249| RX791E  | HONDA        |        0|    15000
//  
//  -------------------------------------------------------------------
