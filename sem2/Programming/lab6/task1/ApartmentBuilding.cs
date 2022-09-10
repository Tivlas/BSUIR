namespace task1
{
    public class ApartmentBuilding : Building
    {
        private int numberOfResidents;

        private readonly int numberOfFlats;

        private Flat[]? flats = null;

        public Flat[]? GetArrayOfFlats => flats;

        public ApartmentBuilding(string address, int numberOfEntrances, int floors, int numberOfResidents, int numberOfFlats)
        {
            Floors = floors;
            NumberOfEntrances = numberOfEntrances;
            Address = address;
            this.numberOfResidents = numberOfResidents;
            this.numberOfFlats = numberOfFlats;
            flats = new Flat[numberOfFlats];
            for (int i = 0; i < numberOfFlats; i++)
            {
                flats[i] = new Flat();
            }
            SetFlatNumber();
            Build();
        }

        private void SetFlatNumber()
        {
            if (flats != null)
            {
                for (int i = 0; i < numberOfFlats; ++i)
                {
                    flats[i].FlatNumber = i + 1;
                }
            }
        }

        protected override void Build()
        {
            Console.WriteLine("Building an apartment building\n");
        }

        public int NumberOfResidents
        {
            get
            {
                return numberOfResidents;
            }
            set
            {
                if (value < 0)
                {
                    numberOfResidents = 0;
                }

                numberOfResidents = value;
            }
        }

        public int NumberOfFlats
        {
            get
            {
                return numberOfFlats;
            }
        }

        public void PayForHeatingInSpecificFlat(int flatNumber)
        {
            if (flats != null)
            {
                if (flatNumber > 0 && flatNumber <= numberOfFlats)
                {
                    flats[flatNumber - 1].IsHeatingPaid = true;
                    flats[flatNumber - 1].SetHeating();
                }
            }
        }

        public void PrintListOfFlatsWithHeatingOn()
        {
            if (flats == null)
            {
                Console.WriteLine("Flats are not created");
                return;
            }
            Flat[]? temp = Array.FindAll(flats, x => x.GetIsHeatingOn == true);
            if (temp != null)
            {
                Console.WriteLine("Flats with heating on:");
                foreach (Flat flat in temp)
                {
                    Console.Write(flat.FlatNumber + " ");
                }
                Console.WriteLine();
            }
        }

        public new void TurnOnTheHeating()
        {
            if (flats != null)
            {
                for (int i = 0; i < numberOfFlats; ++i)
                {
                    if (flats[i].IsHeatingPaid)
                    {
                        flats[i].SetHeating();
                    }
                }
            }
        }

        public override void Repair()
        {
            Console.WriteLine($"Repairing the {GetType().Name} as the residents agreed and raised money\n");
        }
    }
}
