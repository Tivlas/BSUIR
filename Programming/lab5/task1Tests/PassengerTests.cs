using Microsoft.VisualStudio.TestTools.UnitTesting;
using task1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task1.Tests
{
    [TestClass()]
    public class PassengerTests
    {

        [TestMethod()]
        public void PurchaseTicketTest()
        {
            Passenger passenger = new Passenger();
            Assert.AreEqual(passenger.NumberOfTickets, 0);
            
            passenger.PurchaseTicket(10,"minsk");
            Assert.AreEqual(passenger.NumberOfTickets, 1);

            passenger.PurchaseTicket(15, null);
            Assert.AreEqual(passenger.NumberOfTickets, 2);
        }

        [TestMethod()]
        public void PriceOfTicketsTest()
        {
            Passenger passenger = new Passenger();
            passenger.PurchaseTicket(10, "minsk");
            passenger.PurchaseTicket(110, "minsk");
            passenger.PurchaseTicket(13, "minsk");
            passenger.PurchaseTicket(15, "minsk");
            passenger.PurchaseTicket(15, null);
            passenger.PurchaseTicket(-3, "minsk");
            Assert.AreEqual(passenger.PriceOfTickets(), 163);
        }

        [TestMethod()]
        public void FindSpecificTicketDirectionTest()
        {
            Passenger passenger = new Passenger();
            passenger.PurchaseTicket(10, "minsk");
            passenger.PurchaseTicket(110, "gomel");
            passenger.PurchaseTicket(13, "brest");
            passenger.PurchaseTicket(15, null);
            Assert.AreEqual(passenger.FindSpecificTicketDirection("minsk"), true);
            Assert.AreEqual(passenger.FindSpecificTicketDirection("gomel"), true);
            Assert.AreEqual(passenger.FindSpecificTicketDirection("brest"), true);
            Assert.AreEqual(passenger.FindSpecificTicketDirection("destination is unknown"), true);
            Assert.AreEqual(passenger.FindSpecificTicketDirection("kiev"), false);
        }
    }
}