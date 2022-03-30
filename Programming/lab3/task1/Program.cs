// task1 + task2
using numbers;
using task1.Services;

namespace task1
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");

            double x, y;
            Console.Write("Enter the 1st number: ");
            while (!double.TryParse(Console.ReadLine(), out x))
            {
                Console.WriteLine("Wrong data! Try again.");
            }
            Console.Write("Enter 2nd number: ");
            while (!double.TryParse(Console.ReadLine(), out y))
            {
                Console.WriteLine("Wrong data! Try again.");
            }
            Console.WriteLine($"Result is {Numbers.calc(x, y)}\n");

            int z;
            Console.Write("Enter 'z': ");
            while (!int.TryParse(Console.ReadLine(), out z))
            {
                Console.WriteLine("Wrong data! Try again.");
            }
            double res = Calculations.count(z);
            Console.WriteLine($"Result is {res}");
        }
    }
}