using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace task1.Services.Tests
{
    [TestClass()]
    public class CalculationsTests
    {
        [TestMethod()]
        public void countTest()
        {
            double[] expected = { 0.253104009, -0.721787286, 0.666638919, -0.725328831, -0.75, 0.199256809 };
            double[] actual = {
                Math.Round(Calculations.count(-1), 9),
                Math.Round(Calculations.count(-3), 9),
                Math.Round(Calculations.count(-11), 9),
                Math.Round(Calculations.count(25), 9),
                Math.Round(Calculations.count(0), 9),
                Math.Round(Calculations.count(5), 9)
            };

            for (int i = 0; i < expected.Length; i++)
            {
                Assert.AreEqual(expected[i], actual[i]);
            }
        }
    }
}