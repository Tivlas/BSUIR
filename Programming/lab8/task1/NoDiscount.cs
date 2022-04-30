namespace task1
{
    public class NoDiscount : IPrice
    {
        private double price;

        public double GetPrice()
        {
            return price;
        }

        public NoDiscount(int price)
        {
            this.price = price;
        }
    }
}
