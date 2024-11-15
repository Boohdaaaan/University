// Refactoring the interface using Interface Segregation Principle

// Interface for items that have a price
interface IPriceable
{
    void SetPrice(double price);
}

// Interface for items that can have discounts applied
interface IDiscountable
{
    void ApplyDiscount(string discount);
    void ApplyPromocode(string promocode);
}

// Interface for items that have a color attribute
interface IColorable
{
    void SetColor(byte color);
}

// Interface for items that have a size attribute
interface ISizeable
{
    void SetSize(byte size);
}

// Book class implementing relevant interfaces
class Book : IPriceable, IDiscountable
{
    private double _price;
    private string _discount;
    private string _promocode;

    public void SetPrice(double price)
    {
        _price = price;
    }

    public void ApplyDiscount(string discount)
    {
        _discount = discount;
        // Apply discount logic here
    }

    public void ApplyPromocode(string promocode)
    {
        _promocode = promocode;
        // Apply promo code logic here
    }
}

// TopClothes class implementing relevant interfaces
class TopClothes : IPriceable, IDiscountable, IColorable, ISizeable
{
    private double _price;
    private byte _color;
    private byte _size;
    private string _discount;
    private string _promocode;

    public void SetPrice(double price)
    {
        _price = price;
    }

    public void ApplyDiscount(string discount)
    {
        _discount = discount;
        // Apply discount logic here
    }

    public void ApplyPromocode(string promocode)
    {
        _promocode = promocode;
        // Apply promo code logic here
    }

    public void SetColor(byte color)
    {
        _color = color;
        // Set color logic here
    }

    public void SetSize(byte size)
    {
        _size = size;
        // Set size logic here
    }
}
