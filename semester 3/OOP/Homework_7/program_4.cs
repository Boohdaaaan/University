using System;

namespace ChristmasTreeApp
{
    // Step 1: Define the ChristmasTree interface
    public interface IChristmasTree
    {
        string Decorate();
    }

    // Step 2: Concrete implementation of a basic Christmas Tree
    public class ChristmasTree : IChristmasTree
    {
        public string Decorate()
        {
            return "Christmas Tree";
        }
    }

    // Step 3: Abstract Decorator class implementing IChristmasTree
    public abstract class TreeDecorator : IChristmasTree
    {
        protected IChristmasTree _tree;

        public TreeDecorator(IChristmasTree tree)
        {
            _tree = tree;
        }

        public virtual string Decorate()
        {
            return _tree.Decorate();
        }
    }

    // Step 4: Concrete decorators
    public class OrnamentsDecorator : TreeDecorator
    {
        public OrnamentsDecorator(IChristmasTree tree) : base(tree) { }

        public override string Decorate()
        {
            return _tree.Decorate() + " with Ornaments";
        }
    }

    public class GarlandDecorator : TreeDecorator
    {
        public GarlandDecorator(IChristmasTree tree) : base(tree) { }

        public override string Decorate()
        {
            return _tree.Decorate() + " with Garlands";
        }
    }

    public class LightsDecorator : TreeDecorator
    {
        public LightsDecorator(IChristmasTree tree) : base(tree) { }

        public override string Decorate()
        {
            return _tree.Decorate() + " with Lights";
        }
    }

    // Step 5: Add a method to make the tree glow
    public class GlowDecorator : TreeDecorator
    {
        public GlowDecorator(IChristmasTree tree) : base(tree) { }

        public override string Decorate()
        {
            return _tree.Decorate() + " that Glows!";
        }
    }

    // Main Program
    class Program
    {
        static void Main(string[] args)
        {
            // Create a simple Christmas tree
            IChristmasTree tree = new ChristmasTree();

            Console.WriteLine("Basic Tree: " + tree.Decorate());

            // Decorate the tree with ornaments
            tree = new OrnamentsDecorator(tree);
            Console.WriteLine("With Ornaments: " + tree.Decorate());

            // Add garlands
            tree = new GarlandDecorator(tree);
            Console.WriteLine("With Garlands: " + tree.Decorate());

            // Add lights
            tree = new LightsDecorator(tree);
            Console.WriteLine("With Lights: " + tree.Decorate());

            // Make it glow
            tree = new GlowDecorator(tree);
            Console.WriteLine("Final Tree: " + tree.Decorate());
        }
    }
}
