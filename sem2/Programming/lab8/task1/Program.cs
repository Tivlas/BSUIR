using task1;
class Program
{
    static void Main(string[] args)
    {
        Station station = new Station("Minsk");
        station.AddTicket(new Ticket("Vitebsk", new NoDiscount(10)));
        station.AddTicket(new Ticket("Gomel", new NoDiscount(9)));
        station.AddTicket(new Ticket("Molodechno", new WithDiscount(10, 5)));
        station.AddTicket(new Ticket("Brest", new WithDiscount(19, 15)));
        station.AddTicket(new Ticket("Mogilev", new NoDiscount(77)));
        station.AddTicket(new Ticket("Grodno", new WithDiscount(15, 5)));
        string? lowestPriceDestination = station.FindTicketDestinationWithLowestPrice();
        if (lowestPriceDestination != null)
        {
            Console.WriteLine($"The lowest price destination is {lowestPriceDestination}");
        }
        else
        {
            Console.WriteLine("There are no tickets");
        }
    }
}
