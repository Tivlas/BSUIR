namespace task1
{
    public class Station
    {
        private string? name;
        private Ticket[]? tickets;

        // indexator
        public Ticket this[int index]
        {
            get
            {
                if (index < 0 || index >= tickets!.Length)
                {
                    throw new IndexOutOfRangeException();
                }
                return tickets[index];
            }
        }

        public int numbreOfTickets = 0;

        public string Name
        {
            get => name!;
            set => name = value;
        }

        public Station(string name)
        {

            this.name = name;
        }

        public Station(string name, Ticket[] tickets) : this(name)
        {
            this.tickets = tickets;
        }

        public void AddTicket(Ticket ticket)
        {
            if (tickets == null)
            {
                tickets = new Ticket[1];
                tickets[0] = ticket;
            }
            else
            {
                Array.Resize(ref tickets, tickets.Length + 1);
                tickets[^1] = ticket;
            }
            ++numbreOfTickets;
        }

        public string? FindTicketDestinationWithLowestPrice()
        {
            if (tickets == null)
            {
                return null;
            }
            else
            {
                string? destination = null;
                double price = int.MaxValue;
                foreach (var ticket in tickets)
                {
                    if (ticket.Price < price)
                    {
                        price = ticket.Price;
                        destination = ticket.Destination;
                    }
                }
                return destination;
            }
        }
    }
}
