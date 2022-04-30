using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task1.Tests
{
    [TestClass()]
    public class StationTests
    {
        [TestMethod()]
        public void AddTicketTest()
        {
            Station station = new Station("Minsk");
            Assert.AreEqual(station.numbreOfTickets, 0);

            station.AddTicket(new Ticket("Vitebsk", new NoDiscount(10)));
            Assert.AreEqual(station.numbreOfTickets, 1);
            Assert.AreEqual(station[0].Destination, "Vitebsk");
            Assert.AreEqual(station[0].Price, 10);

            station.AddTicket(new Ticket("Gomel", new NoDiscount(9)));
            Assert.AreEqual(station.numbreOfTickets, 2);
            Assert.AreEqual(station[1].Destination, "Gomel");
            Assert.AreEqual(station[1].Price, 9);

            station.AddTicket(new Ticket("Molodechno", new WithDiscount(10, 5)));
            Assert.AreEqual(station.numbreOfTickets, 3);
            Assert.AreEqual(station[2].Destination, "Molodechno");
            Assert.AreEqual(station[2].Price, 9.5);

            station.AddTicket(new Ticket("Brest", new WithDiscount(19, 15)));
            Assert.AreEqual(station.numbreOfTickets, 4);
            Assert.AreEqual(station[3].Price, 16.15);
            Assert.AreEqual(station[3].Destination, "Brest");

            station.AddTicket(new Ticket("Mogilev", new NoDiscount(77)));
            Assert.AreEqual(station.numbreOfTickets, 5);
            Assert.AreEqual(station[4].Destination, "Mogilev");
            Assert.AreEqual(station[4].Price, 77);

            station.AddTicket(new Ticket("Grodno", new WithDiscount(15, 5)));
            Assert.AreEqual(station.numbreOfTickets, 6);
            Assert.AreEqual(station[5].Destination, "Grodno");
            Assert.AreEqual(station[5].Price, 14.25);
        }

        [TestMethod()]
        public void FindTicketDestinationWithLowestPriceTest()
        {
            string? lowestPriceDestination;
            Station station = new Station("Minsk");
            lowestPriceDestination = station.FindTicketDestinationWithLowestPrice();
            Assert.AreEqual(lowestPriceDestination, null);

            station.AddTicket(new Ticket("Vitebsk", new NoDiscount(10)));
            station.AddTicket(new Ticket("Gomel", new NoDiscount(9)));
            station.AddTicket(new Ticket("Molodechno", new WithDiscount(10, 5)));
            lowestPriceDestination = station.FindTicketDestinationWithLowestPrice();
            Assert.AreEqual(lowestPriceDestination, "Gomel");


            station.AddTicket(new Ticket("Brest", new WithDiscount(19, 99)));
            station.AddTicket(new Ticket("Mogilev", new NoDiscount(77)));
            station.AddTicket(new Ticket("Grodno", new WithDiscount(15, 5)));
            lowestPriceDestination = station.FindTicketDestinationWithLowestPrice();
            Assert.AreEqual(lowestPriceDestination, "Brest");
        }
    }
}