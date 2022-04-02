namespace task1
{
    internal class Passenger
    {
        public string? Name { get; set; }

        private uint id;

        private Ticket[]? tickets = null;

        private bool isTicketPurchased = false;

        public uint ID
        {
            get { return id; }
            set
            {
                if (value < 0) { id = 0; return; }
                id = value;
            }

        }

        public bool IsTicketPurchased
        {
            get { return isTicketPurchased; }
            set { isTicketPurchased = value; }
        }

        public Passenger(int id, string name)
        {
            ID = ID;
            name ??= "name";
            Name = name;
        }
        public void PurchaseTicket(int price, string direction)
        {
            if (tickets == null)
            {
                tickets = new Ticket[1];
                tickets[^1].Price = price; ;
                tickets[^1].Direction = direction;
                return;
            }
            Array.Resize(ref tickets, tickets.Length + 1);
            tickets[^1].Price = price;
            tickets[^1].Direction = direction;
        }

        public int PriceOfTickets()
        {
            if (tickets == null)
            {
                return 0;
            }
            int sum = 0;
            for (int i = 0; i < tickets.Length; i++)
            {
                sum += tickets[i].Price;
            }
            return sum;
        }

        public bool FindSpecificTicketDirection(string direction)
        {
            if (tickets == null)
            {
                return false;
            }
            for (int i = 0; i < tickets.Length; i++)
            {
                if (tickets[i].Direction == direction)
                {
                    return true;
                }
            }
            return false;
        }
    }
}
