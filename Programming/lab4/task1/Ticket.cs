namespace task1
{
    internal class Ticket
    {
        private int price = 10;
        public int Price
        {
            get { return price; }
            set
            {
                if (value < 0)
                {
                    price = 10;
                    Console.WriteLine("You havee entered negative number. Now price is equal to 10\n");
                    return;
                }
                price = value;
            }
        }
        public Ticket() { }
    }
}
