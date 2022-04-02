namespace task1
{
    internal class Ticket
    {

        private int price = 5;
        public string? Direction { get; set; }
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
        public Ticket(int price, string direction)
        {
            this.Price = price;
            direction ??= "";
            this.Direction = direction;
        }
    }
}
