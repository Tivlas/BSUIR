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

        public WithDiscount(IPrice price, int discount)
        {
            this.discount = discount;
            this.price = price!.GetPrice();
        }

        public double GetPrice()
        {
            return price - price * discount / 100;
        }
    }
}
