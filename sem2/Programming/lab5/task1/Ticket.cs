namespace task1
{
    public class Ticket
    {

        private int price = 5;
        public string? Destination { get; set; }
        public int Price
        {
            get { return price; }
            set
            {
                if (value < 0) { price = 0; return; }
                price = value;
            }
        }

        public Ticket() { }
        public Ticket(int price, string? destination)
        {
            this.Price = price;
            destination ??= "destination is unknown";
            this.Destination = destination;
        }
    }
}
