using Microsoft.VisualStudio.TestTools.UnitTesting;
using task1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task1.Tests
{
    [TestClass()]
    public class ApartmentBuildingTests
    {
        [TestMethod()]
        public void PayForHeatingInSpecificFlatTest()
        {
            Building apartmentBuilding = new ApartmentBuilding("Minsk", 3, 2, 353, 15);
            ApartmentBuilding? pureApartment = apartmentBuilding as ApartmentBuilding;

            pureApartment?.PayForHeatingInSpecificFlat(1);
            pureApartment?.PayForHeatingInSpecificFlat(2);
            pureApartment?.PayForHeatingInSpecificFlat(4);
            pureApartment?.PayForHeatingInSpecificFlat(6);
            pureApartment?.PayForHeatingInSpecificFlat(9);
            pureApartment?.PayForHeatingInSpecificFlat(12);
            pureApartment?.PayForHeatingInSpecificFlat(13);
            pureApartment?.PayForHeatingInSpecificFlat(0);

            Flat[] temp = pureApartment!.GetArrayOfFlats!;
            bool[] expected = { true, true, false, true, false, true, false, false, true, false, false, true, true, false, false };
            bool[] actual = new bool[temp.Length];
            for (int i = 0; i < temp.Length; i++)
            {
                actual[i] = temp[i].IsHeatingPaid;
            }
            CollectionAssert.AreEqual(expected, actual);

        }
    }
}