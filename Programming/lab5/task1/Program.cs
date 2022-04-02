namespace task1
{
    class Program
    {
        static void Main(string[] args)
        {
            Station station = new Station();
            // all available tickets
            station.addTicketToCashRegister(10, "Minsk");
            station.addTicketToCashRegister(20, "Moscow");
            station.addTicketToCashRegister(30, "London");
            station.addTicketToCashRegister(40, "Paris");
            station.addTicketToCashRegister(50, "Berlin");
            station.addTicketToCashRegister(60, "Madrid");
            station.addTicketToCashRegister(70, "Rome");
            station.addTicketToCashRegister(80, "Prague");
            station.addTicketToCashRegister(90, "Budapest");

            // passengers
            station.addPassengerToList(1, "Oleg");
            station.addPassengerToList(2, "Ivan");
            station.addPassengerToList(3, "Vasiliy");
            station.addPassengerToList(4, "Petr");
            station.addPassengerToList(5, "Dmitriy");
            station.addPassengerToList(6, "Vladimir");

            //purchasing tickets
            station.purchaseTicketForPassenger(2, "Minsk");
            station.purchaseTicketForPassenger(4, "Moscow");
            station.purchaseTicketForPassenger(3, "London");
            station.purchaseTicketForPassenger(4, "Paris");
            station.purchaseTicketForPassenger(4, "Minsk");
            station.purchaseTicketForPassenger(6, "Paris");

            // print passengers
            station.printPassengersWithSpecificDirection("Minsk");
            station.printPassengersWithSpecificDirection("Paris");
            station.printPassengersWithSpecificDirection("dfafa");

            // count price
            station.countPriceOfPassengersTickets(4);


        }
    }
}
