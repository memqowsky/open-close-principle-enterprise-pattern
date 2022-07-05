#include <iostream>
#include <vector>

using namespace std;

enum class Color
{
    red,
    green,
    blue
};
enum class Size
{
    small,
    medium,
    large
};

struct Product
{
    string name;
    Color color;
    Size size;

    Product(string n, Color c, Size s) : name(n), color(c), size(s) {}
};

struct ProducFilter
{
    vector<Product *> by_color(vector<Product *> items, Color color)
    {
        vector<Product *> result;
        for(auto& i : items){
            if(i->color == color){
                result.push_back(i);
            }
        }
        return result;
    }

    vector<Product *> by_size(vector<Product *> items, Size size)
    {
        vector<Product *> result;
        for(auto& i : items){
            if(i->size == size){
                result.push_back(i);
            }
        }
        return result;
    }

};

// Open-Close Principle
// Enterprise Pattern - patern related with data

template <typename T> struct Specification{

    virtual bool is_satisfied(T* item) = 0;

    // AndSpecification<T> operator &&(Specification<T>&& other){
    //     return AndSpecification<T>(*this, other);
    // }
};

struct ColorSpecification : Specification<Product>{
    
    Color color;

    ColorSpecification(Color c) : color(c) {}

    bool is_satisfied(Product* item) override {
        return item->color == color;
    }

};

struct SizeSpecification : Specification<Product>{
    
    Size size;

    SizeSpecification(Size s) : size(s) {}

    bool is_satisfied(Product* item) override {
        return item->size == size;
    }
};

template <typename T> struct AndSpecification : Specification<T> {

    Specification<T>& first;
    Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second) : first(first), second(second) {}
    
    bool is_satisfied(T* item) override{
        return first.is_satisfied(item) && second.is_satisfied(item);    
    }

};

template <typename T> struct Filter{

    virtual vector<T*> filter (vector<T*> items, Specification<T>& spec) = 0;
};

struct BetterFilter : Filter <Product> {
    vector<Product*> filter (vector<Product*> items, Specification<Product>& spec) override{
        
        vector<Product*> result;
        for(auto &i : items)
        {
            if(spec.is_satisfied(i)){
                result.push_back(i);
            }            
        }
        return result;
    }
};

int main()
{
    Product apple("Apple", Color::green, Size::small);
    Product tree("Tree", Color::green, Size::large);
    Product house("House", Color::blue, Size::large);

    vector<Product*> items {&apple, &tree, &house};

    BetterFilter bf;

    ColorSpecification green(Color::green);
    SizeSpecification small(Size::small);

    AndSpecification<Product> green_and_small(small, green);

    for(auto &item : bf.filter(items, green)){
        cout << item->name << " is green" << endl; 
    }

    cout << endl;

    for(auto &item : bf.filter(items, small)){
        cout << item->name << " is small" << endl; 
    }

    cout << endl;

    // Using overloaded && operator (does not work)
    // auto green_and_large = ColorSpecification(Color::green) && SizeSpecification(Size::large);

    for(auto &item : bf.filter(items, green_and_small)){
        cout << item->name << " is green and small" << endl; 
    }

    return 0;
}