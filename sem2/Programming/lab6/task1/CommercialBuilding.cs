﻿namespace task1
{
    internal class CommercialBuilding : Building
    {
        private string? ownerCompany;


        private int numberOfEmployees;


        private int numberOfCabinets;

        public CommercialBuilding(string? address, string? ownerCompany, int floors, int numberOfEntrances, int numberOfEmployees, int numberOfCabinets)
        {
            Address = address;
            Floors = floors;
            ownerCompany ??= "No owner";
            this.ownerCompany = ownerCompany;
            if (numberOfEmployees < 0)
            {
                numberOfEmployees = 0;
            }
            this.numberOfEmployees = numberOfEmployees;
            this.numberOfCabinets = numberOfCabinets;
            NumberOfEntrances = numberOfEntrances;

            Build();
        }

        protected override void Build()
        {
            Console.WriteLine("Building commercial building\n");
        }

        private new bool IsTheHeatingOn { get; } // свойство из Building не переопределяется и не может быть использовано

        public int NumberOfEmployees
        {
            get => numberOfEmployees;

            set
            {
                if (value < 0)
                {
                    numberOfEmployees = 0;
                    return;
                }
                numberOfEmployees = value;
            }
        }

        public int GetNumberOfCabinets
        {
            get => numberOfCabinets;
        }

        public string GetOwnerCompany
        {
            get => ownerCompany ?? "No owner";
        }
    }
}
