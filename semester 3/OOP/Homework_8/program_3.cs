// Violation of the Liskov Substitution Principle.
// The answer 100 for area is incorrect.

interface IShape
{
    int GetArea();
}

class Rectangle : IShape
{
    public int Width { get; set; }
    public int Height { get; set; }
    public int GetArea() => Width * Height;
}

class Square : IShape
{
    public int Side { get; set; }
    public int GetArea() => Side * Side;
}
