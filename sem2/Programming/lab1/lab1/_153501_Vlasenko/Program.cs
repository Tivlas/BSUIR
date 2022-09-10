using System;

namespace _153501_Vlasenko
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter the first number: ");
            int x = Convert.ToInt32(Console.ReadLine());

            Console.Write("Enter the second number: ");
            int y = Convert.ToInt32(Console.ReadLine());

            int res = x / y;
            Console.WriteLine($"The quotient of their dividion: {res}");

        }
    }
}
