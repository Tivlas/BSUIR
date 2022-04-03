namespace task1
{
    public class Station
    {
        private Ticket[]? cashRegister = null;

        private Passenger[]? listOfPassengers = null;

        public int NumberOfTicketsInCashRegister
        {
            get
            {
                if (cashRegister == null)
                    return 0;
                return cashRegister.Length;
            }
        }

        public int NumberOfPassengers
        {
            get
            {
                if (listOfPassengers == null)
                    return 0;
                return listOfPassengers.Length;
            }
        }

        public void addTicketToCashRegister(int price, string destination)
        {
            if (cashRegister == null)
            {
                cashRegister = new Ticket[1];
                cashRegister[^1] = new Ticket();
                cashRegister[^1].Price = price;
                cashRegister[^1].Destination = destination;
                return;
            }
            Array.Resize(ref cashRegister, cashRegister.Length + 1);
            cashRegister[^1] = new Ticket();
            cashRegister[^1].Price = price;
            cashRegister[^1].Destination = destination;
        }

        public void addPassengerToList(uint id, string? name)
        {
            name ??= "name is unknown";
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

        public void purchaseTicketForPassenger(uint id, string? destination)
        {
            destination ??= "destination is unknown";
            if (cashRegister == null)
            {
                Console.WriteLine("No tickets in cash register");
                return;
            }
            if (Array.Find(cashRegister, x => x.Destination == destination) == null)
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
            temp?.PurchaseTicket(Array.Find(cashRegister, x => x.Destination == destination)!.Price, destination);
            temp!.IsTicketPurchased = true;
        }

        public int countPriceOfPassengerTickets(uint id)
        {
            if (listOfPassengers == null)
            {
                Console.WriteLine("There are no passengers in the list");
                return 0;
            }
            Passenger? temp = Array.Find(listOfPassengers, x => x.ID == id);
            if (temp == null)
            {
                Console.WriteLine("There is no passenger with such ID");
                return 0;
            }
            int totalPrice = temp!.PriceOfTickets();
            return totalPrice;
        }

        public void printPassengersWithSpecificDirection(string? destination)
        {
            destination ??= "destination is unknown";
            if (cashRegister == null)
            {
                Console.WriteLine("There are no tickets in the cash register");
                return;
            }
            if (Array.Find(cashRegister!, x => x.Destination == destination) == null)
            {
                Console.WriteLine($"Destination to {destination} does not exist");
                return;
            }

            if (listOfPassengers == null)
            {
                Console.WriteLine("There are no passengers in the list");
                return;
            }
            Passenger[] temp = Array.FindAll(listOfPassengers, x => x.FindSpecificTicketDirection(destination) == true);
            if (temp == null)
            {
                Console.WriteLine("There are no passengers with such destination");
                return;
            }
            Console.WriteLine($"Passengers with destination to {destination}:");
            for (int i = 0; i < temp.Length; ++i)
            {
                Console.WriteLine($"#{i}: name: {temp[i].Name},   ID: {temp[i].ID}");
            }
            Console.WriteLine();
        }
    }
}

