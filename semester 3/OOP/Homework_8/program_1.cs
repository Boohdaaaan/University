// The `Order` class violates the Single Responsibility Principle.
// In the `Order` class:
// 1. Methods related to order management, such as CalculateTotalSum, AddItem, and DeleteItem, which deal with the business logic of managing an order.
// 2. Methods like PrintOrder, ShowOrder, Load, Save, Update, and Delete that handle persistence and presentation logic.



// Class for representing an Item
class Item
{
    // Properties and methods for Item
}

// Class for managing the order
class Order
{
    private List<Item> itemList;

    public List<Item> ItemList
    {
        get { return itemList; }
        set { itemList = value; }
    }

    public void CalculateTotalSum() { /* ... */ }
    public void GetItems() { /* ... */ }
    public void GetItemCount() { /* ... */ }
    public void AddItem(Item item) { /* ... */ }
    public void DeleteItem(Item item) { /* ... */ }
}

// Class for order persistence (loading, saving, updating, and deleting)
class OrderRepository
{
    public void Load(Order order) { /* ... */ }
    public void Save(Order order) { /* ... */ }
    public void Update(Order order) { /* ... */ }
    public void Delete(Order order) { /* ... */ }
}

// Class for order presentation (printing and showing)
class OrderPrinter
{
    public void PrintOrder(Order order) { /* ... */ }
    public void ShowOrder(Order order) { /* ... */ }
}
