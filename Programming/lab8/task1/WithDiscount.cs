namespace task1
{
    public class WithDiscount : IPrice
    {
        private double price;
        private double discount; // in percents

        public WithDiscount(int price, int discount)
        {
            this.price = price;
            this.discount = discount;
        }

        public double GetPrice()
        {
            return price - price * discount / 100;
        }
    }
}
