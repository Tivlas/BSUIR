namespace task1.Services
{
    public class Calculations
    {
        public static double count(int z)
        {
            if (z < 0)
            {
                Console.WriteLine("First branch");
                return (2.0 / 3.0 * Math.Pow(Math.Sin(z), 2) - 3.0 / 4.0 * Math.Pow(Math.Cos(z), 2));
            }
            else
            {
                Console.WriteLine("Second branch");
                return 2.0 / 3.0 * Math.Pow(Math.Sin(Math.Sin(z)), 2) - 3.0 / 4.0 * Math.Pow(Math.Cos(Math.Sin(z)), 2);
            }
        }
    }
}
