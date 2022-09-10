
namespace dateServiceNamespace
{
    public class DateService
    {
        public static int GetDay(string date)
        {
            DateTime givenDate = DateTime.Parse(date);
            int weekDay = (int)givenDate.DayOfWeek;
            Console.Write("Day of the week: ");
            Console.WriteLine($"{weekDay}\n");
            return weekDay;
        }

        public static int GetDaysSpan(int day, int month, int year)
        {
            DateTime givenDate = new DateTime(year, month, day);
            DateTime now = DateTime.Now;
            int daysBetween = (int)(now.Date - givenDate.Date).TotalDays;
            Console.Write("\nNumber of days between today's date and the entered date: ");
            Console.WriteLine($"{daysBetween}\n");
            return daysBetween;
        }
    }
}
