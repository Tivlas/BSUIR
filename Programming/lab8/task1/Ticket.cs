namespace task1
{
    public class Ticket
    {
        private string? destination;
        private IPrice? price;

        public Ticket(string destination, IPrice price)
        {
            this.destination = destination;
            this.price = price;
        }

        public string? Destination
        {
            get => destination;
        }

        public double Price
        {
            get
            {
                return price!.GetPrice();
            }
        }
    }
}
