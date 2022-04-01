﻿namespace task1
{
    class Program
    {
        static void Main(string[] args)
        {
            Station station = Station.getInstance("Vitebsk");
            Console.WriteLine(station.StationName);
            Station station2 = Station.getInstance("Gomel");
            Console.WriteLine(station2.StationName);

            station.NumberOfSeats = 200;
            station.TicketsSold = 134;

            station.StationName = "Minsk";
            Console.WriteLine(station.StationName);

            station.ChangeTicketPrice = 10;
            Console.WriteLine(station.countPriceOfUnsoldTickets());// (200-134)*10
        }
    }
}
