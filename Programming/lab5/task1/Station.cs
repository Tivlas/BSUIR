namespace task1
{
    internal class Station
    {
        private Ticket[]? cashRegister = null;

        private Passenger[]? listOfPassengers = null;
        public void addTicketToCashRegister(int price, string direction)
        {
            if (cashRegister == null)
            {
                cashRegister = new Ticket[1];
                cashRegister[^1] = new Ticket();
                cashRegister[^1].Price = price;
                cashRegister[^1].Direction = direction;
                return;
            }
            Array.Resize(ref cashRegister, cashRegister.Length + 1);
            cashRegister[^1] = new Ticket();
            cashRegister[^1].Price = price;
            cashRegister[^1].Direction = direction;
        }
        public void addPassengerToList(uint id, string name)
        {
            if (listOfPassengers == null)
            {
                listOfPassengers = new Passenger[1];
                listOfPassengers[^1] = new Passenger();
                listOfPassengers[^1].Name = name;
                listOfPassengers[^1].ID = id;
                return;
            }
            Array.Resize(ref listOfPassengers, listOfPassengers.Length + 1);
            listOfPassengers[^1] = new Passenger();
            listOfPassengers[^1].Name = name;
            listOfPassengers[^1].ID = id;
        }
        public void purchaseTicketForPassenger(uint id, string direction)
        {
            if (cashRegister == null)
            {
                Console.WriteLine("No tickets in cash register");
                return;
            }
            if (Array.Find(cashRegister, x => x.Direction == direction) == null)
            {
                Console.WriteLine("There is no such ticket in the cash register");
                return;
            }

            if (listOfPassengers == null)
            {
                Console.WriteLine("There are no passengers in the list");
                return;
            }
            Passenger? temp = Array.Find(listOfPassengers, x => x.ID == id);
            if (temp == null)
            {
                Console.WriteLine("There is no passenger with such ID");
                return;
            }
            temp?.PurchaseTicket(Array.Find(cashRegister, x => x.Direction == direction)!.Price, direction);
            temp!.IsTicketPurchased = true;
        }
        public void countPriceOfPassengersTickets(uint id)
        {
            if (listOfPassengers == null)
            {
                Console.WriteLine("There are no passengers in the list");
                return;
            }
            Passenger? temp = Array.Find(listOfPassengers, x => x.ID == id);
            if (temp == null)
            {
                Console.WriteLine("There is no passenger with such ID");
                return;
            }
            int totalPrice = temp!.PriceOfTickets();
            Console.WriteLine($"The price of tickets for passenger with ID {id} is {totalPrice}");
        }
        public void printPassengersWithSpecificDirection(string direction)
        {
            if (cashRegister == null)
            {
                Console.WriteLine("There are no tickets in the cash register");
                return;
            }
            if(Array.Find(cashRegister!, x => x.Direction == direction) == null)
            {
                Console.WriteLine($"Direction to {direction} does not exist");
                return;
            }

            if (listOfPassengers == null)
            {
                Console.WriteLine("There are no passengers in the list");
                return;
            }
            Passenger[] temp = Array.FindAll(listOfPassengers, x => x.FindSpecificTicketDirection(direction) == true);
            if (temp == null)
            {
                Console.WriteLine("There are no passengers with such direction");
                return;
            }
            Console.WriteLine($"Passengers with direction to {direction}:");
            for (int i = 0; i < temp.Length; ++i)
            {
                Console.WriteLine($"#{i}: name: {temp[i].Name},   ID: {temp[i].ID}");
            }
            Console.WriteLine();
        }
    }
}

 