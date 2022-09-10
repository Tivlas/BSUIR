using task1;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task1.Tests
{
    [TestClass()]
    public class StationTests
    {
        [TestMethod()]
        public void getInstanceTest()
        {
            Station station = Station.getInstance();
            Station station1 = Station.getInstance();
            Assert.AreEqual(station, station1);
        }

        [TestMethod()]
        public void getInstanceWithStringParameterTest()
        {
            Station station = Station.getInstance("Name");
            Station station1 = Station.getInstance("Xccvcv");
            Station station2 = Station.getInstance(null);

            Assert.AreEqual(station.StationName, station1.StationName);
            Assert.AreEqual(station.StationName, station2.StationName);
        }

        [TestMethod()]
        public void countPriceOfUnsoldTicketsTest()
        {
            Station station = Station.getInstance();
            station.NumberOfSeats = 125;
            station.TicketsSold = 35;
            station.ChangeTicketPrice = 10;

            int[] actual = new int[5];
            for (int i = 0; i < actual.Length; i++)
            {
                actual[i] = station.countPriceOfUnsoldTickets();
                station.NumberOfSeats += 15;
                station.TicketsSold += 5;
            }
            int[] expected = { 900, 1000, 1100, 1200, 1300 };
            for (int i = 0; i < expected.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }

        }

        [TestMethod()]
        public void sellTicketTest()
        {
            Station station = Station.getInstance("Vitebsk");

            station.NumberOfSeats = 50;
            station.TicketsSold = 47;
            station.sellTicket();
            station.sellTicket();
            station.sellTicket();
            station.sellTicket();
            station.sellTicket();
            station.sellTicket();
            int expected = 50;
            int actual = station.TicketsSold;
            Assert.AreEqual(expected, actual);
        }
    }
}