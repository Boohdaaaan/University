using System;

namespace Builder
{
    class Program
    {
        class Pizza
        {
            string dough;
            string sauce;
            string topping;
            public Pizza() { }
            public void SetDough(string d) { dough = d; }
            public void SetSauce(string s) { sauce = s; }
            public void SetTopping(string t) { topping = t; }
            public void Info()
            {
                Console.WriteLine("Dough: {0}\nSauce: {1}\nTopping: {2}",
                dough, sauce, topping);
            }
        }

        // Abstract Builder
        abstract class PizzaBuilder
        {
            protected Pizza pizza;
            public PizzaBuilder() { }
            public Pizza GetPizza() { return pizza; }
            public void CreateNewPizza() { pizza = new Pizza(); }
            public abstract void BuildDough();
            public abstract void BuildSauce();
            public abstract void BuildTopping();
        }

        // Concrete Builder for Hawaiian Pizza
        class HawaiianPizzaBuilder : PizzaBuilder
        {
            public override void BuildDough() { pizza.SetDough("cross"); }
            public override void BuildSauce() { pizza.SetSauce("mild"); }
            public override void BuildTopping() { pizza.SetTopping("ham+pineapple"); }
        }

        // Concrete Builder for Spicy Pizza
        class SpicyPizzaBuilder : PizzaBuilder
        {
            public override void BuildDough() { pizza.SetDough("pan baked"); }
            public override void BuildSauce() { pizza.SetSauce("hot"); }
            public override void BuildTopping() { pizza.SetTopping("pepperoni+salami"); }
        }

        // Concrete Builder for Margarita Pizza
        class MargaritaPizzaBuilder : PizzaBuilder
        {
            public override void BuildDough() { pizza.SetDough("thin crust"); }
            public override void BuildSauce() { pizza.SetSauce("tomato"); }
            public override void BuildTopping() { pizza.SetTopping("mozzarella+basil"); }
        }

        // "Director"
        class Waiter
        {
            private PizzaBuilder pizzaBuilder;
            public void SetPizzaBuilder(PizzaBuilder pb) { pizzaBuilder = pb; }
            public Pizza GetPizza() { return pizzaBuilder.GetPizza(); }
            public void ConstructPizza()
            {
                pizzaBuilder.CreateNewPizza();
                pizzaBuilder.BuildDough();
                pizzaBuilder.BuildSauce();
                pizzaBuilder.BuildTopping();
            }
        }

        // A customer ordering a pizza.
        class BuilderExample
        {
            public static void Main(string[] args)
            {
                Waiter waiter = new Waiter();

                PizzaBuilder hawaiianPizzaBuilder = new HawaiianPizzaBuilder();
                PizzaBuilder spicyPizzaBuilder = new SpicyPizzaBuilder();
                PizzaBuilder margaritaPizzaBuilder = new MargaritaPizzaBuilder();

                Console.WriteLine("Hawaiian Pizza:");
                waiter.SetPizzaBuilder(hawaiianPizzaBuilder);
                waiter.ConstructPizza();
                Pizza hawaiianPizza = waiter.GetPizza();
                hawaiianPizza.Info();

                Console.WriteLine("\nSpicy Pizza:");
                waiter.SetPizzaBuilder(spicyPizzaBuilder);
                waiter.ConstructPizza();
                Pizza spicyPizza = waiter.GetPizza();
                spicyPizza.Info();

                Console.WriteLine("\nMargarita Pizza:");
                waiter.SetPizzaBuilder(margaritaPizzaBuilder);
                waiter.ConstructPizza();
                Pizza margaritaPizza = waiter.GetPizza();
                margaritaPizza.Info();

                Console.ReadKey();
            }
        }
    }
}
