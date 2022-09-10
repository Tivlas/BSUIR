using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace task1.Tests
{
    [TestClass()]
    public class StationTests
    {
        [TestMethod()]
        public void addTicketToCashRegisterTest()
        {
            Station station = new Station();
            Assert.AreEqual(station.NumberOfTicketsInCashRegister, 0);

            station.addTicketToCashRegister(10, "Minsk");
            Assert.AreEqual(station.NumberOfTicketsInCashRegister, 1);

            station.addTicketToCashRegister(20, "Moscow");
            Assert.AreEqual(station.NumberOfTicketsInCashRegister, 2);

            station.addTicketToCashRegister(30, "London");
            Assert.AreEqual(station.NumberOfTicketsInCashRegister, 3);
        }

        [TestMethod()]
        public void addPassengerToListTest()
        {
            Station station = new Station();
            Assert.AreEqual(station.NumberOfPassengers, 0);

            station.addPassengerToList(1, "Oleg");
            Assert.AreEqual(station.NumberOfPassengers, 1);

            station.addPassengerToList(2, "Ivan");
            Assert.AreEqual(station.NumberOfPassengers, 2);

            station.addPassengerToList(3, "Vasya");
            Assert.AreEqual(station.NumberOfPassengers, 3);
        }



        [TestMethod()]
        public void countPriceOfPassengerTicketsTest()
        {
            Station station = new Station();
            station.addTicketToCashRegister(10, "Minsk");
            station.addTicketToCashRegister(20, "Moscow");
            station.addTicketToCashRegister(30, "London");

            station.addPassengerToList(1, "Oleg");
            station.addPassengerToList(2, "Ivan");
            station.addPassengerToList(3, "Vasya");

            station.purchaseTicketForPassenger(2, "Minsk");
            station.purchaseTicketForPassenger(2, "Moscow");
            station.purchaseTicketForPassenger(3, "London");
            station.purchaseTicketForPassenger(3, "Minsk");
            station.purchaseTicketForPassenger(3, "Moscow");

            Assert.AreEqual(station.countPriceOfPassengerTickets(1), 0);
            Assert.AreEqual(station.countPriceOfPassengerTickets(2), 30);
            Assert.AreEqual(station.countPriceOfPassengerTickets(3), 60);
        }
    }
}