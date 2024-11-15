using System;

namespace AbstractFactory
{
    public class AbstractFactory
    {
        // AbstractProductA
        abstract class Car
        {
            public abstract void Info();
        }

        // ConcreteProductA1
        class Ford : Car
        {
            public override void Info()
            {
                Console.WriteLine("Ford");
            }
        }

        // ConcreteProductA2
        class Toyota : Car
        {
            public override void Info()
            {
                Console.WriteLine("Toyota");
            }
        }

        // ConcreteProductA3 (Mercedes)
        class Mercedes : Car
        {
            public override void Info()
            {
                Console.WriteLine("Mercedes");
            }
        }

        // AbstractProductB
        abstract class Engine
        {
            public virtual void GetPower()
            {
            }
        }

        // ConcreteProductB1
        class FordEngine : Engine
        {
            public override void GetPower()
            {
                Console.WriteLine("Ford Engine 4.4");
            }
        }

        // ConcreteProductB2
        class ToyotaEngine : Engine
        {
            public override void GetPower()
            {
                Console.WriteLine("Toyota Engine 3.2");
            }
        }

        // ConcreteProductB3 (MercedesEngine)
        class MercedesEngine : Engine
        {
            public override void GetPower()
            {
                Console.WriteLine("Mercedes Engine 5.0");
            }
        }

        // AbstractFactory
        interface ICarFactory
        {
            Car CreateCar();
            Engine CreateEngine();
        }

        // ConcreteFactory1
        class FordFactory : ICarFactory
        {
            Car ICarFactory.CreateCar()
            {
                return new Ford();
            }

            Engine ICarFactory.CreateEngine()
            {
                return new FordEngine();
            }
        }

        // ConcreteFactory2
        class ToyotaFactory : ICarFactory
        {
            Car ICarFactory.CreateCar()
            {
                return new Toyota();
            }

            Engine ICarFactory.CreateEngine()
            {
                return new ToyotaEngine();
            }
        }

        // ConcreteFactory3 (MercedesFactory)
        class MercedesFactory : ICarFactory
        {
            Car ICarFactory.CreateCar()
            {
                return new Mercedes();
            }

            Engine ICarFactory.CreateEngine()
            {
                return new MercedesEngine();
            }
        }

        class Program
        {
            static void Main(string[] args)
            {
                // Toyota Factory
                ICarFactory carFactory = new ToyotaFactory();
                Car myCar = carFactory.CreateCar();
                myCar.Info();
                Engine myEngine = carFactory.CreateEngine();
                myEngine.GetPower();

                // Ford Factory
                carFactory = new FordFactory();
                myCar = carFactory.CreateCar();
                myCar.Info();
                myEngine = carFactory.CreateEngine();
                myEngine.GetPower();

                // Mercedes Factory
                carFactory = new MercedesFactory();
                myCar = carFactory.CreateCar();
                myCar.Info();
                myEngine = carFactory.CreateEngine();
                myEngine.GetPower();

                Console.ReadKey();
            }
        }
    }
}
