namespace task1
{
    public class Station
    {
        private static Station? instance;

        private string? stationName = "Minsk";

        private int numberOfTicketsSold = 0;

        private int numberOfSeats = 10;

        private Ticket ticket = new Ticket();
        public string StationName
        {
            get
            {
                if (stationName is null)
                {
                    Console.WriteLine("Station does not have any name.\n");
                    return "No name";
                }
                return stationName;
            }
            set => stationName = value;
        }
        public int TicketsSold
        {
            get { return numberOfTicketsSold; }
            set
            {
                if (value < 0)
                {
                    numberOfTicketsSold = 0;
                    return;
                }
                if (value > numberOfSeats)
                {
                    numberOfTicketsSold = numberOfSeats;
                    return;
                }
                numberOfTicketsSold = value;
            }
        }
        public int NumberOfSeats
        {
            get => numberOfSeats;
            set
            {
                if (value < 0)
                {
                    numberOfSeats = 0;
                    return;
                }
                numberOfSeats = value;
            }
        }
        private Station() { }
        private Station(string name)
        {
            stationName = name;
        }
        public static Station getInstance()
        {
            if (instance is null)
                instance = new Station();
            return instance;
        }
        public static Station getInstance(string? name)
        {
            name ??= "Minsk";
            if (instance is null)
            {
                instance = new Station(name);
            }
            return instance;
        }
        public int countPriceOfUnsoldTickets()
        {
            return (numberOfSeats - numberOfTicketsSold) * ticket.Price;
        }
        public void sellTicket()
        {
            if (numberOfTicketsSold < numberOfSeats)
            {
                numberOfTicketsSold++;
            }
            else
            {
                Console.WriteLine("There are no free seats.\n");
            }
        }
        public int ChangeTicketPrice
        {
            get => ticket.Price;
            set
            {
                if (value < 0)
                {
                    ticket.Price = 0;
                    return;
                }
                ticket.Price = value;
            }
        }
    }
}
