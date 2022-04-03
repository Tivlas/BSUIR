namespace task1
{
    class Program
    {
        static void Main(string[] args)
        {
            Building commercialBuilding = new CommercialBuilding("Minsk", "company name", 5, 1, 353, 344);
            Building apartmentBuilding = new ApartmentBuilding("Minsk", 3, 2, 353, 344);

            commercialBuilding.TurnOnTheHeating();
            Console.WriteLine($"Commercial building address: {commercialBuilding.Address}\n");
            commercialBuilding.Repair();

            CommercialBuilding? pureCommercial = commercialBuilding as CommercialBuilding;
            Console.WriteLine($"Number of cabinets: {pureCommercial?.GetNumberOfCabinets}");
            Console.WriteLine($"Number of employees: {pureCommercial?.NumberOfEmployees}");
            Console.WriteLine($"Owner company name: {pureCommercial?.GetOwnerCompany}");

            Console.WriteLine();

            apartmentBuilding.TurnOnTheHeating();
            Console.WriteLine($"Appartment building address: {apartmentBuilding.Address}\n");
            apartmentBuilding.Repair();

            ApartmentBuilding? pureApartment = apartmentBuilding as ApartmentBuilding;
            Console.WriteLine($"Number of flats: {pureApartment?.NumberOfFlats}");
            Console.WriteLine($"Number of residents: {pureApartment?.NumberOfResidents}");
            Console.WriteLine();
            
            pureApartment?.PayForHeatingInSpecificFlat(1);
            pureApartment?.PayForHeatingInSpecificFlat(34);
            pureApartment?.PayForHeatingInSpecificFlat(24);
            pureApartment?.PayForHeatingInSpecificFlat(12);
            pureApartment?.PayForHeatingInSpecificFlat(2);
            pureApartment?.PayForHeatingInSpecificFlat(3);
            pureApartment?.PayForHeatingInSpecificFlat(4);
            pureApartment?.PayForHeatingInSpecificFlat(242);

            pureApartment?.PrintListOfFlatsWithHeatingOn();



        }
    }
}
