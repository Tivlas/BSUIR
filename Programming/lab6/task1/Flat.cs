namespace task1
{
    internal class Flat
    {
        private int flatNumber;

        private bool isHeatingPaid = false;

        private bool isHeatingOn = false;
        public int FlatNumber
        {
            get { return flatNumber; }
            set
            {
                if (value < 0)
                {
                    flatNumber = 1;
                }
                flatNumber = value;
            }
        }

        public bool GetIsHeatingOn
        {
            get { return isHeatingOn; }
        }

        public bool IsHeatingPaid
        {
            get { return isHeatingPaid; }
            set { isHeatingPaid = value; }
        }
        public void SetHeating()
        {
            isHeatingOn = isHeatingPaid;
        }
    }
}
