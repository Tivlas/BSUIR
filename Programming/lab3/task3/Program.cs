using dateServiceNamespace;
namespace task3
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime tempDate;
            Console.Write("Enter the date: ");
            while (!DateTime.TryParse(Console.ReadLine(), out tempDate))
            {
                Console.WriteLine("Invalid input, try again!");
            }
            DateService.GetDay(tempDate.ToString());

            int day, month, year;
            string date;
            do
            {
                Console.Write("Enter the day: ");
                while (!int.TryParse(Console.ReadLine(), out day))
                {
                    Console.WriteLine("Invalid input, try again!");
                }
                Console.Write("Enter the month: ");
                while (!int.TryParse(Console.ReadLine(), out month))
                {
                    Console.WriteLine("Invalid input, try again!");
                }
                Console.Write("Enter the year: ");
                while (!int.TryParse(Console.ReadLine(), out year))
                {
                    Console.WriteLine("Invalid input, try again!");
                }

                date = day.ToString() + "." + month.ToString() + "." + year.ToString();

            } while (!DateTime.TryParse(date, out tempDate));
            DateService.GetDaysSpan(day, month, year);

        }
    }
}
